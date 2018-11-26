/*
 * FILE: regs.c
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



/*
The allocation of the bits within the CPSR (and the SPSR registers to which it is saved) is:
  31 30 29 28  ---   7   6   -   4   3   2   1   0
  N  Z  C  V         I   F       M4  M3  M2  M1  M0

                                 0   0   0   0   0     User26 mode
                                 0   0   0   0   1     FIQ26 mode
                                 0   0   0   1   0     IRQ26 mode
                                 0   0   0   1   1     SVC26 mode
                                 1   0   0   0   0     User mode
                                 1   0   0   0   1     FIQ mode
                                 1   0   0   1   0     IRQ mode
                                 1   0   0   1   1     SVC mode
                                 1   0   1   1   1     ABT mode
                                 1   1   0   1   1     UND mode

*/
#if 0
/* read co-processor 15, register #1 (control register) */
unsigned long read_p15_c1 (void)
{
    unsigned long value;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c1, c0, 0   @ read control reg\n"
        : "=r" (value)
        :
        : "memory");

    return value;
}

/* read r0, register #1 (control register) */
unsigned long read_r0 (void)
{
    unsigned long value;

    __asm__ __volatile__(
        "ldr    %0, [lr, #-4]   @ read control reg\n"
        : "=r" (value)
        :
        : "memory");

    return value&0x00ffffff;
}

#endif
