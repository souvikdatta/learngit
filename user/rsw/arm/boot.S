/*
 * FILE: boot.S
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


#define CPU_MMU_BIT 0x00000001
#define NEXX_OFFSET  (0x600000 + 0x8000)

.set  UND_STACK_SIZE, 0x00000004
.set  ABT_STACK_SIZE, 0x00000004
.set  FIQ_STACK_SIZE, 0x00000004
.set  IRQ_STACK_SIZE, 0X00000400
.set  SVC_STACK_SIZE, 0x00000400

.set  MODE_USR, 0x10 /* User Mode */
.set  MODE_FIQ, 0x11 /* FIQ Mode */
.set  MODE_IRQ, 0x12 /* IRQ Mode */
.set  MODE_SVC, 0x13 /* Supervisor Mode */
.set  MODE_ABT, 0x17 /* Abort Mode */
.set  MODE_UND, 0x1B /* Undefined Mode */
.set  MODE_SYS, 0x1F /* System Mode */

.equ    F_BIT, 0x40
.equ    I_BIT, 0x80



.section .traptab, "ax"
.global _start, _halt_system, irq

.macro DELAY
    nop
    nop
    nop
.endm

_start:
    ldr pc, _reset          /* Reset */
    ldr pc, _undefined      /* Undefined */
    ldr pc, _swi            /* SWI */
    ldr pc, _prefetchAbort  /* Prefetch Abort */
    ldr pc, _dataAbort      /* Data Abort */
    ldr pc, _reserved       /* reserved */
    ldr pc,  _irq           /* IRQ */
    ldr pc,  _fiq           /* FIQ */


_reset:
.word reset

_undefined:
.word NEXX_OFFSET+4

_swi:
.word NEXX_OFFSET+8

_prefetchAbort:
.word NEXX_OFFSET+12

_dataAbort:
.word NEXX_OFFSET+16

_reserved:
.word NEXX_OFFSET+20

_irq:
.word NEXX_OFFSET+24

_fiq:
.word NEXX_OFFSET+28



.align 5
reset:
    /*
     * set the cpu to SVC32 mode
     */
p1:
    mrs r0,cpsr
    bic r0,r0,#0x1f
    orr r0,r0,#0xd3
    msr cpsr,r0
p2:
    @@ Disable MMU
    mrc     p15, 0, r2, c1, c0, 0
    @orr     r2, r2, #CPU_MMU_BIT
    bic     r2, r2, #CPU_MMU_BIT
    mcr     p15, 0, r2, c1, c0, 0
    DELAY
p3:
    @@ Initialize .bss
    ldr   r0, =_sbss
    ldr   r1, =_ebss
    ldr   r2, = bss_size
p4:
    mov   r4, #0
zero:
    strb  r4, [r0], #1
    subs  r2, r2, #1
    bne   zero

    ldr sp,=_estack

    @@ Call to C
    bl loader
    b .



.align 5
_halt_system:
    b .


.section ".bss"
.align 8

_stack:
    .zero (1024*8)
_estack:

.previous
