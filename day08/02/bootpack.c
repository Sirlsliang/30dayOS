#include "bootpack.h"
#include <stdio.h>

#define PORT_KEYDAT	0x0060
#define	PORT_KEYSTA	0x0064
#define	PORT_KEYCMD	0x0064

#define	KEYSTA_SEND_NOTREADY	0x02
#define	KEYCMD_WRITE_MODE	0x60
#define	KBC_MODE	0x47

extern struct FIFO8 keyfifo, mousefifo;

struct MOUSE_DEC{
	unsigned char buf[3], phase;
	int x, y, btn;
};
void init_keyboard(void);
void enable_mouse(struct MOUSE_DEC *mdec);
int  mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);


void HariMain(void){
	/*在asmhead.nas文件中为每个地址赋值了*/
	struct BOOTINFO *binfo = (struct BOOTINFO*) ADR_BOOTINFO;
	struct MOUSE_DEC mdec;
	char s[40], mcursor[256],keybuf[32],mousebuf[128];
	int mx, my,i;
	

	init_gdtidt();
	init_pic();
	io_sti(); // IDT/PIC的初初始化已经完成，放开CPU的中断

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
	
	enable_mouse(&mdec);
	
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
				if(mouse_decode(&mdec, i) != 0){
					/* 鼠标的三个字节都已经收集齐了,将他们显示出来 */
					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if((mdec.btn & 0x01) != 0){ //	判断最低位是否为1
						s[1] ='L';
					}
					if((mdec.btn & 0x02) != 0){	//	判断中间位是否为1
						s[3] ='R';
					}
					if((mdec.btn & 0x04) != 0){	//	判断最高位是否为1
						s[2] ='C';
					}
					// 清除原有位置的信息
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 -1, 31);
					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
					/* 鼠标移动 */
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15); // 隐藏鼠标
					mx += mdec.x;
					my += mdec.y;

					if(mx < 0){
						mx = 0;
					}
					if(my<0){
						my = 0;
					}
					if(mx > binfo->scrnx - 16){
						mx =  binfo->scrnx - 16;
					}
					if(my > binfo->scrny - 16){
						my =	binfo->scrny - 16;
					}
					sprintf(s,"(%3d, %3d)", mx, my);
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79,15);
					// 显示坐标
					putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
					// 描绘鼠标
					putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

				}
			} 
		}
	}
}


int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat){
	if (mdec->phase == 0){
		// 等待鼠标的 0xfa阶段
		if(dat == 0xfa){
				mdec->phase = 1;
		}
		return 0;
	}
	if(mdec->phase == 1){
		// 等待鼠标第一字节的阶段
		//用c8的原因是dat的前一位一直在0-3变化,这样能够保证不论怎么变化计算后一直为0
		if ((dat & 0xc8) == 0x08){ 
			// 如果第一字节正确
			mdec->buf[0] = dat;
			mdec->phase = 2;
		}
		return 0;
	}
	if(mdec->phase == 2){
		// 等待鼠标的第二字节阶段
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}
	if(mdec->phase == 3){
		// 等待鼠标的第三字节阶段
		mdec->buf[2] = dat;
		mdec->phase = 1;
		// 记录按键的信息,不按是8,利用0x07读取鼠标的按键状态
		mdec->btn = mdec->buf[0] & 0x07; 
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];

		if((mdec->buf[0] & 0x10) != 0){
			mdec->x |= 0xffffff00;
		}
		if((mdec->buf[0] & 0x20) != 0){
			mdec->y |= 0xffffff00;
		}
		mdec->y = -mdec->y; // 鼠标的方向与画面符号相反
		return 1;

	}
	return -1;
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
void enable_mouse(struct MOUSE_DEC *mdec){
	// 激活鼠标
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	//成功的话，键盘控制器返回ACK即0xfa
	mdec->phase = 0;
	return; 
}
