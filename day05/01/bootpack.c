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

void HariMain(void){
	/*在asmhead.nas文件中为每个地址赋值了*/
	struct BOOTINFO *binfo = (struct BOOTINFO*) 0x0ff0;
	static  char font_A[16]={
		0x00,0x18,0x18,0x18,0x18,0x24,0x24,0x24,
		0x24,0x7e,0x42,0x42,0x42,0xe7,0x00,0x00
	};

	init_palette(); /*初始化调色板*/
	
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

	putfont8(binfo->vram,binfo->scrnx,10,10,COL8_FFFFFF, font_A);
	for(;;){
		io_hlt();
	}
}


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

void boxfill8(unsigned char *vram, int xSize, unsigned char c, int x0, int y0, int x1, int y1){
	int x,y;
	for (y = y0; y<=y1;y++){
		for (x= x0; x<=x1;x++){
			vram[y*xSize+x] = c;
		}
	}
	return;
}

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
