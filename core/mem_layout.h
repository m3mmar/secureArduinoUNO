#ifndef MEM_LAYOUT_H
#define MEM_LAYOUT_H

#include <avr/io.h>
#include <avr/fuse.h>

#ifndef BOOTSIZE
#error "BOOTSIZE must be defined!"
#endif

#if BOOTSIZE == 1

#define FUSE_BOOTSZ (FUSE_BOOTSZ0)

#define APP_META 0x3D00 //0xFD00
#define APP_METAW 0x1E80 //0x7E80

#define SHADOW 0x3E00 //0xFE00
#define SHADOWW 0x1F00 //0x7F00

#define SHADOW_META 0x7B00 //0x1FB00
#define SHADOW_METAW 0x3D80 //0xFD80

#define MICROVISOR 0x7C00 //0x1FC00
#define MICROVISORW 0x3E00 //0xFE00

#elif BOOTSIZE == 2

#define FUSE_BOOTSZ (FUSE_BOOTSZ1)

#define APP_META 0x3B00  //0xFB00
#define APP_METAW 0x1D80 //0x7D80

#define SHADOW 0x3C00 //0xFC00
#define SHADOWW 0x1E00 //0x7E00

#define SHADOW_META 0x7700 //0x1F700
#define SHADOW_METAW 0x3B80 //0xFB80

#define MICROVISOR 0x7800 //0x1F800
#define MICROVISORW 0x3C00 //0xFC00

#elif BOOTSIZE == 4 

#define FUSE_BOOTSZ (FUSE_BOOTSZ0 & FUSE_BOOTSZ1)

#define APP_META 0x3700  
#define APP_METAW 0x1B80

#define SHADOW 0x3800
#define SHADOWW 0x1C00

#define SHADOW_META 0x6F00
#define SHADOW_METAW 0x3780

#define MICROVISOR 0x7000
#define MICROVISORW 0x3800


#else
#error "Incorrect BOOTSIZE value!"
#endif

#define APP_START 0x0000
#define APP_STARTW 0x0000

#define MEM_END 0x7FFF
#define MEM_ENDW 0x4000

#endif
