/* 引用函数,这里声明了一个函数却没有用到{}，就是表明函数在别的文件中
 * ，需要编译器自动寻找*/
void io_hlt(void);	
void HariMain(void){
	fin:
		io_hlt();
		goto fin;
}
