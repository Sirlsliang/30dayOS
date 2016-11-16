; hello-os
; TAB=4

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
  mov SP,0x7c00
  mov DS,AX
  mov ES,AX

  mov SI,msg

putloop:
  mov AL,[SI]
  add SI,1
  cmp AL,0

  je  fin
  mov AH,0x0e ;显示一个文字
  mov BL,0   ;指定字符颜色
  INT 0x10    ;调用显卡BIOS
  jmp putloop

fin:
  HLT         ;让CPU停止，等待指令
  jmp fin

msg:              ;0x7c74
  DB  0x0a, 0x0a  ;换行两次
  db  "HELLO, world"
  db  0x0a        ;换行
  db  0

  times 510-($-$$) db 0
  db  0x55,0xaa

;一下是启动区以外部分的输出
  db  0xf0,0xff,0xff,0x00,0x00,0x00,0x00,0x00
  resb  4600
  db  0xf0,0xff,0xff,0x00,0x00,0x00,0x00,0x00
  resb  1469432
