; naskfunc
; TAB=4

[FORMAT	"WCOFF"] 		;	制作目标文件的模式
[INSTRSET	"i486p"]	; 表明该程序是给486使用的
[BITS	32]						;	制作32位模式用的机械语言

; 制作目标文件的信息
[FILE	"naskfunc.nas"]	; 源文件名信息
	; 程序中包含的函数名
	GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
	GLOBAL	_io_in8, _io_in16, _io_in32
	GLOBAL	_io_out8, _io_out16, _io_out32
	GLOBAL	_io_load_eflags, _io_store_eflags
	GLOBAL	_load_gdtr,	_load_idtr
; 以下是实际的函数
[SECTION .text]			; 目标文件中写了这些之后再写程序

_io_hlt:			; void io_hlt(void);
	HLT
	RET

_io_cli:			; void io_cli(void);
	CLI
	RET

_io_sti:			; void io_sti(void);
	STI
	RET	

_io_stihlt:		;	void io_stihlt(void);
	STI
	HLT
	RET

_io_in8:			; void io_in8(int port);
	mov	EDX,	[ESP+4]
	mov	EAX,	0
	in	AL,	DX
	RET	

_io_in16:			; int	io_in16(int port);
	mov	EDX,	[ESP+4]
	mov	EAX,	0
	in	AX,	DX
	RET

_io_in32:			; int io_in32(int port);
	mov	EDX,	[ESP+4]
	in	EAX,	DX
	RET

_io_out8:			; void io_out8(int port, int data);
	mov	EDX,	[ESP+4]		; port
	mov	AL,	[ESP+8]			;	data
	out	DX,AL
	RET

_io_out16:		; void io_out16(int port,int data);
	mov	EDX,	[ESP+4]
	mov	EAX,	[ESP+8]
	out	DX,AX
	RET

_io_out32:		; void io_out32(int port, int data);
	mov	EDX,	[ESP+4]
	mov	EAX,	[ESP+8]
	out	DX,	EAX
	RET

_io_load_eflags:	; int io_load_eflags(void);
	PUSHFD
	POP	EAX
	RET

_io_store_eflags:	; void io_store_eflags(int eflags);
	mov	EAX,	[ESP+4]
	PUSH	EAX
	POPFD	
	RET

_load_gdtr:				; void	load_gdtr(int limit, int addr);
	mov	AX,	[ESP+4]		; limit
	mov	[ESP+6],AX
	LGDT	[ESP+6]
	RET

_load_idtr:				; void load_idtr(int limit, int addr);
	mov	AX,	[ESP+4]		; limit
	mov	[ESP+6],AX
	LIDT	[ESP+6]
	RET

