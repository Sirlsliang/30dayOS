#include "bootpack.h"

struct TASKCTL *taskctl;
struct TIMER *task_timer;

// 返回当前正在运行的任务的地址
struct TASK *task_now(void){
	struct TASKLEVEL *tl = &taskctl->level[taskctl->now_lv];
	return tl->tasks[tl->now];
}

// 向TASKLEVEL中添加一个任务
void task_add(struct TASK *task){
	struct TASKLEVEL *tl = &taskctl->level[task->level];
	tl->tasks[tl->running] = task;
	tl->running++;
	task->flags = 2; //活动中
	return;
}

void task_remove(struct TASK *task){
	int i;
	struct TASKLEVEL *tl = &taskctl->level[task->level];

	// 寻找task所在的位置
	for (i = 0; i < tl->running; i++){
		if (tl->tasks[i] == task) {
			break;
		}
	}

	tl->running--;
	if (i < tl->now) {
		tl->now--; // 需要移动任务成员，要相应的处理
	}
	if (tl->now >= tl->running) {
		tl->now = 0; // 对now进行修正
	}

	task->flags = 1; // 任务休眠
	// 移动
	for (;i < tl->running; i++){
		tl->tasks[i] = tl->tasks[i + 1];
	}
	return;
}

void task_switchsub(void){
	int i;
	// 寻找最上层的leve
	for (i = 0; i < MAX_TASKLEVELS; i++){
		if (taskctl->level[i].running > 0){
			break;
		}
	}
	taskctl->now_lv = i;
	taskctl->lv_change = 0;
	return;
}

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
	for (i = 0; i < MAX_TASKLEVELS; i++) {
		taskctl->level[i].running = 0;
		taskctl->level[i].now = 0;
	}
	task = task_alloc();
	task->flags = 2; // 活动中的标志
	task->priority = 2;	// 0.02秒
	task->level = 0; // 最高level
	task_add(task);
	task_switchsub();	// level设置
	load_tr(task->sel); // 加载当前任务的GDT编号，
	task_timer = timer_alloc();
	timer_settime(task_timer, task->priority);
	return task;
}

// 获取一个未使用的task
struct TASK *task_alloc(void){
	int i;
	struct TASK *task;
	for (i = 0; i < MAX_TASKS; i++) {
		if (taskctl->tasks0[i].flags == 0) {
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
void task_run(struct TASK *task, int level, int priority){
	if (level < 0) {
		level = task->level; // 不改变level
	}
	if (priority > 0) {	// 优先级为0则不改变已经设定的优先级
		task->priority = priority;
	}

	if (task->flags == 2 && task->level != level){
		// 改变活动中的level
			task_remove(task); // 这会将task的flag设置为1
	}

	if (task->flags != 2) {
		task->level = level;
		task_add(task);
	}
	taskctl->lv_change = 1; // 下次任务切换时检查level
	return;
}

// 任务休眠，即将任务从任务列表中删除
void task_sleep(struct TASK *task){
	struct TASK *now_task;
	if (task->flags == 2) { //如果指定任务处于唤醒状态
		now_task = task_now();
		task_remove(task); // 将task 的flags设置为1
		if (task == now_task) {
			task_switchsub();	// 如果是让自己休眠，修要进行任务切换
			now_task = task_now(); // 设定后获取当前任务的值
			farjmp(0, now_task->sel);
		}
	}
	return;
}

void task_switch(void){
	struct  TASKLEVEL *tl = &taskctl->level[taskctl->now_lv];
	struct TASK *new_task, *now_task = tl->tasks[tl->now];
	tl->now++;
	if (tl->now == tl->running) {
		tl->now = 0;
	}
	if (taskctl->lv_change != 0) {
		task_switchsub();
		tl = &taskctl->level[taskctl->now_lv];
	}
	new_task = tl->tasks[tl->now];
	timer_settime(task_timer, new_task->priority);
	if (new_task != now_task) {
		farjmp(0, new_task->sel);
	}
	return;
}
