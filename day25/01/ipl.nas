; hello-os
; TAB=4

CYLS EQU	10		;声明常数,EQU 相当与C中的#define

  ORG 0x7c00  ;指明程序的装载地址

; 以下的记述用于标准FAT12格式的软盘

  JMP entry
  db  0x90
  db  "HARIBOTE"
  dw  512
  db  1
  dw  1
  db  2
  dw  224
  dw  2880
  db  0xf0
  dw  9
  dw  18 
  dw  2
  dd  0
  dd  2880
  db  0,0,0x29
  dd  0xffffffff
  db  "HARIBOTEOS "
  db  "FAT32   "
  resb  18

;程序核心
entry:      ;0x7c50
  mov AX,0  ; 初始化寄存器
  mov SS,AX
  mov DS,AX
	mov ES,AX

  mov SP,0x7c00

; 读盘时的参数
	mov	AX,0x0820
	mov	ES,AX
	mov	CH,0				;	柱面 0
	mov	DH,0				;	磁头 0
	mov	CL,2				;	扇区 2

readloop:
;试错
	mov SI,0				;	记录失败次数的寄存器

retry:
; 读盘指令
	mov	AH,0x02			; 读盘指令
	mov	AL,1				;	1个扇区
	mov	BX,0
	mov	DL,0x00			;	A驱动器
	INT	0x13				; 调用磁盘BIOS
	JNC	next				; Jump if not carry,进位符为0,即没有出错,跳转
	ADD	SI,1
	CMP SI,5
	JAE	error				; JAE : Jump if above or equal 

; 复位磁盘状态
	mov AH,0x00
	mov	DL,0x00			; A 驱动器
	INT	0x13
	JMP retry

next:
	mov	AX,ES				;	把内存地址后移0x20,读取下一个扇区，需要ES+0x20 ,CL +1
	add AX,0x0020
	mov	ES,AX				; 没有Add ES,0x20指令，需要借助AX实现
	add	CL,1				; 向CL中+1
	CMP	CL,18				; CL 扇区
	JBE	readloop		; CL<=18 跳转至readloop
	mov	CL,1
	add DH,1
	cmp	DH,2				;读取磁头2
	JB	readloop		; 如果<2 ，则跳转至readloop
	mov	DH,0				; 读取新的柱面了
	add	CH,1				; CH 柱面
	cmp	CH,CYLS
	JB	readloop		; 如果CH < CYLS，则跳转到readloop

	mov [0x0ff0],CH	; 将CYLS的值写入到0x0ff0这个地址,4080

;将程序跳到系统文件的开始出，具体的查看笔记文件
	JMP	0xc400



error:
  mov SI,msg

putloop:
  mov AL,[SI]
  add SI,1
  cmp AL,0

  je  fin
  mov AH,0x0e ;显示一个文字
  mov BX,15   ;指定字符颜色
  INT 0x10    ;调用显卡BIOS
  jmp putloop

fin:
  HLT         ;让CPU停止，等待指令
  jmp fin

msg:              ;0x7c74
  DB  0x0a, 0x0a  ;换行两次
  db  "load error"
  db  0x0a        ;换行
  db  0

  times 510-($-$$) db 0
  db  0x55,0xaa
