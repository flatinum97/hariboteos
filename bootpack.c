#include "bootpack.h"

extern struct KEYBUF keybuf;

void HariMain(void)
{
    int i, j;
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;

    init_gdtidt();
    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    char mcursor[16 * 16];
    const int mx = binfo->scrnx / 2 - 16;
    const int my = binfo->scrny / 2 - 16;

    init_mouse_cursor8(mcursor, COL8_008484);
    putblock_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    init_pic();
    io_sti();

    io_out8(PIC0_IMR, 0xf9);
    io_out8(PIC1_IMR, 0xef);

    for (;;) {
        io_cli();
        if (keybuf.next == 0) {
            io_stihlt();
        } else {
            i = keybuf.data[0];
            keybuf.next--;
            for (j = 0; j < keybuf.next; j++) {
                keybuf.data[j] = keybuf.data[j + 1];
            }
            io_sti();
            unsigned char s[4];
            sprintf(s, "%x", i);
            boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
            putfont8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
        }
    }
}
