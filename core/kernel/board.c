/*
 * FILE: board.c
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
 * control registers
 */
#define VERSATILE_SCTL_BASE            0x101E0000   /* System controller */

/*
 * System controller bit assignment
 */
#define VERSATILE_REFCLK    0
#define VERSATILE_TIMCLK    1

#define VERSATILE_TIMER1_EnSel  15
#define VERSATILE_TIMER2_EnSel  17
#define VERSATILE_TIMER3_EnSel  19
#define VERSATILE_TIMER4_EnSel  21


int board_init (void)
{
    /*
     * set clock frequency:
     *  VERSATILE_REFCLK is 32KHz
     *  VERSATILE_TIMCLK is 1MHz
     */
    xprintf("Antes [%x]\n",  *(volatile unsigned int *)(VERSATILE_SCTL_BASE));
    *(volatile unsigned int *)(VERSATILE_SCTL_BASE) |=
      ((VERSATILE_TIMCLK << VERSATILE_TIMER1_EnSel) | (VERSATILE_TIMCLK << VERSATILE_TIMER2_EnSel) |
       (VERSATILE_TIMCLK << VERSATILE_TIMER3_EnSel) | (VERSATILE_TIMCLK << VERSATILE_TIMER4_EnSel));

    xprintf("Despues [%x]\n",  *(volatile unsigned int *)(VERSATILE_SCTL_BASE));
//    icache_enable ();

    return 0;
}

