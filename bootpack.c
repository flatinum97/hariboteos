#include "bootpack.h"

#define PORT_KEYDAT          0x0060
#define PORT_KEYSTA          0x0064
#define PORT_KEYCMD          0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE    0x60
#define KEYCMD_SENDTO_MOUSE  0xd4
#define KBC_MODE             0x47
#define MOUSECMD_ENABLE      0xf4

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void HariMain(void)
{
    int i, j;
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
    struct MOUSE_DEC mdec;

    init_gdtidt();
    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    char mcursor[16 * 16], s[4], keybuf[32], mousebuf[128];
    const int mx = binfo->scrnx / 2 - 16;
    const int my = binfo->scrny / 2 - 16;

    init_mouse_cursor8(mcursor, COL8_008484);
    putblock_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    init_pic();
    io_sti();

    io_out8(PIC0_IMR, 0xf9);
    io_out8(PIC1_IMR, 0xef);

    init_keyboard();
    enable_mouse(&mdec);

    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);

    for (;;) {
        io_cli();
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            io_stihlt();
        } else {
            if (fifo8_status(&keyfifo) != 0) {
                i = fifo8_get(&keyfifo);
                io_sti();
                sprintf(s, "%x", i);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
                putfont8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
            } else if (fifo8_status(&mousefifo) != 0) {
                i = fifo8_get(&mousefifo);
                io_sti();
                if (mouse_decode(&mdec, i) != 0) {
                    sprintf(s, "[lcr %d %d]", mdec.x, mdec.y);
                    if ((mdec.btn & 0x01) != 0) {
                        s[1] = 'L';
                    }
                    if ((mdec.btn & 0x02) != 0) {
                        s[3] = 'R';
                    }
                    if ((mdec.btn & 0x04) != 0) {
                        s[2] = 'C';
                    }
                    boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 - 1, 31);
                    putfont8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
                }
            }
        }
    }
}

void wait_KBC_standby(void)
{
    for (;;) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(void)
{
    wait_KBC_standby();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_standby();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}

void enable_mouse(struct MOUSE_DEC *mdec)
{
    wait_KBC_standby();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_standby();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    mdec->phase = 0;
    return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
    switch (mdec->phase) {
        case 0:
            if (dat == 0xfa) {
                mdec->phase = 1;
            }
            return 0;
        case 1:
            if ((dat & 0xc8) == 0x08) {
                mdec->buf[0] = dat;
                mdec->phase = 2;
            }
            return 0;
        case 2:
            mdec->buf[1] = dat;
            mdec->phase = 3;
            return 0;
        case 3:
            mdec->buf[2] = dat;
            mdec->phase = 1;
            mdec->btn = mdec->buf[0] & 0x07;
            mdec->x = mdec->buf[1];
            mdec->y = mdec->buf[2];
            if ((mdec->buf[0] & 0x10) != 0) {
                mdec->x |= 0xffffff00;
            }
            if ((mdec->buf[0] & 0x20) != 0) {
                mdec->y |= 0xffffff00;
            }
            mdec->y = - mdec->y;
            return 1;
    }
    return -1;
}
