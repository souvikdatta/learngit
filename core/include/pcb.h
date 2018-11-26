/*
 * FILE: pcb.h
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


typedef struct {
   nexx_addr r0;
   nexx_addr r1;
   nexx_addr r2;
   nexx_addr r3;
   nexx_addr r4;
   nexx_addr r5;
   nexx_addr r6;
   nexx_addr r7;
   nexx_addr r8;
   nexx_addr r9;
   nexx_addr r10;
   nexx_addr fp; //fp
   nexx_addr r12; 
   nexx_addr sp; //sp
   nexx_addr lr; //lr
   nexx_addr pc; //pc
   nexx_addr psr;
   nexx_s32_t id;
   nexx_s32_t firstex;
   nexx_u32_t tbr;
} cpuCtxt_t;
