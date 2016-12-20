#include "bootpack.h"

#define FLAGS_OVERRUN		0x0001

// 栈的初始化
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task ){
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size; // 空
	fifo->flags = 0;
	fifo->p = 0; // 写入位置
	fifo->q = 0; // 读取位置
	fifo->task = task;	// 有数据写入时需要唤醒的任务
	return;
}

// 向FIFO中写入数据
int fifo32_put(struct FIFO32 *fifo, int data){
	if (fifo->free == 0){
		// 没有空余空间，溢出
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size) {
		fifo->p = 0;
	}
	fifo->free--;
	if (fifo->task != 0) {
		// 如果任务处于休眠状态，将任务唤醒
		if (fifo->task->flags != 2){
			task_run(fifo->task,-1, 0); //将任务唤醒
		}
	}
	return 0;
}

// 从栈中获取数据
int fifo32_get(struct FIFO32 *fifo){
	int data;
	if (fifo->free == fifo->size){
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if(fifo->q == fifo->size){
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

// 查询栈的状态
int fifo32_status(struct FIFO32 *fifo){
	return fifo->size - fifo->free;
}
