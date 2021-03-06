## Day09
	
#### 内存容量检查
- BIOS开机时会对内存容量进行检查，但是不同的BIOS有不同的调用方法。
- CPU内部有高速缓存器，每次访问内存，都要将所访问的地址和内容存入到高速缓存里。往内存中写入时也一样
，先更新高速缓存中的信息，然后再写入内存。
- 内存检查时向内存中写入一个值，然后马上再将该值读取出来，检查读取的值和写入的值是否相等。如果连接正
常，则写入的值能够记在内存中，没连接上，读出的值则会出现异常。但是缓存的存在可能会影响写入和读出，因
此需要先关闭缓存。
- 内存检查时，如果在某个环节没有恢复成预想的值，就在那个环节终止，并报告终止时的地址，break可以打断
for循环即直接跳出for循环。

#### qemu
- 默认内存大小为128M

#### 内存管理
- 负责内存分配和内存释放
- 以4KB = 4096B为一个单位管理内存
- 内存分配管理的思路
	- 创建一个char[]来映射分配的内存，被分配了就为1,未分配为0。
	- 不用char[]而是利用Bit位来对内存中的分配进行映射。
	- 创建一个MEMMAN管理列表，即负责管理内存的数据结构
			//内存分配的数据结构：
			struct FREEINFO{
				unsigned int addr;  // 分配的起始地址
				unsigned int size;	// 分配的大小
			};
			// 内存管理
			strcut MEMMAN{
				int frees;	// 内存分配的信息
				struct FREEINFO free[1000]; // 分配的内存
			};
			//系统进行初始化时就把空闲内存分配初始化，这样就能保证内存管理中至少有一个空
			//闲内存
		- 内存回收时的思路:
			1. 确定需要回收的内存在空闲内存中的位置
			2. 内存回收的情景判定：
				- 可以与前面空余的内存相连,然后再判定可不可以与后面的内存相连
				- 可以与后面空余的内存相连,
				- 既不能与前面的相连，也不能与后面的相连。这样会增加空余内存的管理数目
