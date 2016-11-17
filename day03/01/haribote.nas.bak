;haribote-os
;TAB = 4

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

fin:
	HTL
	JMP fin
