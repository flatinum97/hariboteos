myos.img : ipl10.asm Makefile
	nasm -o myos.img ipl10.asm -l ipl.lst

run: myos.img
	qemu-system-i386 -L . -m 32 -rtc base=localtime -vga std -drive file=myos.img,index=0,if=floppy,format=raw

clean:
	rm -f ipl.lst
