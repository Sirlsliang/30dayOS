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
MAKE      := wine $(TOOL_PATH)make.exe -r

default :
	$(MAKE) haribote.img

haribote.img : hairbote/ipl.bin hairbote/haribote.sys Makefile \
		a/a.hrb hello3/hello3.hrb hello4/hello4.hrb winhelo/winhelo.hrb \
		winhelo2/winhelo2.hrb winhelo3/winhelo3.hrb star1/star1.hrb stars/stars.hrb \
		stars2/stars2.hrb lines/lines.hrb walk/walk.hrb noodle/noodle.hrb beepdown/beepdown.hrb \
		color2/color2.hrb
	dd if=ipl.bin of=haribote.img bs=1 
	dd if=/dev/zero of=haribote.img bs=512 seek=1 count=2879
	- sudo umount /mnt
	sudo mount -o loop haribote.img /mnt
	sudo cp  hairbote/haribote.sys	/mnt
	sudo cp  hairbote/ipl.nas	/mnt
	sudo cp  a/a.hrb /mnt
	sudo cp  hello3/hello3.hrb /mnt
	sudo cp  hello4/hello4.hrb /mnt
	sudo cp  winhelo/winhelo.hrb   /mnt
	sudo cp  winhelo2/winhelo2.hrb   /mnt
	sudo cp  winhelo3/winhelo3.hrb   /mnt
	sudo cp  star1/star1.hrb   /mnt
	sudo cp  stars/stars.hrb   /mnt
	sudo cp  stars2/stars2.hrb   /mnt
	sudo cp  lines/lines.hrb   /mnt
	sudo cp  walk/walk.hrb   /mnt
	sudo cp  noodle/noodle.hrb   /mnt
	sudo cp  beepdown/beepdown.hrb   /mnt
	sudo cp  color2/color2.hrb   /mnt
	sudo umount /mnt

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
	$(MAKE) clean
	-rm -f *.img
