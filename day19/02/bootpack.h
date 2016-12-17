/* asmhead.nas */
struct BOOTINFO{ /* 0x0ff0-0x0fff */
	char cyls;		/* 启动区读硬盘读到何处为止 */
	char leds;		/* 启动时键盘LED的状态 */
	char vmode; 	/* 显卡模式为多少位色彩 */
	char reserve; 	
	short scrnx, scrny; /*  画面分辨率 */
	char *vram;
};
#define ADR_BOOTINFO  0x00000ff0
#define	ADR_DISKIMG		0x00100000

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void load_tr(int tr);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void farjmp(int eip, int cs);

/* fifo.c */
struct FIFO32{
	int *buf;
	int p, q, size, free, flags;
	struct TASK *task;
};
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task);
int  fifo32_put(struct FIFO32 *fifo, int data);
int  fifo32_get(struct FIFO32 *fifo);
int  fifo32_status(struct FIFO32 *fifo);


/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xSize, unsigned char c, int x0, int y0,int x1, int y1);
void init_screen(char *vram, int xsize,int ysize);
void putfont8(char *vram, int xsize,int x, int y, char c, char *font);
void putfonts8_asc(char*vram, int xsize, int x, int y, char c, unsigned char * s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, 
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

/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

#define ADR_IDT		0x0026f800
#define	LIMIT_IDT	0x000007ff
#define	ADR_GDT		0x00270000
#define	LIMIT_GDT	0x0000ffff
#define	ADR_BOTPAK	0x00280000
#define	LIMIT_BOTPAK	0x0007ffff
#define	AR_DATA32_RW	0x4092
#define	AR_CODE32_ER	0x409a
#define	AR_TSS32			0x0089
#define	AR_INTGATE32	0x008e

/* int.c */
void init_pic(void);
void inthandler27(int *esp);
#define PIC0_ICW1	0x0020
#define PIC0_OCW2	0x0020

#define PIC0_IMR	0x0021
#define	PIC0_ICW2	0x0021
#define	PIC0_ICW3	0x0021
#define	PIC0_ICW4	0x0021

#define	PIC1_ICW1	0x00a0
#define	PIC1_OCW2	0x00a0
#define	PIC1_IMR	0x00a1
#define	PIC1_ICW2	0x00a1
#define	PIC1_ICW3	0x00a1
#define	PIC1_ICW4	0x00a1

/* keyboard.c */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO32 *fifo, int data0);
#define	PORT_KEYDAT	0x0060
#define	PORT_KEYCMD	0x0064


/* mouse.c */
struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};
void inthandler2c(int *esp);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

/* memory.c */
#define MEMMAN_FREES	4090	/*4090*8/1024 = 31KB */
#define MEMMAN_ADDR	0x003c0000

struct FREEINFO{	//可用信息
	unsigned int addr;	// 分配内存的起始地址
	unsigned int size;	// 分配内存的大小
};

struct MEMMAN{ 		// 内存管理 
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO	free[MEMMAN_FREES];
};

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

/* sheet.c */
#define MAX_SHEETS	256 // 能够管理的最大层数
// 图层结构体
struct SHEET{
	struct SHTCTL *ctl;
	unsigned char* buf; // 记录图层上所描画内容的图纸
	int bxsize;
	int bysize;		// 两个对应着图层的大小
	int vx0;
	int vy0;			// 图层在画面上的位置的坐标
	int col_inv;	// 透明色色号
	int height;		// 图层高度
	int flags;		// 图层的设定信息
};

// 图层的管理结构体
struct SHTCTL{
	unsigned char *vram, *map;//利用map来映射vram中各个图层的信息
	int xsize, ysize; // vram 及 xsize 和 yszie 代表屏幕的地址及大小,
	int	top; //	top代表最上面图层的高度
	struct SHEET *sheets[MAX_SHEETS];	//	用来存放图层排序信息
	struct SHEET sheets0[MAX_SHEETS];	//	存放图层信息
};

struct SHTCTL* shtctl_init(struct MEMMAN * memman, unsigned char* vram,int xsize, int ysize);
struct SHEET* sheet_alloc(struct SHTCTL* ctl);
void sheet_updown(struct SHEET* sht, int height);
void sheet_setbuf(struct SHEET* sht, unsigned char* buf, int xsize, int ysize, int col_inv);
void sheet_refresh(struct SHEET* sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHEET* sht, int vx0, int vy0);
void sheet_free(struct SHEET* sht);

/* timer.c */
#define MAX_TIMER 500
struct TIMER {
	struct TIMER *next_timer;
	unsigned int timeout, flags; // 记录离超时还有多长时间,一旦剩余时间为0,程序就往FIFO缓冲区中发送数据
	struct FIFO32 *fifo;
	int data;
};

struct TIMERCTL {
	// using显示定时器的数目
	unsigned int count, next_time;
	struct TIMER *t0;
	struct TIMER timers0[MAX_TIMER];
};
extern struct TIMERCTL timerctl;
void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);
void timer_settime(struct TIMER *timer, unsigned int timeout);
void inthandler20(int *esp);

/* mtask.c */
#define MAX_TASKS	1000  // 最大的任务量数
#define TASK_GDT0	3			// 定义从GDT的几号开始分配给TSS
#define	MAX_TASKS_LV	100	// 定义最多的任务级别
#define	MAX_TASKLEVELS 10	// 定义最大的运行级别

struct TSS32{
	// 任务相关的设置，任务切换时CPU不会写入内容
	int backlink, esp0,	ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi; // 32位寄存器
	int es, cs, ss, ds, fs, gs; // 16位寄存器
	int ldtr, iomap; // 任务相关的设置，任务切换CPU不会写入内容
};

struct TASK{
	// sel用来存放GDT的编号,selector(选择符)的缩写，表示从GDT里面选
	// 择哪个编号
	int sel, flags;  
	int priority,level; // 定义任务的优先级
	struct FIFO32 fifo;
	struct TSS32 tss;
};

struct TASKLEVEL {
	int running; // 正在运行的任务数量
	int now;		 // 正在运行的任务
	struct TASK *tasks[MAX_TASKS_LV];
};

struct TASKCTL{
	int now_lv;				// 现在活动中的level
	char lv_change;		// 下次任务切换时，是否需要改变level
	struct TASKLEVEL level[MAX_TASKLEVELS]; // 任务的级别
	struct TASK tasks0[MAX_TASKS];
};

extern struct TIMER *task_timer;
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run(struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep(struct TASK *task);

/* window.c */
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
// (x, y): 坐标; c:字符颜色; b: 背景颜色; s:字符串; l:字符串长度
void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act);

/* console.c */
void console_task(struct SHEET *sheet, unsigned int memtotal);
int cons_newline(int cursor_y, struct SHEET *sheet);

/* file.c */
struct FILEINFO {
	unsigned char not[32];
	unsigned char name[8], ext[3], type;
	char reserve[10];
	unsigned short time, date, clustno;
	unsigned int size;
};
void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);
