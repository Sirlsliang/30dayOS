OBJS_BOOTPACK	= bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj int.obj fifo.obj \
								keyboard.obj mouse.obj memory.obj sheet.obj timer.obj mtask.obj window.obj	\
								console.obj	file.obj

# 添加默认命令
TOOL_PATH :=  ../../../z_tools/
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
MAKE      := wine $(TOOL_PATH)make.exe -r

default :
	make ipl.bin
	make haribote.sys

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

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas	:	%.gas	Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj :	%.nas	Makefile
	$(NASK)	$*.nas $*.obj	$*.lst

clean	: 
	-rm asmhead.bin
	-rm hankaku.bin
	-rm *.lst
	-rm *.obj
	-rm *.map
	-rm *.bim
	-rm *.hrb

src :
	$(MAKE) clean
	-rm ipl.bin
	-rm haribote.sys
