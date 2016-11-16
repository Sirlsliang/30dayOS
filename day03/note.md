## Day03
> 这一次实现了对磁盘文件的读取，将磁盘文件中各个扇区的内容读入到内存中，

#### 汇编语言
		mov	CH,0
		mov	DH,0
		mov	CL,2
		mov AH,0x02
		mov	AL,1
		mov	BX,0
		mov	DL,0x00
		INT	0x13

- JC : Jump if carry 如果进位标志为1 的话，就跳转,函数调用出错，进位标志1,函数调用无错，进位标志为0
- INT 0x13 : 调用BIOS的0x13号函数，根据AH中的值对磁盘进行操作
	- AH = 0X02	: 读盘
  - AH = 0x03	: 写盘
  - AH = 0x04 : 校验
  - AH = 0x0c : 寻道
  - AL = 处理对象的扇区数，
  - CH = 柱面号
  - CL = 扇区号
  - DH = 磁头号
  - DL = 驱动器号
  - ES:BX 缓冲地址，
  	- 内存地址
  	- 表示从内存中读取的数据要装载到软盘的位置，计算机开发初期（没有EBX的时期）利用这两个寄存器来增加内存空间，
  	- [ES:BX] = ES \* 16 + BX
  	- 程序如果不指定段寄存器，默认使用DS mov CX,[123] == mov CX,[DS:1234],
  	- ** 每次使用需要将 DS置为 0 **

  - FLAGS.CF == 0 没有错误
  - FLAGS.CF == 1 有错误，错误存在AH中

#### DD 函数
		防止后面的dd 数据会覆盖前面的光盘数据，利用seek 指定开始的块