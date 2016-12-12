#include "bootpack.h"

struct TASKCTL *taskctl;
struct TIMER *task_timer;

// 任务段初始化设置，
//为从第三段开始的段描述符添加任务描述符
struct TASK *task_init(struct MEMMAN *memman){
	int i;
	struct TASK *task;
	struct SEGMENT_DESCRIPTOR	*gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	taskctl = (struct TASKCTL *) memman_alloc_4k(memman, sizeof(struct TASKCTL));
	for (i = 0; i < MAX_TASKS; i++) {
		taskctl->tasks0[i].flags = 0;
		taskctl->tasks0[i].sel 	= (TASK_GDT0 + i) * 8; // 设置任务的GDT编号
		set_segmdesc(gdt + TASK_GDT0 + i, 103, (int) &taskctl->tasks0[i].tss, AR_TSS32);
	}
	task = task_alloc();
	task->flags = 2; // 活动中的标志
	task->priority = 2;	// 0.02秒
	taskctl->running = 1;
	taskctl->now = 0;
	taskctl->tasks[0] = task;
	load_tr(task->sel); // 加载当前任务的GDT编号，
	task_timer = timer_alloc();
	timer_settime(task_timer, task->priority);
	return task;
}

// 获取一个未使用的task
struct TASK *task_alloc(void){
	int i;
	struct TASK *task;
	for (i = 0; i < MAX_TASKS; i++){
		if (taskctl->tasks0[i].flags == 0){
			task = &taskctl->tasks0[i];
			task->flags = 1; // 正在使用的标志
			task->tss.eflags = 0x00000202; // IF = 1;
			task->tss.eax = 0;
			task->tss.ecx = 0;
			task->tss.edx = 0;
			task->tss.ebx = 0;
			task->tss.ebp = 0;
			task->tss.esi = 0;
			task->tss.edi = 0;
			task->tss.es = 0;
			task->tss.ds = 0;
			task->tss.fs = 0;
			task->tss.gs = 0;
			task->tss.ldtr = 0;
			task->tss.iomap = 0x40000000;
			return task;
		}
	}
	return 0;
}

// 通过改变任务的优先级来改变
void task_run(struct TASK *task, int priority){
	if (priority > 0) {	// 优先级为0则不改变已经设定的优先级
		task->priority = priority;
	}
	if (task->flags != 2){
		task->flags = 2; 				// 活动中的标志
		taskctl->tasks[taskctl->running] = task;
		taskctl->running++;
	}
	return;
}

void task_switch(void){
	struct TASK *task;
	taskctl->now++;
	if (taskctl->now == taskctl->running){
		taskctl->now = 0;
	}
	task = taskctl->tasks[taskctl->now];
	timer_settime(task_timer, task->priority);
	if (taskctl->running >= 2) {
		farjmp(0, task->sel);
	}
	return;
}

// 任务休眠，即将任务从任务列表中删除
void task_sleep(struct TASK *task){
	int i;
	char ts = 0;
	if (task->flags == 2){ //如果指定任务处于唤醒状态
		if (task == taskctl->tasks[taskctl->now]){
			ts = 1;	// 让自己休眠的话，稍后需要进行任务切换?
		}
		// 寻找task所在的位置
		for (i = 0; i < taskctl->running; i++){
			if (taskctl->tasks[i] == task){
				break;
			}
		}
		taskctl->running--;
		if (i < taskctl->now){
			taskctl->now--; // 后面需要移动成员，now会减少一个
		}
		// 移动任务成员
		for (; i< taskctl->running; i++){
			taskctl->tasks[i] = taskctl->tasks[i + 1];
		}
		task->flags = 1; // 不工作的状态
		if (ts != 0){
			// 要删除的任务正在运行
			if (taskctl->now >= taskctl->running){
				// now 值出现异常，进行修正
				taskctl->now = 0;
			}
			farjmp(0, taskctl->tasks[taskctl->now]->sel);
		}
	}
	return;
}
