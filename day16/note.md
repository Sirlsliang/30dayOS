## Days16

#### 关于内存的分配
- memory.c文件中利用函数分配出来的内存是从低地址到高地址的
- 在day15中，为新的任务指配参数时是从段的最高地址开始指配的。也就是说函数的运行栈从段的最高地址向最
低地址，而需要分配的内存则相反。第一个段的大小等信息

#### 优先级
- 优先级越高运行的时间越长，这样就使得运行的速度越快
- 利用不同的数据结构，将任务分成不同的运行级别。优先在运行级别比较高的任
务中切换，然后再切换到低级的运行级别中
- 将任务划分为10个运行级别,每个运行级别中都包含当前的运行的任务以及该级别
下正在运行的任务

#### 复习
- 操作系统利用任务状态段(TSS)来保存寄存器中的各个值。
- 任务切换时如果一条JMP指令所指定的目标地址段不是可执行的代码，而是TSS的话，CPU就不会执行通常的改写
EIP和CS的操作，而是将这条指令理解为任务切换
- sel(selector:选择器)里面保存的是每个任务的GDT编号,即段编号
- load_tr() 和farjmp()都只需要传入段的编号即sel即可
 
