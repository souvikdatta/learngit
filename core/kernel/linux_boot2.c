/*
 * FILE: linux_boot2.c
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


#include "linux_boot.h"
#include "types.h"

#define tag_nexxt(t)     ((struct tag *)((nexx_u32_t *)(t) + (t)->hdr.size))
#define tag_size(type)  ((sizeof(struct tag_header) + sizeof(struct type)) >> 2)

#define ATAG_NONE   0x00000000      //2  Empty tag used to end list
#define ATAG_CORE   0x54410001      //5 (2 if empty)  First tag used to start list
#define ATAG_MEM    0x54410002      //4   Describes a physical area of memory
#define ATAG_VIDEOTEXT  0x54410003  //5   Describes a VGA text display
#define ATAG_RAMDISK    0x54410004  //5   Describes how the ramdisk will be used in kernel
#define ATAG_INITRD2    0x54420005  //4   Describes where the compressed ramdisk image is placed in memory
#define ATAG_SERIAL 0x54410006      //4   64 bit board serial number
#define ATAG_REVISION   0x54410007  //3   32 bit board revision number
#define ATAG_VIDEOLFB   0x54410008  //8   Initial values for vesafb-type framebuffers
#define ATAG_CMDLINE    0x54410009  //2 + ((length_of_cmdline + 3) / 4)   Command line to pass to kernel

static struct atag *params; /* used to point at the current tag */

static void
setup_core_tag(void * address,long pagesize)
{
    params = (struct tag *)address;         /* Initialise parameters to start at given address */

    params->hdr.tag = ATAG_CORE;            /* start with the core tag */
    params->hdr.size = tag_size(atag_core); /* size the tag */

    params->u.core.flags = 1;               /* ensure read-only */
    params->u.core.pagesize = pagesize;     /* systems pagesize (4k) */
    params->u.core.rootdev = 0;             /* zero root device (typicaly overidden from commandline )*/

    params = tag_nexxt(params);              /* move pointer to nexxt tag */
}

static void
setup_mem_tag(nexx_u32_t start, nexx_u32_t len)
{
    params->hdr.tag = ATAG_MEM;             /* Memory tag */
    params->hdr.size = tag_size(atag_mem);  /* size tag */

    params->u.mem.start = start;            /* Start of memory area (physical address) */
    params->u.mem.size = len;               /* Length of area */

    params = tag_nexxt(params);              /* move pointer to nexxt tag */
}

static void
setup_end_tag(void)
{
    params->hdr.tag = ATAG_NONE;            /* Empty tag ends list */
    params->hdr.size = 0;                   /* zero length */
}


static void
setup_tags(void)
{
    setup_core_tag(0x100, 4096);            /* standard core tag 4k pagesize */
    setup_mem_tag(0x0, 0x400000);    /* 64Mb at 0x10000000 */
    //setup_mem_tag(0x10000000, 0x400000);    /* 64Mb at 0x10000000 */
    //setup_mem_tag(0x18000000, 0x400000);    /* 64Mb at 0x18000000 */
    setup_end_tag(void);                    /* end of tags */
}

