#include "bootpack.h"
#include <stdio.h>

unsigned int memtest(unsigned int start, unsigned int end);

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
	enable_mouse(&mdec); // 初始化鼠标

	init_palette(); /*初始化调色板*/
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx -16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);

	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s,"(%d,%d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF,s);
	
	i = memtest(0x00400000, 0xbfffffff) / (1024 * 1024);
	sprintf(s, "memeory %dMB", i);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 32, COL8_FFFFFF, s);
	
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

#define EFLAGS_AC_BIT			0x00040000
#define	CR0_CACHE_DISABLE	0x60000000

unsigned int memtest(unsigned int start, unsigned int end){
	char flag486 = 0;
	unsigned int eflag, cr0, i;
	/* 确认CPU是386还是486以上的 */
	eflag = io_load_eflags();
	/* AC-bit = 1*/
	eflag |= EFLAGS_AC_BIT; // 会将 15位设置为1
	io_store_eflags(eflag);
	eflag = io_load_eflags();
	/* 如果是386, 即使设定AC=1,AC的值还会自动回到0 */
	if((eflag & EFLAGS_AC_BIT) != 0){ // 检查15 位是否为1,
			flag486 = 1;
	}
	eflag &= ~EFLAGS_AC_BIT; //AC-bit = 0
	io_store_eflags(eflag);

	if(flag486 != 0){
		cr0 = load_cr0();
		cr0 != CR0_CACHE_DISABLE; // 禁止缓存
		store_cr0(cr0);
	}

	i = memtest_sub(start, end);

	if(flag486 != 0){
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE; //允许缓存
		store_cr0(cr0);
	}
	return i;
}
