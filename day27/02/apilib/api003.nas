[FORMAT "WCOFF"]		;	生成对象文件的格式
[INSTRSET "i486p"]	;	表示使用486兼容指令集
[BITS 32]						; 生成32位模式机器语言
[FILE "api003.nas"]	; 源文件名信息


		GLOBAL	_api_putstr1
	
[SECTION .text]

_api_putstr1: 	; void api_putstr1(char *s, int l);
		PUSH 	EBX
		MOV		EDX,3
		MOV		EBX,[ESP+8]
		MOV		ECX,[ESP+12]
		INT		0x40
		POP		EBX
		RET
