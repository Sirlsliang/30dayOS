[FORMAT "WCOFF"]		;	生成对象文件的格式
[INSTRSET "i486p"]	;	表示使用486兼容指令集
[BITS 32]						; 生成32位模式机器语言
[FILE "api008.nas"]	; 源文件名信息

		GLOBAL	 _api_initmalloc

[SECTION .text]

_api_initmalloc:	; void api_initmalloc(void);
		PUSH	EBX
		MOV		EDX,8
		MOV		EBX,[CS:0x0020]	
		MOV		EAX,EBX
		MOV		EAX,32*1024
		MOV		ECX,[CS:0x0000]
		MOV		ECX,EAX
		INT		0x40
		POP		EBX
		RET

