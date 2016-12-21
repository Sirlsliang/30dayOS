OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj int.obj fifo.obj \
								keyboard.obj mouse.obj memory.obj sheet.obj timer.obj mtask.obj window.obj	\
								console.obj	file.obj

OBJS_API =	api001.obj api002.obj api003.obj api004.obj api005.obj api006.obj \
			api007.obj api008.obj api009.obj api010.obj api011.obj api012.obj \
			api013.obj api014.obj api015.obj api016.obj api017.obj api018.obj \
			api019.obj api020.obj	api021.obj api022.obj	api023.obj api024.obj \
			api025.obj api026.obj alloca.obj
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
GOLIB			:= wine $(TOOL_PATH)golib00.exe

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


apilib.lib : Makefile $(OBJS_API)
	$(GOLIB) $(OBJS_API) out:apilib.lib

a.bim : a.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:a.bim map:a.map a.obj apilib.lib

a.hrb : a.bim Makefile
	$(BIM2HRB) a.bim a.hrb 0

hello3.bim : hello3.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello3.bim map:hello3.map hello3.obj apilib.lib

hello3.hrb : hello3.bim Makefile
	$(BIM2HRB) hello3.bim hello3.hrb 0

hello4.bim : hello4.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello4.bim stack:1k map:hello4.map \
		hello4.obj apilib.lib

hello4.hrb : hello4.bim Makefile
	$(BIM2HRB) hello4.bim hello4.hrb 0

hello5.bim : hello5.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello5.bim stack:1k map:hello5.map hello5.obj

hello5.hrb : hello5.bim Makefile
	$(BIM2HRB) hello5.bim hello5.hrb 0

winhelo.bim : winhelo.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo.bim stack:8k map:winhelo.map \
		winhelo.obj apilib.lib

winhelo.hrb : winhelo.bim Makefile
	$(BIM2HRB) winhelo.bim winhelo.hrb 0

winhelo2.bim : winhelo2.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo2.bim stack:8k map:winhelo2.map \
		winhelo2.obj apilib.lib

winhelo2.hrb : winhelo2.bim Makefile
	$(BIM2HRB) winhelo2.bim winhelo2.hrb 0

winhelo3.bim : winhelo3.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:winhelo3.bim stack:1k map:winhelo3.map \
		winhelo3.obj apilib.lib

winhelo3.hrb : winhelo3.bim Makefile
	$(BIM2HRB) winhelo3.bim winhelo3.hrb 40k

star1.bim : star1.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:star1.bim stack:1k map:star1.map \
		star1.obj apilib.lib

star1.hrb : star1.bim Makefile
	$(BIM2HRB) star1.bim star1.hrb 47k

stars.bim : stars.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:stars.bim stack:1k map:stars.map \
		stars.obj apilib.lib

stars.hrb : stars.bim Makefile
	$(BIM2HRB) stars.bim stars.hrb 47k

stars2.bim : stars2.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:stars2.bim stack:1k map:stars2.map \
		stars2.obj apilib.lib

stars2.hrb : stars2.bim Makefile
	$(BIM2HRB) stars2.bim stars2.hrb 47k

lines.bim : lines.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:lines.bim stack:1k map:lines.map \
		lines.obj apilib.lib

lines.hrb : lines.bim Makefile
	$(BIM2HRB) lines.bim lines.hrb 48k

walk.bim : walk.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:walk.bim stack:1k map:walk.map \
		walk.obj apilib.lib

walk.hrb : walk.bim Makefile
	$(BIM2HRB) walk.bim walk.hrb 48k

noodle.bim : noodle.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:noodle.bim stack:1k map:noodle.map \
		noodle.obj apilib.lib

noodle.hrb : noodle.bim Makefile
	$(BIM2HRB) noodle.bim noodle.hrb 40k

beepdown.bim : beepdown.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:beepdown.bim stack:1k map:beepdown.map \
		beepdown.obj apilib.lib

beepdown.hrb : beepdown.bim Makefile
	$(BIM2HRB) beepdown.bim beepdown.hrb 40k

color.bim : color.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:color.bim stack:1k map:color.map \
		color.obj apilib.lib

color.hrb : color.bim Makefile
	$(BIM2HRB) color.bim color.hrb 56k

color2.bim : color2.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:color2.bim stack:1k map:color2.map \
		color2.obj apilib.lib

color2.hrb : color2.bim Makefile
	$(BIM2HRB) color2.bim color2.hrb 56k

sosu2.bim : sosu2.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:sosu2.bim stack:11k map:sosu2.map \
		sosu2.obj apilib.lib

sosu2.hrb : sosu2.bim Makefile
	$(BIM2HRB) sosu2.bim sosu2.hrb 56k

sosu3.bim : sosu3.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:sosu3.bim stack:11k map:sosu3.map \
		sosu3.obj apilib.lib

sosu3.hrb : sosu3.bim Makefile
	$(BIM2HRB) sosu3.bim sosu3.hrb 56k

type.bim : type.obj apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:type.bim stack:11k map:type.map \
		type.obj apilib.lib

type.hrb : type.bim Makefile
	$(BIM2HRB) type.bim type.hrb 56k

haribote.img : ipl.bin haribote.sys Makefile \
		a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb \
		winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb \
		noodle.hrb beepdown.hrb color2.hrb sosu2.hrb sosu3.hrb type.hrb
	dd if=ipl.bin of=haribote.img bs=1 
	dd if=/dev/zero of=haribote.img bs=512 seek=1 count=2879
	- sudo umount /mnt
	sudo mount -o loop haribote.img /mnt
	sudo cp  haribote.sys	/mnt
	sudo cp  ipl.nas	/mnt
	sudo cp  asmhead.nas	/mnt
	sudo cp  hello5.hrb /mnt
	sudo cp  a.hrb /mnt
	sudo cp  hello3.hrb /mnt
	sudo cp  hello4.hrb /mnt
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
	sudo cp  sosu2.hrb   /mnt
	sudo cp  sosu3.hrb   /mnt
	sudo cp  type.hrb   /mnt
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
