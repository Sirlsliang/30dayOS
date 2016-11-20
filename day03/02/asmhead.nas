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
	
	mov	AL,0x13			;VGA 显卡，320x200x8 位彩色
	mov	AH,0x00
	INT 0x10
	mov	BYTE [VMODE],	8	; 记录画面模式
	mov	WORD [SCRNX], 320;
	mov	WORD [SCRNY], 200;
	mov	DWORD [VRAM], 0x000a0000

; 用BIOS取得键盘上各种欧冠你LED指示灯的状态
	mov	AH, 0x02
	INT	0x16						; KeyBoard BIOS
	mov [LEDS], AL
;PCI 关闭一切中断
;根据AT兼容机的规格，如果要初始化PIC必须在CLI之前进行
;否则有时会挂起，随后进行PIC的初始化
	mov	AL,	0xff
	out	0x21,	AL
	NOP									;如果连续执行out指令，有些机种会无法正常运行
	out	0xa1,	AL
	CLI 								;禁止CPU级别的中断

;为了让CPU能够访问1MB以上的内存空间，设定A20GATE
	call waitkbdout
	mov	AL,	0xd1
	out	0x64,	AL
	call waitkbdout
	mov	AL,	0xdf
	out	0x60,	AL
	call waitkbdout

;切换到保护模式
;[INSTRSET "i4860"]
	LGDT	[GDTR0]										;设定临时GDT
	mov	EAX,	CR0
	and	EAX,	0x7fffffff						;	设bit31为0(为了禁止颁)
	or	EAX,	0x00000001						;	设bit0为1(为了切换到保护模式)
	mov	CR0,	EAX
	jmp	pipelineflush

pipelineflush:
	mov	AX,	1*8											;	可读写的段 32bit
	mov	DS,	AX
	mov	ES,	AX
	mov	FS,	AX
	mov	GS,	AX
	mov	SS,	AX


;	bootpack的转送
	mov	ESI,	bootpack
	mov	EDI,	BOTPAK
	mov	ECX,	512*1024/4
	call	memcpy

;	磁盘数据最终转送到它本来的位置去
;	首先从启动扇区开始
	mov	ESI,	0x7c00										;	转送源
	mov	EDI,	DSKCAC										;	转送目的地
	mov	ECX,	512/4
	call	memcpy

;	所有剩下的
	mov	ESI,	DSKCAC0+512								;	转送源
	mov	EDI,	DSKCAC+512								;	转送目的地
	mov	ECX,	0
	mov	CL,		BYTE	[CYLS]
	imul	ECX,	512*18*2/4							;	从柱面数变为字节数/4
	sub	ECX,	512/4											;	减去	IPL
	call	memcpy

;	必须由asmhead来完成的工作，至此全部完毕，以后就交给bootpack来完成

;	bootpack的启动
	mov	EBX,	BOTPAK
	mov	ECX,	[EBX+16]
	add	ECX,	3								; ECX +=3
	SHR	ECX,	2								; ECX	/=2
	jz	skip
	mov	ESI,	[EBX+20]									;	转送源
	add	ESI,	EBX
	mov	EDI,	[EBX+12]									;	转送目的地
	call	memcpy

skip:
	mov	ESP,	[EBX+12]
	jmp	DWORD 2*8:0x0000001b

waitkbdout
	in	AL,	0x64
	and	AL,	0x02
	jnz	waitkbdout												;	AND 的结果不是0,就跳转到waitkbdout
	RET

memcpy:
	mov	EAX,	[ESI]
	add	ESI,	4
	mov	[EDI],	EAX
	add	EDI,	4
	sub	ECX,	1
	jnz	memcpy														;	减法运算的结果如果不是0,就跳转到memcpy
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
	

