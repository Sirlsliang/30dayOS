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

os-img = helloos.img
os-sys = haribote.sys

default :
	make img
ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst
#	nasm -o ipl.bin -l ipl.lst ipl.nas

asmhead.bin : asmhead.nas Makefile
	nasm -o asmhead.bin -l asmhead.lst asmhead.nas
#	$(NASK1) asmhead.nas asmhead.bin asmhead.lst

bootpack.gas : bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj : bootpack.nas	Makefile
	$(NASK) bootpack.nas bootpack.obj	bootpack.lst
#	nasm -o bootpack.obj -l bootpack.lst bootpack.nas

naskfunc.obj : naskfunc.nas Makefile
	$(NASK) naskfunc.nas naskfunc.obj	naskfunc.lst
#	nasm -o naskfunc.obj	-l naskfunc.lst naskfunc.nas

bootpack.bim : bootpack.obj	Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map	bootpack.obj naskfunc.obj

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

$(os-sys) : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > $(os-sys)

$(os-img) : ipl.bin haribote.sys
	dd if=ipl.bin of=$(os-img) bs=1 
	dd if=/dev/zero of=$(os-img) bs=512 seek=1 count=2879
	sudo mount -o loop $(os-img) /mnt
	sudo cp  $(os-sys)	/mnt
	sudo umount /mnt



img :
	make -r $(os-img)

run :	$(os-img)
	qemu-system-i386 -fda $(os-img)

clean	: 
	rm -f *.lst
	rm -f *.bin
	rm -f *.gas
	rm -f *.ob
	rm -f bootpack.nas
	rm -f bootpack.map
	rm -f bootpack.bim
	rm -f bootpack.hrb
	rm -f bootpack.obj
	rm -f $(os-sys)

src :
	make clean
	rm -f *.img
