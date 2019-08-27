myos.img : ipl.asm Makefile
	nasm -o myos.img ipl.asm -l ipl.lst
