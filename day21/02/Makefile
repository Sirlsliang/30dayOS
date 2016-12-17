OBJS_BOOTPACK	= bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj int.obj fifo.obj \
								keyboard.obj mouse.obj memory.obj sheet.obj timer.obj mtask.obj window.obj	\
								console.obj	file.obj
# 添加默认命令
TOOL_PATH := ../../z_tools/
INCPATH		:=	$(TOOL_PATH)haribote/

CC1				:= wine	$(TOOL_PATH)cc1.exe -I $(INCPATH) -Os -Wall -quiet
GAS2NASK	:= wine	$(TOOL_PATH)gas2nask.exe -a

NASK			:= wine $(TOOL_PATH)nask.exe
OBJ2BIM		:= wine	$(TOOL_PATH)obj2bim.exe
RULEFILE	:= $(TOOL_PATH)haribote/haribote.rul
BIM2HRB		:= wine	$(TOOL_PATH)bim2hrb.exe
MAKEFONT	:= wine	$(TOOL_PATH)makefont.exe
BIN2OBJ		:= wine	$(TOOL_PATH)bin2obj.exe

default :
	make img

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst
#	nasm -o ipl.bin -l ipl.lst ipl.nas

asmhead.bin : asmhead.nas Makefile
	nasm -o asmhead.bin -l asmhead.lst asmhead.nas
#	$(NASK) asmhead.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > haribote.sys

hello.hrb : hello.nas Makefile
	$(NASK) hello.nas hello.hrb hello.lst

hello2.hrb : hello2.nas Makefile
	$(NASK) hello2.nas hello2.hrb hello2.lst

a.bim : a.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:a.bim map:a.map a.obj a_nask.obj

a.hrb : a.bim Makefile
	$(BIM2HRB) a.bim a.hrb 0

hello3.bim : hello3.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello3.bim map:hello3.map hello3.obj a_nask.obj

hello3.hrb : hello3.bim Makefile
	$(BIM2HRB) hello3.bim hello3.hrb 0

crack1.bim : crack1.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:crack1.bim map:crack1.map crack1.obj a_nask.obj

crack1.hrb : crack1.bim Makefile
	$(BIM2HRB) crack1.bim crack1.hrb 0


crack2.hrb : crack2.nas Makefile
	$(NASK) crack2.nas crack2.hrb crack2.lst

haribote.img : ipl.bin haribote.sys Makefile \
		hello.hrb hello2.hrb a.hrb hello3.hrb crack1.hrb crack2.hrb
	dd if=ipl.bin of=haribote.img bs=1 
	dd if=/dev/zero of=haribote.img bs=512 seek=1 count=2879
	- sudo umount /mnt
	sudo mount -o loop haribote.img /mnt
	sudo cp  haribote.sys	/mnt
	sudo cp  ipl.nas	/mnt
	sudo cp  asmhead.nas	/mnt
	sudo cp  hello.hrb	/mnt
	sudo cp  hello2.hrb /mnt
	sudo cp  a.hrb /mnt
	sudo cp  hello3.hrb /mnt
	sudo cp  crack1.hrb /mnt
	sudo cp  crack2.hrb /mnt
	sudo umount /mnt

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas	:	%.gas	Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj :	%.nas	Makefile
	$(NASK)	$*.nas $*.obj	$*.lst

img :
	make -r haribote.img

run :	haribote.img
	qemu-system-i386 -fda haribote.img

clean	: 
	-rm *.bin
	-rm *.lst
	-rm *.obj
	-rm *.map
	-rm *.bim
	-rm *.hrb
	-rm haribote.sys

src :
	make clean
	-rm -f *.img
