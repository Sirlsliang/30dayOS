## Days22

####	指令保护
- 以应用程序模式运行时，IN指令和OUT指令会产生一般保护异常
- 以应用程序模式运行时，执行CLI、STI和HLT这些指令都会产生异常,即应用程序不能控制中断
- 除了设置好的地址以外，应用程序禁止CALL其他的地址,应用程序只能调用系统的API，假
如操作系统API存在问题的话，那么则会出现问题
- CPU自动检测数据的范围，如果出现异常会产生相应的中断


#### 中断号与异常
- CPU中0x00到0x1f都是异常所使用的中断，IRQ的中断号都是从0x20开始的(0x00:除零异常;0x06:非法指令异常)

#### 函数调用
-  函数运行时数据栈是从高地址向低地址运行的，ebp指向栈底，esp指向栈顶。而且函数的参数是被反向
压栈中的。以下面的函数作出分析

		_api_openwin:				; int api_openwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
			PUSH	EDI
			PUSH	ESI
			PUSH	EBX
			MOV		EDX,5
			MOV		EBX,[ESP+16]	; buf
			MOV		ESI,[ESP+20]	; xsiz
			MOV		EDI,[ESP+24]	; ysiz
			MOV		EAX,[ESP+28]	; col_inv
			MOV		ECX,[ESP+32]	; title
			INT		0x40				
			POP		EBX
			POP		ESI
			POP		EDI
			RET
	函数栈中的数据组合样式为
			ESP			 
			EBX			
			ESI			
			EDI			
			buf			
			xsiz		
			ysiz		
			col_inv	
			title		
			EBP---> 上一个函数的栈底
	





