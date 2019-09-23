#include "bootpack.h"

void HariMain(void)
{
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

    for (;;) {
        io_hlt();
    }
}
