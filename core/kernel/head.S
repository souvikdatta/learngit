/*
 * FILE: head.S
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
#define CTXT_SIZE   80

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




.macro DELAY
    nop
    nop
    nop
.endm

.macro SAVE_EXCEPTION

   push {r5}
   push {r0}
   ldr r5,=threads
   ldr r0,=currentP
   ldr r0, [r0]
   cmp r0,#0
   beq 1f
   add r5, r5, #CTXT_SIZE
1:
   pop {r0}

   @Save all registers
   str r0, [r5]         @ store r1 register
   str r1, [r5,#4]      @ store r2 register
   str r2, [r5,#8]      @ store r3 register
   str r3, [r5,#12]     @ store r4 register
   str r4, [r5,#16]     @ store r6 register

   str r6, [r5,#24]     @ store r7 register
   str r7, [r5,#28]     @ store r8 register
   str r8, [r5,#32]     @ store r9 register
   str r9, [r5,#36]    @ store r10 register
   str r10, [r5,#40]    @ store r11 register FP register
   str r11, [r5,#44]    @ store r12 register
   str r12, [r5,#48]    @ store r12 register


   @Save r13/sp
   push {r0}
   mov r0, #0
   add r0, r5, #52
   stm r0, {sp}^       @ [r5,#52] <- sp_usr (r13)
   
   @Save r14/lr
   mov r0, #0
   add r0, r5, #56
   stm r0, {lr}^       @ [r5,#56] <- lr_usr (r14)


   @Save r15/pc
   str lr, [r5,#60]

   @Save PSR
   push {r6}
   mrs r6,spsr          @ Read PSR from caller
   str r6, [r5,#64]
   pop {r6}

   @Save USR r5 register in his CTXT
   @push {r1}
   @mov r1, r5
   @ldr r5, [sp], #4
   @str r5, [r1,#20]     @ store r5 register
   @pop {r1}

   pop {r5}


.endm

.macro SAVE_CTXT_IRQ

   push {r5}
   push {r0}
   ldr r5,=threads
   ldr r0,=currentP
   ldr r0, [r0]
   cmp r0,#0
   beq 1f
   add r5, r5, #CTXT_SIZE
1:
   pop {r0}

   @Save all registers
   str r0, [r5]         @ store r1 register
   str r1, [r5,#4]      @ store r2 register
   str r2, [r5,#8]      @ store r3 register
   str r3, [r5,#12]     @ store r4 register
   str r4, [r5,#16]     @ store r6 register

   str r6, [r5,#24]     @ store r7 register
   str r7, [r5,#28]     @ store r8 register
   str r8, [r5,#32]     @ store r9 register
   str r9, [r5,#36]    @ store r10 register
   str r10, [r5,#40]    @ store r11 register FP register
   str r11, [r5,#44]    @ store r12 register
   str r12, [r5,#48]    @ store r12 register


   @Save r13/sp
   push {r0}
   mov r0, #0
   add r0, r5, #52
   stm r0, {sp}^       @ [r5,#52] <- sp_usr (r13)
   
   @Save r14/lr
   mov r0, #0
   add r0, r5, #56
   stm r0, {lr}^       @ [r5,#52] <- lr_usr (r14)
   pop {r0}

   @Save r15/pc
   str lr, [r5,#60]

   @Save PSR
   push {r6}
   mrs r6,spsr          @ Read PSR from caller
   str r6, [r5,#64]
   pop {r6}

   @Save USR r5 register in his CTXT
   @push {r1}
   @mov r1, r5
   @ldr r5, [sp], #4
   @str r5, [r1,#20]     @ store r5 register
   @pop {r1}

   pop {r5}


.endm



.macro SAVE_REGISTERS

   push {r5}
   push {r0}
   ldr r5,=threads
   ldr r0,=currentP
   ldr r0, [r0]
   cmp r0,#0
   beq 1f
   add r5, r5, #CTXT_SIZE
1:
   pop {r0}

   @Save all registers
   str r0, [r5]         @ store r1 register
   str r1, [r5,#4]      @ store r2 register
   str r2, [r5,#8]      @ store r3 register
   str r3, [r5,#12]     @ store r4 register
   str r4, [r5,#16]     @ store r6 register

   str r6, [r5,#24]     @ store r7 register
   str r7, [r5,#28]     @ store r8 register
   str r8, [r5,#32]     @ store r9 register
   str r9, [r5,#36]     @ store r10 register
   str r10, [r5,#40]    @ store r11 register FP register
   str r11, [r5,#44]    @ store r12 register
   str r12, [r5,#48]    @ store r12 register

   @Save r13/sp
   push {r0}
   mov r0, #0
   add r0, r5, #52
   stm  r0, {sp}^       @ [r5,#52] <- sp_usr (r13)
   
   @Save r14/lr
   mov r0, #0
   add r0, r5, #56
   stm  r0, {lr}^       @ [r5,#56] <- lr_usr (r14)
   pop {r0}

   @Save r15/pc
   str lr, [r5,#60]

   @Save PSR
   push {r6}
   mrs r6,spsr          @ Read PSR from caller
   str r6, [r5,#64]
   pop {r6}

   @Save USR r5 register in his CTXT
   @push {r1}
   @mov r1, r5
   @ldr r5, [sp], #4
   @str r5, [r1,#20]     @ store r5 register
   @pop {r1}
  
   pop {r5}
.endm



.section .nexx.traptab, "ax"
.global _start, _halt_system, irq

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
.word undefined

_swi:
.word swi

_prefetchAbort:
.word prefetchAbort

_dataAbort:
.word dataAbort

_reserved:
.word reserved

_irq:
.word irq

_fiq:
.word fiq



.align 5
reset:

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
    bl Setup
    b .



.align 5
_halt_system:
    b .

/* 
 *  jump to handlers 
 */

.align  5
irq:
    ldr sp,=_estack

    SUB lr, lr, #4

 
    SAVE_CTXT_IRQ

    b  do_irq
    b .

.align  5
swi:
    @Save Context 
    @ r0 <- Hypercall number
    @ r1 <- 1 param. (ptr string)

    ldr sp,=_estack

    SAVE_REGISTERS
   
    @ r0 <- HyperCall number
    @ r1 <- First hypercall Param 

    b do_swi
    b .

.align 5
undefined:
    
    ldr sp,=_estack

    SAVE_EXCEPTION

    b do_undefined
    b .
    @ msr cpsr, #MODE_USR

.align 5
prefetchAbort:
    ldr sp,=_estack

    SUB lr, lr, #4

    SAVE_EXCEPTION

    b do_prefetchAbort
    b .

.align 5
dataAbort:
    ldr sp,=_estack

    SUB lr, lr, #8

    SAVE_EXCEPTION

    b do_dataAbort
    b .

.align 5
reserved:

    ldr sp,=_estack
    SAVE_EXCEPTION
    b do_reserved
    b .

.align 5
fiq:

    ldr sp,=_estack
    SAVE_EXCEPTION
    b do_fiq
    b .


.section ".bss"
.align 8

_stack:
    .zero (1024*16)
_estack:

.previous
