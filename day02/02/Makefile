# 添加默认命令
default :
	make img
bootloader : ipl.nas Makefile
	nasm -o bootloader -l ipl.lst ipl.nas
helloos.img : bootloader 
	dd if=/dev/zero of=helloos.img bs=512 count=2880
	dd if=bootloader of=helloos.img bs=1 
img :
	make -r helloos.img
run :	helloos.img
	qemu-system-i386 helloos.img
clean	: 
	rm ipl.lst
	rm bootloader
