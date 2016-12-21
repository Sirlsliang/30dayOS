; naskfunc
; TAB=4

[FORMAT "WCOFF"] 		;	制作目标文件的模式
[INSTRSET "i486p"]	; 表明该程序是给486使用的
[BITS 32]						;	制作32位模式用的机械语言
[FILE "naskfunc.nas"]	; 源文件名信息
	; 程序中包含的函数名
		GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL	_io_load_eflags, _io_store_eflags
		GLOBAL	_load_gdtr, _load_idtr
		GLOBAL	_load_cr0, _store_cr0
		GLOBAL	_load_tr
		GLOBAL	_asm_inthandler20, _asm_inthandler21
		GLOBAL	_asm_inthandler27, _asm_inthandler2c
		GLOBAL	_asm_inthandler0c, _asm_inthandler0d
		GLOBAL	_asm_end_app,_memtest_sub
		GLOBAL	_farjmp, _farcall
		GLOBAL	_asm_hrb_api, _start_app
		EXTERN	_inthandler20, _inthandler21
		EXTERN	_inthandler27, _inthandler2c
		EXTERN	_inthandler0c, _inthandler0d
		EXTERN	_hrb_api

; 以下是实际的函数
[SECTION .text]			; 目标文件中写了这些之后再写程序

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_io_cli:	; void io_cli(void);
		CLI
		RET

_io_sti:	; void io_sti(void);
		STI
		RET

_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]
		MOV		EAX,0
		IN		AL,DX
		RET

_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]
		IN		EAX,DX
		RET

_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		EAX,[ESP+8]
		OUT		DX,AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		EAX,[ESP+8]
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]		; RET时，EAX中的值作为函数的返回值
		PUSH	EAX
		POPFD	
		RET

_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

_load_cr0:		; int load_cr0(void);
		MOV		EAX,CR0
		RET

_store_cr0:		; void store_cr0(int cr0);
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

_load_tr:		; void load_tr(int tr);
		LTR		[ESP+4]
		RET

_asm_inthandler20:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler20
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler0d:
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler0d
		CMP		EAX,0
		JNE		end_app
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		ADD		ESP,4
		IRETD

_asm_inthandler0c:
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler0c
		CMP		EAX,0
		JNE		end_app
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		ADD		ESP,4
		IRETD
		
_memtest_sub:				; unsigned int memtest_sub(unsigned int start, unsigned int end);
		PUSH	EDI					; EBX, ESI, EDI 还要使用
		PUSH	ESI
		PUSH	EBX
		MOV		ESI,0xaa55aa55		; pat0 = 0xaa55aa55
		MOV		EDI,0x55aa55aa		; pat1 = 0x55aa55aa
		MOV		EAX,[ESP+12+4]		; i = start
mts_loop:
		MOV		EBX,EAX
		ADD		EBX,0xffc				; p = i + 0xffc
		MOV		EDX,[EBX]				; old = *p,EBX存放的是地址，[EBX]就是取该地址中存放的数
		MOV		[EBX],ESI				; *p = pat0
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		EDI,[EBX]				; if(*p != pat1) goto fin;
		JNE		mts_fin
		XOR		DWORD [EBX],0xffffffff ; *p ^= 0xffffffff;
		CMP		ESI,[EBX]				; if (*p != pat0) goto fin;
		JNE		mts_fin
		MOV		[EBX],EDX				; *p = old
		ADD		EAX,0x1000			; i += 0x1000
		CMP		EAX,[ESP+12+8]	; if(i<= end) goto mts_loop;
		JBE		mts_loop
		POP		EBX
		POP		ESI
		POP		EDI
		RET
mts_fin:
		MOV		[EBX],EDX				; *p = old
		POP		EBX
		POP		ESI
		POP		EDI
		RET

_farjmp:		; void farjmp(int eip, int cs);
		JMP		FAR	[ESP+4]				; eip, cs
		RET

_farcall:		; void farcall(int eip, int cs);
		CALL	FAR	[ESP+4]			; eip, cs
		RET

_asm_hrb_api:
		STI
; 为方便起见从头就禁止中断请求
		PUSH	DS
		PUSH	ES
		PUSHAD			; 用于保存值
		PUSHAD			; 用于传值
		MOV		AX,SS		
		MOV		DS,AX
		MOV		ES,AX
		CALL	_hrb_api
		CMP		EAX,0
		JNE		end_app
		ADD		ESP,32
		POPAD
		POP		ES
		POP		DS
		IRETD					; 会自动执行STI
end_app:

		MOV		ESP,[EAX]
		POPAD
		RET

_start_app:		; void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
		PUSHAD			; 保存32位的寄存器
		MOV		EAX,[ESP+36]		; 应用程序用EIP
		MOV		ECX,[ESP+40]		; 应用程序用CS
		MOV		EDX,[ESP+44]		; 应用程序用ESP
		MOV		EBX,[ESP+48]		; 应用程序用DS/SS
		MOV		EBP,[ESP+52]		; tss.esp0的地址
		MOV		[EBP  ],ESP				; 保存操作系统用ESP
		MOV		[EBP+4],SS			;	保存操作系统用SS
		MOV		ES,BX
		MOV		DS,BX
		MOV		FS,BX
		MOV		GS,BX
	; 调整栈
		OR		ECX,3						; 将应用程序用段号和3进行OR运算
		OR		EBX,3						; 将应用程序用段号和3进行OR运算
		PUSH	EBX							; 应用程序的SS
		PUSH	EDX							; 应用程序的ESP
		PUSH	ECX							; 应用程序的CS
		PUSH	EAX							; 应用程序的EIP
		RETF

_asm_end_app:
		MOV		ESP,[EAX]
		MOV		DWORD [EAX+4],0
		POPAD
		RET
