## Day23

#### 内存分配
- 应用程序可以进行读写的只是操作系统为它准备好的数据段中的内存空间

#### 增长方向
- 数据段，堆这个区域是动态分配给内存的，也就是用malloc等函数分配的内存就是在该区域，地址是向上增长的
- 堆栈段，存放局部变量和函数参数等数据


#### bug
- 修复了文件中与源码不一致产生的效果不一样的bug