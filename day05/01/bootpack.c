#include<stdio.h>
/* 引用函数,这里声明了一个函数却没有用到{}，就是表明函数在别的文件中
 * ，需要编译器自动寻找*/
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

/*即使在同一个文件中，如果想在定义之前使用，还是必须事先声明一下*/
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xSize, unsigned char c, int x0, int y0,int x1, int y1);
void init_screen(char *vram, int x,int y);
void putfont8(char *vram, int xsize,int x, int y, char c, char *font);
void putfonts8_asc(char* vram, int xsize, int x, int y, char c, unsigned char* s);
void init_mouse_cursor8(char* mouse, char bc);
void putblock8_8(char* vram, int vxsize, int pxsize, int pysize, 
		int px0, int py0, char *buf, int bxsize);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15


struct BOOTINFO{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char* vram;
};

struct SEGMENT_DESCRIPTOR{
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

struct GATE_DESCRIPTOR{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void HariMain(void){
	/*在asmhead.nas文件中为每个地址赋值了*/
	struct BOOTINFO *binfo = (struct BOOTINFO*) 0x0ff0;
	char s[40], mcursor[256];
	int mx, my;

	init_gdtidt();
	init_palette(); /*初始化调色板*/
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

	mx = (binfo->scrnx -16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

	sprintf(s,"(%d,%d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF,s);

	for(;;){
		io_hlt();
	}
}

/*初始化段记录表,地址是作者自己添加的*/
void init_gdtidt(){
	//段描述符
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
	//中断描述符
	struct GATE_DESCRIPTOR		*idt = (struct GATE_DESCRIPTOR *) 0x0026f800;
	int i;
	/*GDT 的初始化*/
	for(i = 0; i < 8192; i++){
		// 指针每增加1,地址就增加8字节，原因？
		set_segmdesc(gdt + i,0,0,0);
	}
	set_segmdesc(gdt + 1,0xffffffff,0x00000000,0x4092);
	set_segmdesc(gdt + 2,0x0007ffff,0x00280000,0x409a);
	load_gdtr(0xffff, 0x002700000);

	/*IDT 的初始化*/
	for(i = 0; i < 256; i++){
		set_gatedesc(idt+i, 0, 0, 0);
	}
	load_idtr(0x7ff, 0x0026f800);
	return;
}
/*设定段描述符信息，sd:指向段描述符的指针，limit：上限， base：基址;ar:权限*/
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar){
	if (limit > 0xfffff){
		ar |= 0x8000; /* G_bit = 1*/
		limit /= 0x1000;
	}
	sd->limit_low = limit & 0xffff;
	sd->base_low 	= base	& 0xffff;
	sd->base_mid	= (base >> 16) & 0xff;
	sd->access_right	= ar & 0xff;
	sd->limit_high = ((limit>>16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high	= (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int  ar){
	gd->offset_low	= offset & 0xffff;
	gd->selector		= selector;
	gd->dw_count		=	(ar	>> 8) & 0xff;
	gd->access_right	= ar & 0xff;
	gd->offset_high	=	(offset	>> 16) & 0xffff;
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
