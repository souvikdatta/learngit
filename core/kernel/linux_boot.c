/*
 * FILE: linux_boot.c
 *
 * VERSION: 0.0.1
 *
 * Author: Hector Marco-Gisbert <hmarco@hmarco.org>
 *
 * LICENSE:
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */


#include "./../../autoconf2.h"

#ifdef IS_LINUX_PARTITION

#include "types.h"

/* list of possible tags */
#define ATAG_NONE       0x00000000
#define ATAG_CORE       0x54410001
#define ATAG_MEM        0x54410002
#define ATAG_VIDEOTEXT  0x54410003
#define ATAG_RAMDISK    0x54410004
#define ATAG_INITRD2    0x54420005
#define ATAG_SERIAL     0x54410006
#define ATAG_REVISION   0x54410007
#define ATAG_VIDEOLFB   0x54410008
#define ATAG_CMDLINE    0x54410009

#define u8  nexx_u8_t
#define u16  nexx_u16_t
#define u32  nexx_u32_t


/* structures for each atag */
struct atag_header {
        u32 size; /* length of tag in words including this header */
        u32 tag;  /* tag type */
};

struct atag_core {
        u32 flags;
        u32 pagesize;
        u32 rootdev;
};

struct atag_mem {
        u32     size;
        u32     start;
};

struct atag_videotext {
        u8              x;
        u8              y;
        u16             video_page;
        u8              video_mode;
        u8              video_cols;
        u16             video_ega_bx;
        u8              video_lines;
        u8              video_isvga;
        u16             video_points;
};

struct atag_ramdisk {
        u32 flags;
        u32 size;
        u32 start;
};

struct atag_initrd2 {
        u32 start;
        u32 size;
};

struct atag_serialnr {
        u32 low;
        u32 high;
};

struct atag_revision {
        u32 rev;
};

struct atag_videolfb {
        u16             lfb_width;
        u16             lfb_height;
        u16             lfb_depth;
        u16             lfb_linelength;
        u32             lfb_base;
        u32             lfb_size;
        u8              red_size;
        u8              red_pos;
        u8              green_size;
        u8              green_pos;
        u8              blue_size;
        u8              blue_pos;
        u8              rsvd_size;
        u8              rsvd_pos;
};

struct atag_cmdline {
        char    cmdline[1];
};

struct atag {
        struct atag_header hdr;
        union {
                struct atag_core         core;
                struct atag_mem          mem;
                struct atag_videotext    videotext;
                struct atag_ramdisk      ramdisk;
                struct atag_initrd2      initrd2;
                struct atag_serialnr     serialnr;
                struct atag_revision     revision;
                struct atag_videolfb     videolfb;
                struct atag_cmdline      cmdline;
        } u;
};

#define tag_nexxt(t)     ((struct tag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)  ((sizeof(struct atag_header) + sizeof(struct type)) >> 2)
static struct atag *params; /* used to point at the current tag */


static void setup_memory_tags(void)
{
            params->hdr.tag = ATAG_MEM;
            params->hdr.size = tag_size(atag_mem);

            params->u.mem.start = 0xC00000;
            params->u.mem.size = 0x7300000;

            params = (struct atag *)tag_nexxt(params);
}

static void setup_ramdisk_tag(void)
{
    /* an ATAG_RAMDISK node tells the kernel how large the
         * decompressed ramdisk will become.
     */
    params->hdr.tag = ATAG_RAMDISK;
    params->hdr.size = tag_size(atag_ramdisk);
    
    params->u.ramdisk.start = 0x7F00000; // 117 MB
    params->u.ramdisk.size = 0x100000;//4096; 
    params->u.ramdisk.flags = 1;    /* automatically load ramdisk */
    
    params = (struct atag *)tag_nexxt(params);
}


static void setup_start_tag(void)
{
    params = (struct atag *)0xC00100;

    params->hdr.tag = ATAG_CORE;
    params->hdr.size = tag_size(atag_core);

    params->u.core.flags = 0;
    params->u.core.pagesize = 0;
    params->u.core.rootdev = 0;

    params = (struct atag *)tag_nexxt(params);
}


static void setup_end_tag(void)
{
    params->hdr.tag = ATAG_NONE;
    params->hdr.size = 0;
}


void setup_tags(void)
{
    setup_start_tag();
    setup_memory_tags();
    setup_ramdisk_tag();
    setup_end_tag();
}

#endif
