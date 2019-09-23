#define COL8_000000  0
#define COL8_FF0000  1
#define COL8_00FF00  2
#define COL8_FFFF00  3
#define COL8_0000FF  4
#define COL8_FF00FF  5
#define COL8_00FFFF  6
#define COL8_FFFFFF  7
#define COL8_C6C6C6  8
#define COL8_840000  9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

void io_hlt(void);

void init_palette(void);
void init_screen(char *vram, int xsize, int ysize);
void init_mouse_cursor8(char *mouse, char bc);
void init_gdtidt(void);

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

    for (;;) {
        io_hlt();
    }
}
