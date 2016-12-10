#include "bootpack.h"

#define PIT_CTRL	0x0043
#define	PIT_CNT0	0x0040
#define TIMER_FLAGS_ALLOC	1	// 已配置状态
#define	TIMER_FLAGS_USING	2	// 定时器运行中

struct TIMERCTL timerctl;

// PIT的初始化
void init_pit(void){
	int i;
	struct TIMER	*t;
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0,	0x9c);
	io_out8(PIT_CNT0, 0x2e); //0x2e9c=11932,在该数字下，中断频率为100Hz，即1秒钟会有100次中断
	timerctl.count = 0;
	for (i = 0; i < MAX_TIMER; i++){
		timerctl.timers0[i].flags = 0; //未使用
	}
	t = timer_alloc();
	t->timeout = 0xffffffff;
	t->flags = TIMER_FLAGS_USING;
	t->next_timer = 0; // 末尾
	timerctl.t0 = t; // 只有哨兵自己
	timerctl.next_time =  0xffffffff; //  因为只有哨兵，所以下一个超时时刻就是哨兵的时刻
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
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data){
	timer->fifo = fifo;
	timer->data = data;
	return;
}

// 为计时器设置超时时间
void timer_settime(struct TIMER *timer, unsigned int timeout){
	int e;
	struct TIMER *t, *s;
	timer->timeout = timeout + timerctl.count;
	timer->flags = TIMER_FLAGS_USING;
	e = io_load_eflags();
	io_cli();
	t = timerctl.t0;

	if (timer->timeout <= t->timeout){
		// 插入最前面的情况
		timerctl.t0 = timer;
		timer->next_timer = t;
		timerctl.next_time = timer->timeout;
		io_store_eflags(e);
		return;
	}
	// 寻找插入位置
	for (;;){
		s = t;
		t = t->next_timer;
		if (timer->timeout <= t->timeout){
			// 插入到s和t之间时
			s->next_timer = timer;
			timer->next_timer = t;
			io_store_eflags(e);
			return;
		}
	}
}

// IRQ0发生时所调用的中断处理程序
void inthandler20(int *esp){

	struct TIMER	*timer;
	io_out8(PIC0_OCW2, 0x60); // 把IRQ-00信号接收完了的信息通知给PIC
	timerctl.count ++;
	if (timerctl.next_time > timerctl.count){ // 比较是否到了下一个
		return;
	}
	timer = timerctl.t0;	// 将最前面的地址赋给timer
	for (;;){
		// timers 的定时器都处于动作中，所以不确认flags
		if (timer->timeout > timerctl.count){
			// timers中的定时器是根据timeout由小到大的顺序排序的，假如第一个没有到达时间
			// 则直接跳过
			break;
		}
		// 超时
		timer->flags = TIMER_FLAGS_ALLOC;
		fifo32_put(timer->fifo, timer->data);
		timer = timer->next_timer; // 下一个定时器的地址
	}
	// 新移位
	timerctl.t0 = timer;

	//	timerctl.next 的设定
	timerctl.next_time = timerctl.t0->timeout;
	return;
}
