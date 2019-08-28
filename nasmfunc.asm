; nasmfunc.asm
; TAB=4

bits 32
section .text
                GLOBAL io_hlt

io_hlt:
                HLT
                RET
