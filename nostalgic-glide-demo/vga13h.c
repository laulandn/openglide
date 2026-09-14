#ifdef DOS
#include <i86.h>
#include <conio.h>
#endif
#include <string.h> // for memset

#include "vga13h.h"

#ifdef DOS
#ifdef __386__
#define DO_INT(i, r_in, r_out) int386(i, r_in, r_out)
#define VGA_MEMORY 0xA0000
#else
#define DO_INT(i, r_in, r_out) int86(i, r_in, r_out)
#define VGA_MEMORY 0xA0000000L
#endif
#endif


static unsigned char default_palette[768];


void set_mode13h()
{
#ifdef DOS
    union REGS regs;

    regs.w.ax = 0x13;
    DO_INT(0x10, &regs, &regs);
#endif

    dump_palette(default_palette);
}

void unset_mode13h()
{
#ifdef DOS
    union REGS regs;

    regs.w.ax = 0x3;
    DO_INT(0x10, &regs, &regs);
#endif
}

void copy_buffer(unsigned char* buffer)
{
#ifdef DOS
    memcpy((unsigned char *)VGA_MEMORY, buffer, 64000);
#endif
}

void clear_buffer(unsigned char* buffer)
{
    memset(buffer, 0, 64000);
}

void retrace()
{
#ifdef DOS
    while (!(inp(0x03da) & 8))
        ;
    while ((inp(0x03da) & 8))
        ;
#endif
}

void set_palette(unsigned char* palette)
{
    int i;
        
#ifdef DOS
    outp(0x03c8, 0);
    for (i = 0; i < 768; i++) {
        outp(0x03c9, palette[i]);
    }
#endif
}

void dump_palette(unsigned char* palette)
{
    int i;
        
#ifdef DOS
    outp(0x03c7, 0);
    for (i = 0; i < 768; i++) {
        palette[i] = inp(0x03c9);
    }
#endif
}

void reset_palette()
{
    set_palette(default_palette);
}
