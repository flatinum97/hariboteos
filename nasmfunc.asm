; nasmfunc.asm
; TAB=4

bits 32
section .text
                GLOBAL io_hlt, write_mem8

io_hlt:
                HLT
                RET

write_mem8:     ; void write_mem8(int addr, int data)
                MOV             ECX, [ESP+4]            ; addr: [ESP+4]
                MOV             AL, [ESP+8]             ; data: [ESP+8]
                MOV             [ECX], AL
                RET
