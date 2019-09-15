TARGET=haribote

run :
	docker run --rm -v $(shell pwd):/mnt haribote make img
	qemu-system-i386 -fda haribote.img

$(TARGET).img : ipl10.bin $(TARGET).sys
	echo $(TARGET).sys > $(TARGET).name
	dd if=ipl10.bin of=$(TARGET).img count=2880 bs=512 conv=notrunc
	dd if=$(TARGET).name of=$(TARGET).img count=1 bs=512 seek=19 conv=notrunc
	dd if=$(TARGET).sys  of=$(TARGET).img count=4 bs=512 seek=33 conv=notrunc

%.o : %.c
	gcc -m32 -c -fno-pic -nostdlib -o $@ $<

asmhead.o : asmhead.asm
	nasm $^ -o $@ -l asmhead.lst

nasmfunc.o : nasmfunc.asm
	nasm -felf32 $^ -o $@ -l nasmfunc.lst

$(TARGET).bin : bootpack.o  nasmfunc.o
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $(TARGET).bin $^

$(TARGET).sys : asmhead.o $(TARGET).bin
	cat $^ > $(TARGET).sys

ipl10.bin : ipl10.asm
	nasm -fbin -o $@ $^ -l ipl10.lst

img :
	make -r $(TARGET).img

clean :
	rm -f *.bin
	rm -f *.lst
	rm -f *.sys
	rm -f *.img
	rm -f *.o
	rm -f *.hrb
	rm -f *.name
