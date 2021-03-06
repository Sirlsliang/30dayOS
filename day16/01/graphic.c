/* 关于向屏幕描画的处理 */
#include "bootpack.h"

/*初始化调色板*/
void init_palette(void){
	static unsigned char table_rgb[16 * 3]={
		0x00,0x00,0x00,		/*	0:黑	*/
		0xff,0x00,0x00,		/*	1:亮红	*/
		0x00,0xff,0x00,		/*	2:亮绿	*/
		0xff,0xff,0x00,		/*	3:亮黄	*/
		0x00,0x00,0xff,		/*	4:亮蓝	*/
		0xff,0x00,0xff,		/*	5:亮紫	*/
		0x00,0xff,0xff,		/*	6:浅亮蓝	*/
		0xff,0xff,0xff,		/*	7:白	*/
		0xc6,0xc6,0xc6,		/*	8:亮灰	*/
		0x84,0x00,0x00,		/*	9:暗红	*/
		0x00,0x84,0x00,		/*	10:暗绿	*/
		0x84,0x84,0x00,		/*	11:暗黄	*/
		0x00,0x00,0x84,		/*	12:暗青	*/
		0x84,0x00,0x84,		/*	13:暗紫	*/
		0x00,0x84,0x84,		/*	14:浅暗紫	*/
		0x84,0x84,0x84,		/*	15:暗灰	*/
	};
	set_palette(0,15,table_rgb); 
	return;
}

/*设置调色板*/
void set_palette(int start, int end, unsigned char *rgb){
	int i,eflags;
	eflags = io_load_eflags(); 	/*记录中断许可的标志*/
	io_cli();										/*将中断许可标志置为0,禁止中断*/
	io_out8(0x03c8, start);
	for(i = start; i <= end; i++){
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags); 		/* 复原中断许可标志*/
	return;
}

/*绘制屏幕，基本颜色及样式*/
void init_screen(char* vram, int xSize, int ySize){

	boxfill8(vram, xSize, COL8_008484, 0,		0,				xSize - 1, ySize-29);
	boxfill8(vram, xSize, COL8_C6C6C6, 0,		ySize-28,	xSize - 1, ySize-28);
	boxfill8(vram, xSize, COL8_FFFFFF, 0,		ySize-27,	xSize - 1, ySize-27);
	boxfill8(vram, xSize, COL8_C6C6C6, 0,		ySize-26,	xSize - 1, ySize- 1);

	boxfill8(vram, xSize, COL8_FFFFFF, 3,		ySize-24,	59,				 ySize-24);
	boxfill8(vram, xSize, COL8_FFFFFF, 2,		ySize-24,	 2,				 ySize- 4);
	boxfill8(vram, xSize, COL8_848484, 3,		ySize- 4,	59,				 ySize- 4);
	boxfill8(vram, xSize, COL8_848484,59,		ySize-23,	59,				 ySize- 5);
	boxfill8(vram, xSize, COL8_000000, 2,		ySize- 3,	59,				 ySize- 3);
	boxfill8(vram, xSize, COL8_000000,60,		ySize-24,	60,				 ySize- 3);

	boxfill8(vram, xSize, COL8_848484,xSize-47, ySize-24,	xSize- 4, ySize-24);
	boxfill8(vram, xSize, COL8_848484,xSize-47, ySize-23,	xSize-47, ySize- 4);
	boxfill8(vram, xSize, COL8_FFFFFF,xSize-47, ySize- 3,	xSize- 4, ySize- 3);
	boxfill8(vram, xSize, COL8_FFFFFF,xSize- 3, ySize-24,	xSize- 3, ySize- 3);
	return;
}

/*绘制矩形*/
void boxfill8(unsigned char *vram, int xSize, unsigned char c, int x0, int y0, int x1, int y1){
	int x,y;
	for (y = y0; y<=y1;y++){
		for (x= x0; x<=x1;x++){
			vram[y*xSize+x] = c;
		}
	}
	return;
}

/*准备鼠标指针*/
void init_mouse_cursor8(char* mouse, char bc){
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***",
	};
	int x, y;
	for(y = 0; y < 16; y++){
		for(x = 0; x < 16; x++){
			if (cursor[y][x] == '*'){
				mouse[y*16+x] = COL8_000000;
			}
			if (cursor[y][x] == 'O'){
				mouse[y*16+x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.'){
				mouse[y*16+x] = bc;
			}
		}
	}
	return;
}

/* 显示字符串*/
void putfonts8_asc(char* vram, int xsize, int x, int y, char c, unsigned char* s){
	extern char hankaku[4096];
	for(;*s != 0x00;s++){
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		/*每个字符的宽度是8*/
		x += 8;
	}
	return;
}

/*显示字体*/
void putfont8(char *vram, int xsize,int x, int y, char c, char *font){
	int i;
	char*p ,d; /*data*/
	for (i=0;i<16;i++){
			p = vram + (y+i) * xsize +x;
			d = font[i];
			if ((d & 0x80) != 0) { p[0] = c; }
			if ((d & 0x40) != 0) { p[1] = c; }
			if ((d & 0x20) != 0) { p[2] = c; }
			if ((d & 0x10) != 0) { p[3] = c; }
			if ((d & 0x08) != 0) { p[4] = c; }
			if ((d & 0x04) != 0) { p[5] = c; }
			if ((d & 0x02) != 0) { p[6] = c; }
			if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

/*在屏幕上显示鼠标,即将鼠标的值赋给屏幕*/
void putblock8_8(char* vram, int vxsize, int pxsize, int pysize,
		int px0, int py0, char *buf, int bxsize){
	int x,y;
	for (y=0;y < pysize; y++){
		for (x =0;x < pxsize; x++){
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}


