ipl10.bin : ipl10.asm Makefile
	nasm ipl10.asm -o ipl10.bin -l ipl10.lst

asmhead.bin : asmhead.asm Makefile
	nasm asmhead.asm -o asmhead.bin -l asmhead.lst

bootpack.bin : bootpack.c
	gcc -c -m32 -fno-pic -o bootpack.bin bootpack.c

myos.sys : asmhead.bin bootpack.bin
	cat asmhead.bin bootpack.bin > myos.sys

myos.img : ipl10.bin myos.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i myos.img ::
	mcopy -i myos.img myos.sys ::

img :
	make -r myos.img

run : myos.img
	qemu-system-i386 -L . -m 32 -rtc base=localtime -vga std -drive file=myos.img,index=0,if=floppy,format=raw

clean :
	rm -f *.bin
	rm -f *.lst
	rm -f *.sys
	rm -f *.img
