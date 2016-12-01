#include	"bootpack.h"
#include	<stdio.h>

#define	PORT_KEYDAT	0x0060

/* 初始化中断 */
void init_pic(void){
	io_out8(PIC0_IMR, 0xff); /* 禁止所有中断 */
	io_out8(PIC1_IMR, 0xff); /* 禁止所有中断 */

	io_out8(PIC0_ICW1, 0x11); /* 边沿触发模式 */
	io_out8(PIC0_ICW2, 0x20); /* IRQ0-7由INT20-27接收 */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接 */
	io_out8(PIC0_ICW4, 0x01 ); /* 无缓冲区模式 */

	io_out8(PIC1_ICW1, 0x11 ); /* 边沿触发模式 */
	io_out8(PIC1_ICW2, 0x28 ); /* IRQ8-15由INT28-2f接收 */
	io_out8(PIC1_ICW3, 2); /* PIC1由IRQ2连接 */
	io_out8(PIC1_ICW4, 0x01 ); /* 无缓冲区模式 */

	io_out8(PIC0_IMR, 0xfb); /* 11111011 PIC1以外全部禁止 */
	io_out8(PIC1_IMR, 0xff); /* 11111111 禁止所有中断 */
	return;
}

struct  KEYBUF keybuf;
/* 处理键盘中断 */
void inthandler21(int *esp){
	unsigned char data;
	io_out8(PIC0_OCW2,	0x61); /* 通知PIC “IRQ-01已经受理完毕”,这样PIC继续监视IRQ1是否执行中断 */
	data = io_in8(PORT_KEYDAT);
	if (keybuf.next < 32){
		keybuf.data[keybuf.next] = data;
		keybuf.next++;
	}
	return;
}

/* 处理鼠标中断 */
void inthandler2c(int *esp){
	struct BOOTINFO* binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2c (IRQ-12):PS/2 mouse");
	for(;;){
			io_hlt();
	}
}

/* 对于部分机器，随着PIC的初始化，会产生一次QRQ7中断，如果不对该中断处理程序执行STI(设置中断标志位)，操作系统会启动
 * 失败*/
void inthandler27(int *esp){
	io_out8(PIC0_OCW2,	0x67);
	return;
}
