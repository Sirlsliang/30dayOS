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




void HariMain(void){
	int i;
	char *p ;
	
	init_palette(); /*初始化调色板*/

	p = (char*) 0xa0000;

	for(i = 0; i<= 0xffff; i++){
		p[i] = i & 0x0f;
	}
	for(;;){
		io_hlt();
	}
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
