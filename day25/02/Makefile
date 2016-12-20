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

hello5.hrb : hello5.nas Makefile
	$(NASK) hello5.nas hello5.hrb hello5.lst


a.bim : a.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:a.bim map:a.map a.obj a_nask.obj

a.hrb : a.bim Makefile
	$(BIM2HRB) a.bim a.hrb 0

hello3.bim : hello3.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello3.bim map:hello3.map hello3.obj a_nask.obj

hello3.hrb : hello3.bim Makefile
	$(BIM2HRB) hello3.bim hello3.hrb 0

hello4.bim : hello4.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello4.bim map:hello4.map hello4.obj a_nask.obj

hello4.hrb : hello4.bim Makefile
	$(BIM2HRB) hello4.bim hello4.hrb 0

bug1.bim : bug1.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bug1.bim map:bug1.map bug1.obj a_nask.obj

bug1.hrb : bug1.bim Makefile
	$(BIM2HRB) bug1.bim bug1.hrb 0

bug2.bim : bug2.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bug2.bim map:bug2.map bug2.obj a_nask.obj

bug2.hrb : bug2.bim Makefile
	$(BIM2HRB) bug2.bim bug2.hrb 0

bug3.bim : bug3.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bug3.bim map:bug3.map bug3.obj a_nask.obj

bug3.hrb : bug3.bim Makefile
	$(BIM2HRB) bug3.bim bug3.hrb 0

winhelo.bim : winhelo.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo.bim map:winhelo.map winhelo.obj a_nask.obj

winhelo.hrb : winhelo.bim Makefile
	$(BIM2HRB) winhelo.bim winhelo.hrb 0

winhelo2.bim : winhelo2.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo2.bim map:winhelo2.map winhelo2.obj a_nask.obj

winhelo2.hrb : winhelo2.bim Makefile
	$(BIM2HRB) winhelo2.bim winhelo2.hrb 0

winhelo3.bim : winhelo3.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo3.bim map:winhelo3.map winhelo3.obj a_nask.obj

winhelo3.hrb : winhelo3.bim Makefile
	$(BIM2HRB) winhelo3.bim winhelo3.hrb 40K

star1.bim : star1.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:star1.bim map:star1.map star1.obj a_nask.obj

star1.hrb : star1.bim Makefile
	$(BIM2HRB) star1.bim star1.hrb 47K

stars.bim : stars.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:stars.bim map:stars.map stars.obj a_nask.obj

stars.hrb : stars.bim Makefile
	$(BIM2HRB) stars.bim stars.hrb 47K

stars2.bim : stars2.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:stars2.bim map:stars2.map stars2.obj a_nask.obj

stars2.hrb : stars2.bim Makefile
	$(BIM2HRB) stars2.bim stars2.hrb 47K

lines.bim : lines.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:lines.bim map:lines.map lines.obj a_nask.obj

lines.hrb : lines.bim Makefile
	$(BIM2HRB) lines.bim lines.hrb 48K

walk.bim : walk.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:walk.bim map:walk.map walk.obj a_nask.obj

walk.hrb : walk.bim Makefile
	$(BIM2HRB) walk.bim walk.hrb 48K

noodle.bim : noodle.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:noodle.bim map:noodle.map noodle.obj a_nask.obj

noodle.hrb : noodle.bim Makefile
	$(BIM2HRB) noodle.bim noodle.hrb 48K

beepdown.bim : beepdown.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:beepdown.bim map:beepdown.map beepdown.obj a_nask.obj

beepdown.hrb : beepdown.bim Makefile
	$(BIM2HRB) beepdown.bim beepdown.hrb 48K

color2.bim : color2.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:color2.bim map:color2.map color2.obj a_nask.obj

color2.hrb : color2.bim Makefile
	$(BIM2HRB) color2.bim color2.hrb 48K

haribote.img : ipl.bin haribote.sys Makefile \
		hello.hrb hello2.hrb a.hrb hello3.hrb hello4.hrb bug1.hrb bug2.hrb bug3.hrb \
		hello5.hrb	winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb \
		lines.hrb walk.hrb noodle.hrb beepdown.hrb color2.hrb
	dd if=ipl.bin of=haribote.img bs=1 
	dd if=/dev/zero of=haribote.img bs=512 seek=1 count=2879
	- sudo umount /mnt
	sudo mount -o loop haribote.img /mnt
	sudo cp  haribote.sys	/mnt
	sudo cp  ipl.nas	/mnt
	sudo cp  asmhead.nas	/mnt
	sudo cp  hello.hrb	/mnt
	sudo cp  hello2.hrb /mnt
	sudo cp  hello5.hrb /mnt
	sudo cp  a.hrb /mnt
	sudo cp  hello3.hrb /mnt
	sudo cp  hello4.hrb /mnt
	sudo cp  bug1.hrb   /mnt
	sudo cp  bug2.hrb   /mnt
	sudo cp  bug3.hrb   /mnt
	sudo cp  winhelo.hrb   /mnt
	sudo cp  winhelo2.hrb   /mnt
	sudo cp  winhelo3.hrb   /mnt
	sudo cp  star1.hrb   /mnt
	sudo cp  stars.hrb   /mnt
	sudo cp  stars2.hrb   /mnt
	sudo cp  lines.hrb   /mnt
	sudo cp  walk.hrb   /mnt
	sudo cp  noodle.hrb   /mnt
	sudo cp  beepdown.hrb   /mnt
	sudo cp  color2.hrb   /mnt
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
