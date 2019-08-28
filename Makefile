ipl10.bin : ipl10.asm Makefile
	nasm ipl10.asm -o ipl10.bin -l ipl10.lst

asmhead.bin : asmhead.asm Makefile
	nasm asmhead.asm -o asmhead.bin -l asmhead.lst

nasmfunc.o : nasmfunc.asm Makefile
	nasm -g -f elf nasmfunc.asm -o nasmfunc.o

bootpack.hrb : bootpack.c  nasmfunc.o Makefile
	gcc -c -m32 -fno-pic -nostdlib -T har.ld -g bootpack.c nasmfunc.o -o bootpack.hrb

myos.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > myos.sys

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
	rm -f *.o
	rm -f *.hrb
