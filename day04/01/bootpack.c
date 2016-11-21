/* 引用函数,这里声明了一个函数却没有用到{}，就是表明函数在别的文件中
 * ，需要编译器自动寻找*/
void io_hlt(void);	
void write_mem8(int addr, int data);
void HariMain(void){
	int i;
	for(i = 0xa000; i<= 0xaffff; i++){
		write_mem8(i, i & 0x0f); 
	}
	for(;;){
		io_hlt();
	}
}
