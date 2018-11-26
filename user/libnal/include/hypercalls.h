/*
 * FILE: hypercalls.h
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


#include "types.h"

#define arm_write_console  0

#define __STR(x) #x
#define TO_STR(x) __STR(x)

#define SAVE_R0_R3 \
      __asm__ __volatile__ (" push {r0}\n\t" \
      "push {r1}\n\t" \
      "push {r2}\n\t" \
      "push {r3}\n\t":: :)

#define RESTORE_R0_R3 \
      __asm__ __volatile__ (" pop {r3}\n\t" \
      "pop {r2}\n\t" \
      "pop {r1}\n\t" \
      "pop {r0}\n\t":: :)

#define _NEXX_HCALL1(_hc, p0) \
      SAVE_R0_R3; \
      __asm__ __volatile__ ("mov r0, #"TO_STR(_hc)"\n\t" \
           "mov r1, %1\n\t" \
           "SWI 0x16\n\t" \
          :: "r" (_hc), "r" (p0) : "r0", "r1", "r2", "r3" ); \
      RESTORE_R0_R3;

#define _NEXX_HCALL2(_hc, p0, p1) \
      SAVE_R0_R3; \
      __asm__ __volatile__ ("mov r0, #"TO_STR(_hc)"\n\t" \
           "mov r1, %1\n\t" \
           "mov r2, %2\n\t" \
           "SWI 0x16\n\t" \
          :: "r" (_hc), "r" (p0), "r" (p1) : "r0", "r1", "r2", "r3"); \
      RESTORE_R0_R3; 



#if 0
static inline void NEXX_arm_write_console(nexx_s8_t *str, nexx_s32_t n) {
   _NEXX_HCALL2(arm_write_console, str,n);
}
#endif


#if 0
static inline void NEXX_arm_write_console(nexx_s8_t *str, nexx_s32_t n) {
      SAVE_R0_R3;
      __asm__ __volatile__ ("mov r0, #"TO_STR(arm_write_console)"\n\t"
           "mov r1, %0\n\t"
           "mov r2, %1\n\t"
           "SWI 0x16\n\t"
          ::  "r" (str), "r" (n) : );
      RESTORE_R0_R3 ;
}
#endif
