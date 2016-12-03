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
	GLOBAL	_asm_inthandler21, _asm_inthandler27, _asm_inthandler2c
	GLOBAL	_load_cr0, _store_cr0 
	GLOBAL	_memtest_sub
	EXTERN	_inthandler21, _inthandler27, _inthandler2c

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

_asm_inthandler21:
	PUSH	ES
	PUSH	DS
	PUSHAD
	mov	EAX, ESP
	PUSH	EAX
	mov	AX,	SS
	mov	DS,	AX
	mov	ES,	AX
	call	_inthandler21
	POP	EAX
	POPad
	POP	DS
	POP	ES
	IRETD

_asm_inthandler27:
	PUSH ES
	PUSH DS
	PUSHAD
	mov	EAX, ESP
	PUSH	EAX
	mov	AX,	SS
	mov	DS,	AX
	mov	ES,	AX
	call _inthandler27
	POP EAX
	POPAD
	POP	DS
	POP	ES
	IRETD

_asm_inthandler2c:
	PUSH	ES
	PUSH	DS
	PUSHAD
	mov	EAX,	ESP
	PUSH	EAX
	mov	AX,	SS
	mov	DS,	AX
	mov	ES,	AX
	call	_inthandler2c
	POP	EAX
	POPAD
	POP	DS
	POP	ES
	IRETD

_load_cr0:					; int load_cr0(void);
	mov	EAX, CR0
	RET

_store_cr0:					; void store_cr0(int cr0);
	mov	EAX,	[ESP+4]
	mov	CR0,	EAX
	RET

_memtest_sub:				; unsigned int memtest_sub(unsigned int start, unsigned int end);
	push EDI					; EBX, ESI, EDI 还要使用
	push ESI
	push EBX
	mov	ESI, 0xaa55aa55		; pat0 = 0xaa55aa55
	mov	EDI, 0x55aa55aa		; pat1 = 0x55aa55aa
	mov	EAX, [ESP+12+4]		; i = start
mts_loop:
	mov	EBX,	EAX
	add	EBX,	0xffc				; p = i + 0xffc
	mov	EDX,	[EBX]				; old = *p,EBX存放的是地址，[EBX]就是取该地址中存放的数
	mov	[EBX], ESI				; *p = pat0
	xor	DWORD [EBX], 0xffffffff	; *p ^= 0xffffffff;
	cmp EDI,	[EBX]				; if(*p != pat1) goto fin;
	jne	mts_fin
	xor	DWORD [EBX], 0xffffffff ; *p ^= 0xffffffff;
	cmp ESI,	[EBX]				; if (*p != pat0) goto fin;
	jne	mts_fin
	mov	[EBX], EDX				; *p = old
	add	EAX,	0x1000			; i += 0x1000
	cmp EAX,	[ESP+12+8]	; if(i<= end) goto mts_loop;

	jbe	mts_loop
	pop	EBX
	pop	ESI
	pop	EDI
	RET
mts_fin:
	mov	[EBX], EDX				; *p = old
	pop	EBX
	pop ESI
	pop EDI
	RET
	

