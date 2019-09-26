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

struct FIFO8 {
    unsigned char *buf;
    int p, q, size, free, flags;
};

struct MOUSE_DEC {
    unsigned char buf[3], phase;
};

void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
void io_out8(int port, int data);
char io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21(void);
void asm_inthandler2c(void);

void init_palette(void);
void init_screen(char *vram, int xsize, int ysize);
void init_mouse_cursor8(char *mouse, char bc);
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfont8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void putblock_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);

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

#define ADR_BOOTINFO 0x00000ff0

#define AR_INTGATE32 0x008e

void init_pic(void);
#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR  0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR  0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1

#define FLAGS_OVERRUN 0x0001;

void fifo8_init(struct FIFO8 *fifo, int size, unsigned char * buf);
int fifo8_get(struct FIFO8 *fifo);
int fifo8_put(struct FIFO8 *fifo, unsigned char data);
int fifo8_status(struct FIFO8 *fifo);

void wait_KBC_standby(void);
void init_keyboard(void);
void enable_mouse(struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);
