#include "bootpack.h"

#define FLAGS_OVERRUN 0x0001

/* 初始化FIFO8 */
void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf){
	fifo->size = size;
	fifo->buf	= buf;
	fifo->free = size;	/* 缓冲区大小 */
	fifo->flags = 0;
	fifo->p = 0;	/* 下一个数据写入地址 */
	fifo->q = 0;	/* 下一个数据读入地址 */
	return;
}

/* 写入一个字节的数据 */
int fifo8_put(struct FIFO8 *fifo, unsigned char data){
	if ( fifo->free == 0 ){
		//空余没有了，溢出
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p ++;
	if( fifo->p == fifo->size ){
		fifo->p = 0;
	}
	fifo->free--;
	return 0;
}

/* 从fifo中读出一个字节的数据 */
int fifo8_get(struct FIFO8 *fifo){
	int data;
	if(fifo->free == fifo->size){
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if(fifo->q == fifo->size){
		fifo->q = 0;
	}
	fifo->free ++;
	return data;
}

/* 查看FIFO8的状态 */
int fifo8_status(struct FIFO8 * fifo){
	return fifo->size - fifo->free;
}
