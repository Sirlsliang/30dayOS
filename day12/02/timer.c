#include "bootpack.h"

#define PIT_CTRL	0x0043
#define	PIT_CNT0	0x0040
#define TIMER_FLAGS_ALLOC	1	// 已配置状态
#define	TIMER_FLAGS_USING	2	// 定时器运行中

struct TIMERCTL timerctl;

// PIT的初始化
void init_pit(void){
	int i;
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0,	0x9c);
	io_out8(PIT_CNT0, 0x2e); //0x2e9c=11932,在该数字下，中断频率为100Hz，即1秒钟会有100次中断
	timerctl.count = 0;
	timerctl.next = 0xffffffff; // 最初没有正在运行的定时器

	timerctl.using = 0;
	for (i = 0; i < MAX_TIMER; i++){
		timerctl.timers0[i].flags = 0; //未使用
	}
	return;
}

// 获取一个未使用的计时器
struct TIMER *timer_alloc(void){
	int i;
	for (i = 0; i < MAX_TIMER; i++){
		if (timerctl.timers0[i].flags == 0){
			timerctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
			return &timerctl.timers0[i];
		}
	}
	return 0;	// 未找到
}

//	释放计时器
void timer_free(struct TIMER *timer){
	timer->flags = 0; //未使用
	return;
}

// 初始化计时器
void timer_init(struct TIMER *timer, struct FIFO8 *fifo, unsigned char data){
	timer->fifo = fifo;
	timer->data = data;
	return;
}

// 为计时器设置超时时间
void timer_settime(struct TIMER *timer, unsigned int timeout){
	int e, i, j;
	timer->timeout = timeout + timerctl.count;
	timer->flags = TIMER_FLAGS_USING;
	e = io_load_eflags();
	io_cli();
	// 搜索注册位置
	for (i = 0; i < timerctl.using; i++){
		if (timerctl.timers[i]->timeout >= timer->timeout){
			break;
		}
	}
	for (j = timerctl.using; j > i; j--){
		timerctl.timers[j] = timerctl.timers[j - 1];
	}
	timerctl.using++;
	// 插入到空位上
	timerctl.timers[i] = timer;
	timerctl.next = timerctl.timers[0]->timeout;
	io_store_eflags(e);
	return;
}

// IRQ0发生时所调用的中断处理程序
void inthandler20(int *esp){
	int i, j;
	io_out8(PIC0_OCW2, 0x60); // 把IRQ-00信号接收完了的信息通知给PIC
	timerctl.count ++;
	if (timerctl.next > timerctl.count){ // 比较是否到了下一个
		return;
	}
	// 将timerctl.next的值设为最大，方便后面的使用
	timerctl.next = 0xffffffff;
	for (i = 0; i < timerctl.using; i++){
		// timers 的定时器都处于动作中，所以不确认flags
		if (timerctl.timers[i]->timeout > timerctl.count){
			// timers中的定时器是根据timeout由小到大的顺序排序的，假如第一个没有到达时间
			// 则直接跳过
			break;
		}
		timerctl.timers[i]->flags = TIMER_FLAGS_ALLOC;
		fifo8_put(timerctl.timers[i]->fifo, timerctl.timers[i]->data);
	}
	// 有i个定时器超时了，对它们进行移位
	timerctl.using -= i;
	for (j = 0; j < timerctl.using; j++){
		timerctl.timers[j] = timerctl.timers[i+j];
	}
	if (timerctl.using > 0){
		timerctl.next = timerctl.timers[0]->timeout;
	} else {
		timerctl.next = 0xffffffff;
	}
	return;
}
