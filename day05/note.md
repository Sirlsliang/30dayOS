## Day05

#### 结构体
- 声明 : 
			struct XXXX{ char xx; };
- 使用 : 
			XXXX xxx

#### 指向结构体的指针
- 指针变量：地址用4个字节表示

#### 使用外部数据
- 关键字:extern
- A： 十进制：65;16进制

#### 在屏幕中画图
- 假设屏幕左上角的像素点地址为：0xa000
- 屏幕的像素为（xSize，ySize）
- 屏幕上点（x，y) 的地址= 0xa000 +(y*xSize)+x
- 因此在画字体时，只要确定了左上角的位置（x，y),便可以确定每个像素的位置，只要再
为像素赋值即可

#### 箭头标记方式
- 省略地址中的括号，直接使用箭头
- xsize = (*binfo).scrnx == xsize = binfo->scrnx

#### 错误
- two or more data types in declaration : 定义结构体时,没有加分号(重复定义)

#### 字符串
- 字符串按顺序排列在内存当中，末尾加上 0x00组成的字符编码。
- 可以利用char* s = “xxx”;的形式来指向字符串，s 指向字符串的头地址，*s 取出字符
串的值

#### 字符串输出
- sprinf(addr，format,value) : 将输出内容作为字符串写入内存中,将value按照format格式，写入addr中
- sprinf(addr，format,value);
	- addr: 字符串存放的地址
	- format: 指定了数值以什么方式变换为字符串
		- %d: 十进制数
		- %5d: 5位十进制数,不够5位数，则前面加空格
		- %05d: 5位十进制数，不够5位，前面补0
		- %x	: 16进制数

#### 段
- 将内存分成许多块，每一块的起始地址都看作0来处理，像这样分割出来的块，就称为段
- 表示一个段需要的信息：段的大小、段的起始地址、段的管理属性（禁止写入、禁止执行
、系统专用等）需要8个字节即64位来表示这些信息,但是段寄存器只有16位,解决办法就是提前设定段号与段的关系。段寄存器有16位，但是低3位不可使用，因此能够
处理0~8191个区域，即有8192个段。8192 * 8 = 65536字节 = 64KB。这64KB的数据称为
GDT("global (segment) descriptor table")，即全局段号记录表。将这些数据整齐地排列
在内存的某个地方，然后将内存的起始地址和有效设定个数放在CPU内被称作GDTR的特殊寄
存器中，设定就完成了。

#### 中断
- IDT("Interrupt descriptor table"): 中断记录表,CPU遇到外部状况变化，或者内部偶
然发生某些错误时，会临时切换过去处理这种突发事件。这就是中断。
- 举例： 键盘要输入文字，使用查询的方法会导致CPU资源消耗太大，无法应付。利用中断
，每个设备有变化时就产生中断，发生中断后,CPU暂停正在处理的任务，并做好接下来能够
继续处理的准备，转而执行中断程序。中断程序执行完毕以后，再掉用事先设定好的函数，
返回处理中的任务。
- IDT记录了0~255的中断号码与调用函数的对应关系，设定方法同GDT类似。

#### 设置段描述符
- set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
- set_segmdesc(gdt+1,0xffffffff,0x00000000,0x4092):段号为1的段,表示CPU能够管理的整个内存,0x4092
	的含义参看day06
- set_segmdesc(gdt+1,0x007fffff,0x00280000,0x409a):段号为2的段，大小为512KB,为bootpacl.hrb准备的



