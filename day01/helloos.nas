;HELLO-OS
;TAB=4

;以下这段是标准FAT12格式软盘专用的代码
  DB  0xeb, 0x4e, 0x90
  db  "HELLOIPL"    ;启动区的名称（可以是任意的字符串，8字节）
  dw  512           ;每个扇区（sector）的大小，必须为512字节
  db  1             ;簇的大小（必须为1个扇区）
  dw  1             ;FAT的起始位置（一般从第一个扇区开始
  db  2             ;FAT的个数，必须为2
  dw  224           ;根目录的大小（一般设为224项）
  dw  2880          ;该磁盘的大小（必须是2880扇区）
  db  0xf0          ;磁盘的种类（必须是0xf0）
  dw  9             ;FAT的长度(必须是9个扇区)
  dw  18            ;1个磁道有几个扇区
  dw  2             ;磁头数（必须是2）
  dd  0             ;不使用分区，必须是0
  dd  2880          ;重写一次磁盘大小
  db  0,0,0x29      ;意义不明，固定
  dd  0xffffffff    ;？
  db  "HELLO-OS123" ;磁盘的名称,11字节
  db  "FAT-3212"    ;磁盘格式名称,8字节
  resb  18          ;空出18字节

;程序主体
  db  0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
  db  0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
  db  0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
  db  0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
  db  0xee, 0xf4, 0xeb, 0xfd

;信息显示部分
  db  0x0a, 0x0a    ;两个换行
  db  "hello, world"
  db  0x0a          ;换行
  db  0
  
  times 510-($-$$) db 0         ;resb  378
  db  0x55,0xaa     ;最后两个字节(即511,512)

;以下是启动区以外部分的输出
  db  0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
  resb  4600
  db  0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
  resb  1469432















