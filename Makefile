ipl10.bin : ipl10.asm Makefile
	nasm ipl10.asm -o ipl10.bin -l ipl10.lst

asmhead.sys : asmhead.asm Makefile
	nasm asmhead.asm -o asmhead.sys -l asmhead.lst

myos.img : ipl10.bin asmhead.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i myos.img ::
	mcopy -i myos.img asmhead.sys ::

img :
	make -r myos.img

run : myos.img
	qemu-system-i386 -L . -m 32 -rtc base=localtime -vga std -drive file=myos.img,index=0,if=floppy,format=raw

clean :
	rm -f ipl10.bin
	rm -f ipl10.lst
	rm -f asmhead.sys
	rm -f asmhead.lst
	rm -f myos.img
