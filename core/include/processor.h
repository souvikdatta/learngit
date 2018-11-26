/*
 * FILE: processor.h
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



#define CPUID_ID    0
#define CPUID_CACHETYPE 1
#define CPUID_TCM   2
#define CPUID_TLBTYPE   3


#define __stringify_1(x)    #x
#define __stringify(x)      __stringify_1(x)

#define read_cpuid(reg)                         \
    ({                              \
        unsigned int __val;                 \
        asm("mrc    p15, 0, %0, c0, c0, " __stringify(reg)  \
            : "=r" (__val)                  \
            :                           \
            : "cc");                        \
        __val;                          \
    })


static inline unsigned int get_cr(void)
{
    unsigned int val;
    asm("mrc p15, 0, %0, c1, c0, 0  @ get CR" : "=r" (val) : : "cc");
    return val;
}

