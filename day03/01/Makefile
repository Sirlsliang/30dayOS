# 添加默认命令
TOOL_PATH := ../../z_tools/
NASK			:= wine $(TOOL_PATH)nask.exe





os-img = helloos.img
os-sys = haribote.sys

default :
	make img
ipl.bin : ipl.nas Makefile
#	$(NASK) ipl.nas ipl.bin ipl.lst
	nasm -o ipl.bin -l ipl.lst ipl.nas

$(os-img) : ipl.bin haribote.sys
	dd if=ipl.bin of=$(os-img) bs=1 
	dd if=/dev/zero of=$(os-img) bs=512 seek=1 count=2879
	sudo mount -o loop $(os-img) /mnt
	sudo cp  $(os-sys)	/mnt
	sudo umount /mnt

$(os-sys) : haribote.nas
#	$(NASK) haribote.nas $(os-sys) haribote.lst
	nasm -o $(os-sys) -l haribote.lst haribote.nas

img :
	make -r $(os-img)

run :	$(os-img)
	qemu-system-i386 -fda $(os-img)

clean	: 
	rm -f ipl.lst
	rm -f ipl.bin
	rm -f haribote.lst
	rm $(os-sys)

src :
	make clean
	rm -f *.img
