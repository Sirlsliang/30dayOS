;haribote-os
;TAB = 4

BOTPAK	EQU	0x00280000
DSKCAC	EQU	0x00100000
DSKCAC0	EQU	0x00008000
; 有关BOOT_INFO
CYLS	EQU	0x0ff0		;	设定启动区
LEDS	EQU	0x0ff1
VMODE	EQU	0x0ff2		;	关于颜色数目的信息，颜色的位数
SCRNX	EQU	0x0ff4		;	分辨率的X
SCRNY	EQU	0x0ff6		; 分辨率的Y
VRAM	EQU	0x0ff8		;	图像缓冲区的开始地址

	ORG 0xc400				; 程序装载到内存的位置
	
	MOV	AL,0x13			;VGA 显卡，320x200x8 位彩色
	MOV	AH,0x00
	INT 0x10
	MOV	BYTE [VMODE],	8	; 记录画面模式
	MOV	WORD [SCRNX], 320;
	MOV	WORD [SCRNY], 200;
	MOV	DWORD [VRAM], 0x000a0000

; 用BIOS取得键盘上各种欧冠你LED指示灯的状态
	MOV	AH, 0x02
	INT	0x16						; KeyBoard BIOS
	MOV [LEDS], AL
;PCI 关闭一切中断
;根据AT兼容机的规格，如果要初始化PIC必须在CLI之前进行
;否则有时会挂起，随后进行PIC的初始化

	MOV	AL,	0xff
	OUT	0x21,	AL
	NOP									;如果连续执行OUT指令，有些机种会无法正常运行
	OUT	0xa1,	AL
	CLI 								;禁止CPU级别的中断

;为了让CPU能够访问1MB以上的内存空间，设定A20GATE
	CALL waitkbdOUT
	MOV	AL,	0xd1
	OUT	0x64,	AL
	CALL waitkbdOUT
	MOV	AL,	0xdf
	OUT	0x60,	AL
	CALL waitkbdOUT

;切换到保护模式
;[INSTRSET "i486p"]
	LGDT	[GDTR0]										;设定临时GDT
	MOV	EAX,	CR0
	AND	EAX,	0x7fffffff						;	设bit31为0(为了禁止颁)
	OR	EAX,	0x00000001						;	设bit0为1(为了切换到保护模式)
	MOV	CR0,	EAX
	JMP	pipelINeflush

pipelINeflush:
	MOV	AX,	1*8											;	可读写的段 32bit
	MOV	DS,	AX
	MOV	ES,	AX
	MOV	FS,	AX
	MOV	GS,	AX
	MOV	SS,	AX


;	bootpack的转送
	MOV	ESI,	bootpack
	MOV	EDI,	BOTPAK
	MOV	ECX,	512*1024/4
	CALL	memcpy

;	磁盘数据最终转送到它本来的位置去
;	首先从启动扇区开始
	MOV	ESI,	0x7c00										;	转送源
	MOV	EDI,	DSKCAC										;	转送目的地
	MOV	ECX,	512/4
	CALL	memcpy

;	所有剩下的
	MOV	ESI,	DSKCAC0+512								;	转送源
	MOV	EDI,	DSKCAC+512								;	转送目的地
	MOV	ECX,	0
	MOV	CL,		BYTE	[CYLS]
	IMUL	ECX,	512*18*2/4							;	从柱面数变为字节数/4
	SUB	ECX,	512/4											;	减去	IPL
	CALL	memcpy

;	必须由asmhead来完成的工作，至此全部完毕，以后就交给bootpack来完成

;	bootpack的启动
	MOV	EBX,	BOTPAK
	MOV	ECX,	[EBX+16]
	ADD	ECX,	3								; ECX +=3
	SHR	ECX,	2								; ECX	/=2
	JZ	skip
	MOV	ESI,	[EBX+20]									;	转送源
	ADD	ESI,	EBX
	MOV	EDI,	[EBX+12]									;	转送目的地
	CALL	memcpy

skip:
	MOV	ESP,	[EBX+12]
	jmp	DWORD 2*8:0x0000001b

waitkbdOUT
	IN	AL,	0x64
	AND	AL,	0x02
	JNZ	waitkbdOUT												;	AND 的结果不是0,就跳转到waitkbdOUT
	RET

memcpy:
	MOV	EAX,	[ESI]
	ADD	ESI,	4
	MOV	[EDI],	EAX
	ADD	EDI,	4
	sub	ECX,	1
	JNZ	memcpy														;	减法运算的结果如果不是0,就跳转到memcpy
	RET
	
	ALIGNB	16

GDT0:
	RESB	8																; NULL selector
	DW	0xffff,	0x0000,	0x9200,	0x00cf		;	可以读写的段(segment) 32bit
	DW	0xffff,	0x0000,	0x9a28,	0x0047		;	可以执行的段(segment) 32bit(bootpack 用)
	
	DW	0

GDTR0:
	DW	8*3-1
	DD	GDT0
	ALIGNB	16

bootpack:
