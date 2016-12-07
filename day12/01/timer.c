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
	for (i = 0; i < MAX_TIMER; i++){
		timerctl.timer[i].flags = 0; //未使用
	}
	return;
}

// 获取一个未使用的计时器
struct TIMER *timer_alloc(void){
	int i;
	for (i = 0; i < MAX_TIMER; i++){
		if (timerctl.timer[i].flags == 0){
			timerctl.timer[i].flags = TIMER_FLAGS_ALLOC;
			return &timerctl.timer[i];
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
	timer->timeout = timeout;
	timer->flags = TIMER_FLAGS_USING;
	return;
}

// IRQ0发生时所调用的中断处理程序
void inthandler20(int *esp){
	int i;
	io_out8(PIC0_OCW2, 0x60); // 把IRQ-00信号接收完了的信息通知给PIC
	timerctl.count ++;
	for (i = 0; i < MAX_TIMER; i++){
		if (timerctl.timer[i].flags == TIMER_FLAGS_USING){ // 如果已经设定了超时
			timerctl.timer[i].timeout --;
			if (timerctl.timer[i].timeout == 0){
				timerctl.timer[i].flags = TIMER_FLAGS_ALLOC;
				fifo8_put(timerctl.timer[i].fifo, timerctl.timer[i].data);
			}
		}
	}
	return;
}
