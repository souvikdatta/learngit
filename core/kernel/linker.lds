/*
 * FILE: linker.lds
 *
 * VERSION: 0.0.1
 *
 * Author: Hector Marco-Gisbert <hmarco@hmarco.org>
 *
 * Comment: Generic boot code, sets up a valid stack and jumps to the C code
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

NEXX_OFFSET = 0x8000;

SECTIONS {

   . = 0x600000 + NEXX_OFFSET;
   .text : { 
      *(.nexx.traptab);
      *(.text);
   }

   .data : {
      *(.data);
      *(.rodata);
   }

   .bss : {
      _sbss = .;
      *(COMMON)
      *(.bss);
      _ebss = .;
  } 
  bss_size = _ebss - _sbss;
}
