#include "bootpack.h"
#include <stdio.h>

#define PORT_KEYDAT	0x0060
#define	PORT_KEYSTA	0x0064
#define	PORT_KEYCMD	0x0064

#define	KEYSTA_SEND_NOTREADY	0x02
#define	KEYCMD_WRITE_MODE	0x60
#define	KBC_MODE	0x47

extern struct FIFO8 keyfifo;
extern struct FIFO8	mousefifo;

void enable_mouse(void);
void init_keyboard(void);

void HariMain(void){
	/*在asmhead.nas文件中为每个地址赋值了*/
	struct BOOTINFO *binfo = (struct BOOTINFO*) 0x0ff0;
	char s[40], mcursor[256],keybuf[32],mousebuf[128];
	int mx, my,i;
	

	init_gdtidt();
	init_pic();
	io_sti();

	fifo8_init(&keyfifo,32, keybuf);	//初始化键盘缓冲
	fifo8_init(&mousefifo, 128, mousebuf);

	io_out8(PIC0_IMR, 0xf9);	
	io_out8(PIC1_IMR, 0xef);	

	init_keyboard(); // 初始化键盘控制电路

	init_palette(); /*初始化调色板*/
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx -16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);

	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s,"(%d,%d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF,s);
	
	enable_mouse();
	
	for(;;){
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0){
			io_stihlt();
		}else{
			if (fifo8_status(&keyfifo) != 0){
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			}else if (fifo8_status(&mousefifo) != 0){
				i = fifo8_get(&mousefifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 47, 31);
				putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
			}
		}
	}
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

/* 一百你确认可否键盘控制电路是否开启，发送模式设定指令，模式设定指令 0x60,
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

#define KEYCMD_SENDTO_MOUSE	0xd4
#define	MOUSECMD_ENABLE	0xf4
/* 发送激活鼠标的指令，*/
void enable_mouse(void){
	// 激活鼠标
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return; 
	//成功的话，键盘控制器返回ACL（0xfa）
}
