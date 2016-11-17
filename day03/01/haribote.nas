;haribote-os
;TAB = 4

; 有关BOOT_INFO
	ORG 0xc400				; 程序装载到内存的位置
	
	mov	AL,0x13			;VGA 显卡，320x200x8 位彩色
	mov	AH,0x00
	INT 0x10

fin:
	HTL
	JMP fin
