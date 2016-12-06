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
- void asm_inthandler21(void)
	处理键盘中断信息
- void asm_inthandler27(void)
- void asm_inthandler2c(void)
	处理鼠标中断信息

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

#### fifo.c
> 先入先出栈的实现
- struct 
- void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf)
	栈的初始化
	fifo:要初始化的结构体
