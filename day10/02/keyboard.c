#include "bootpack.h"

#define	PORT_KEYSTA	0x0064
#define	KEYSTA_SEND_NOTREADY	0x02
#define	KEYCMD_WRITE_MODE	0x60
#define	KBC_MODE	0x47

/* 确认键盘控制电路是否开启，发送模式设定指令，模式设定指令 0x60,
	利用鼠标模式的模式号码：0x47*/
void  init_keyboard(void){
	/* 初始化键盘控制电路 */
	wait_KBC_sendready();
	//发送模式设定指令
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	//设定控制电路
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

/* 让键盘控制电路KBC做好准备，等待控制指令的到来 */
void wait_KBC_sendready(void){
	/* 等待控制电路准备完毕 */
	for(;;){
		if((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0){
			break;
		}
	}
	return;
}

/* 处理键盘中断 */
struct  FIFO8 keyfifo;
void inthandler21(int *esp){
	unsigned char data;
	io_out8(PIC0_OCW2,	0x61); /* 通知PIC “IRQ-01已经受理完毕”,这样PIC继续监视IRQ1是否执行中断 */
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo,data);
	return;
}

