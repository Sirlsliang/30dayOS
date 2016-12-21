[FORMAT "WCOFF"]		;	生成对象文件的格式
[INSTRSET "i486p"]	;	表示使用486兼容指令集
[BITS 32]						; 生成32位模式机器语言
[FILE "api010.nas"]	; 源文件名信息

		GLOBAL	_api_point

[SECTION .text]

_api_point:			; void api_point(int win, int x, int y, int col);
		PUSH	EDI
		PUSH	ESI
		PUSH	EBX
		MOV		EDX,11
		MOV		EBX,[ESP+16]		; win
		MOV		ESI,[ESP+20]		; x
		MOV		EDI,[ESP+24]		; y
		MOV		EAX,[ESP+28]		; col
		INT		0x40
		POP		EBX
		RET

