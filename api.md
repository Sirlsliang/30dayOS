## 系统中各个函数的API

#### asmhead.nas
> 系统启动时所需信息的设定
- struct BOOTINFO{ /* 0x0ff0-0x0fff */
	char cyls;		/* 启动区读硬盘读到何处为止 */
	char leds;		/* 启动时键盘LED的状态 */
	char vmode; 	/* 显卡模式为多少位色彩 */
	char reserve; 	
	short scrnx, scrny; /*  画面分辨率 */
	char* vram;
};
#### naskfunc.nas
> 在该文件中使用汇编语言实现的函数，可在C语言文件中使用
- void io_hlt(void)
	处理器"暂停",当复位线上有复位信号，CPU响应非屏蔽中断，CPU相应可屏蔽中断时，CPU
	脱离暂停状态
- void io_cli(void)
	关闭系统中断,禁止中断发生
- void io_sti(void)
	允许中断发生
- void io_stihlt(void)
	开启中断后，使就系统暂停
- io_in8(int port)
	CPU从设备(port)中接收信号
- io_out8(int port, int data)
	CPU向设备port发送数据data
- int io_load_eflags(void)
	读取EFLAGS寄存器中的标志位
- void io_store_eflags(int eflags)
	设置EFLAGS寄存器中的标志位?
- void load_gdtr(int limit, int addr)
	将指定的段上限和地址赋值给GDTR的48位寄存器
- void load_idtr(int limit, int addr)
	将制定的上限和地址赋值给IDTR的寄存器？
- void asm_inthandler20(void)
	处理定时器的中断
- void asm_inthandler21(void)
	处理键盘中断信息
- void asm_inthandler27(void)
- void asm_inthandler2c(void)
	处理鼠标中断信息
- unsigned int memtest_sub(unsigned int start, unsigned int end)
- void farjmp(int eip, int cs)
	任务切换，eip值下次要执行的指令，cs指段号

#### fifo.c
> 先入先出栈的实现
- void fifo32_init(struct FIFO32 *fifo, int size, int *buf)
	栈的初始化, fifo:要初始化的结构体, size:栈的大小，buf：栈存储数据的地址
- int fifo32_put(struct FIFO32 *fifo, int data)
	向栈fifo中压入数据data
- int fifo32_get(struct FIFO32 *fifo)
	获取栈中最先压入的数据
- int fifo32_status(struct FIFO32 *fifo)
	查询栈的状态

#### graphic.c
> 图形图像相关的实现
- void init_palette(void)
	初始化调色板,调色板值等信息的初始化
- void set_palette(int start, int end, unsigned char *rgb)
	根据调色板初始化的值，设置调色板
- void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
	绘制矩形，
	(x0, y0):左上角的点
	(x1, y1):右上角的点
	c:颜色，
	vram：为图层的地址
	xsize:图层的宽度
- void init_screen(char *vram, int x, int y)
	绘制屏幕，最下面的那一层
- void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
	显示字体,vram:要显示的层;xszie:图层的宽度;(x,y):显示位置;c:颜色;font:字
- void putfonts8_asc(char *vram, int xsize, int x, int y, char c.; unsigned char *s)
	显示字符串;
	vram:显示文字的图层;xsize:图层的宽度;(x, y):位置;c:颜色;s:字符串
- void init_mouse_cursor8(char *mouse, char bc)
	初始化鼠标指针;mouse:绘制鼠标的图层;bc:背景颜色
- void putblock8_8(char *vram, int vxsize, int pxsize, int pysize,
			int px0, int py0, char *buf, int bxsize);
	在一个图层上绘制另一个图画
	vram:画布;vxsize:画布宽度;(pxsize,pysize):要绘制的图形宽度和高度;
	(px0,py0):绘制图形的起点;buf:要绘制图形的地址;bxsize:每一行的像素数

#### dsctbl.c
> 负责段记录表和中断记录表信息的初始化
-	void init_gdtidt()
	初始化段记录表
- void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
	设定段描述符信息
	sd:段描述符指针;limit:上限; base:基址; ar:权限
- void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
	注册中断函数
	gd:中断号；offset: 函数名; 

#### int.c
> 处理中断相关的事务
- void init_pic(void)
	初始化中断控制器PIC
- void inthandler27(int *esp)
	
#### keyboard.c
> 处理键盘相关的事务
- void inthandler21(int *esp)
	处理键盘中断,当键盘产生中断时，便调用该函数
- void wait_KBC_sendready(void)
	让键盘控制电路KBC做好准备，等待控制指令的到来
- void init_keyboard(struct FIFO32 *fifo, int data0)
	初始化键盘缓冲;
	fifo:要写入数据的键盘缓冲区;data0:键盘发生中断时要写入的数据

#### mouse.c
> 处理鼠标相关的事务
- void inthandler2c(int *esp)
	处理鼠标的中断,当鼠标产生中断后便调用该函数
- void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec)
	初始化鼠标缓冲
	fifo:要写入鼠标数据的缓冲区;data0:鼠标中断产生的数据;medc：鼠标数据的描述符
- void mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
	解析鼠标中断产生的数据;
	mdec:鼠标描述符; dat:鼠标中断产生的数据

#### memory.c
> 处理内存相关的事务
- unsigned int memtest(unsigned int start, unsigned int end)
	测试内存是否可用
- void memman_init(struct MEMMAN *man)
	初始化内存
- unsigned int memman_total(struct MEMMAN *man)
	获取内存容量
- unsigned int memman_alloc(struct MEMMAN *man, unsigned int size)
	获取容量为size的内存
- int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size)
	释放以addr为初始地址，size大小的内存
- unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size)	
	以4K为单位分配内存
- int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size)
	释放以4K为单位分配的内存

#### sheet.c
> 处理图层相关的事务
- struct SHTCTL* shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize)
	初始化图层控制管理器
	memman:内存管理器;vram:屏幕地址;(xsize,ysize):屏幕大小 
- struct SHEET* sheet_alloc(struct SHTCTL *ctl)
	获取一个图层;
	ctl:图成控制器
- void sheet_updown(struct SHEET *sht, int height)
	调整图成的高度(上下高度，即控制图层间的遮挡问题)
	sht:图层;height:高度
- void sheet_setbuf(struct SHEET* sht,unsigned char *buf, int xsize, int ysize, int col_inv)
	设置需要绘制图层的属性
	sht:图层;buf:数据;(xsize,ysize):图层大小;col_inv:透明度
- void sheet_refresh(struct SHEET *sht, int bx0, int by0, int bx1, int by1)
	刷新图层的某一部分
	sht:要刷新的图层;(bx0,by0):刷新部分的左上角;(bx1,by1):刷新部分的右下角
- void sheet_slide(struct SHEET *sht, int vx0, int vy0)
	移动图层
	sht:要移动的图层;(vx0, vy0):新的位置
- void sheet_free(struct SHEET *sht)
	删除图层

#### timer.c
> 定时器相关的设定
- void init_pit(void)
	在硬件上设置时间中断
- struct TIMER* timer_alloc(void)
	获取一个定时器
- void timer_free(struct TIMER *timer)
	删除一个定时器
- void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data)
	定时器的初始化
- void timer_settime(struct TIMER *timer, unsigned int timerout)
	设置定时器的时间
- void inthandler20(int *esp)
	设置定时器产生中断时处理的函数
	
#### mtask.c
> 任务处理相关的函数
- void mt_init(void)
	任务控制器初始化
- void mt_taskswitch(void)
	任务切换
