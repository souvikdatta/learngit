/*
 * FILE: irqs.c
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
#include "io.h"
#include "pcb.h"
#include "hypercalls.h"

#define INT_BASE    0x10140000

extern nexx_s32_t currentP;
extern cpuCtxt_t threads[2];

void printRegs(cpuCtxt_t *currentCpu){
   xprintf("DFSR (0x%x) ", readDFSR());
   xprintf("IFSR (0x%x)\n", readIFSR());
   xprintf("FAR (0x%x)\n", readFAR());

   xprintf("r0 (0x%x)\n", currentCpu->r0);
   xprintf("r1 (0x%x)\n", currentCpu->r1);
   xprintf("r2 (0x%x)\n", currentCpu->r2);
   xprintf("r3 (0x%x)\n", currentCpu->r3);
   xprintf("r4 (0x%x)\n", currentCpu->r4);
   xprintf("r5 (0x%x)\n", currentCpu->r5);
   xprintf("r6 (0x%x)\n", currentCpu->r6);
   xprintf("r7 (0x%x)\n", currentCpu->r7);
   xprintf("r8 (0x%x)\n", currentCpu->r8);
   xprintf("r9 (0x%x)\n", currentCpu->r9);
   xprintf("r10 (0x%x)\n", currentCpu->r10);
   xprintf("r11/fp (0x%x)\n", currentCpu->fp);
   xprintf("r12 (0x%x)\n", currentCpu->r12);
   xprintf("r13/sp (0x%x)\n", currentCpu->sp);
   xprintf("r14/lr (0x%x)\n", currentCpu->lr);
   xprintf("r15/pc (0x%x)\n", currentCpu->pc);
   xprintf("psr (0x%x)\n", currentCpu->psr);

}


#if 0
void print_int_regs(void)
{

    xprintf("IRQ Status Reg [%x]\n", IO_READ(INT_BASE+0x0));
   // xprintf("FIQ Status Reg [%x]\n", IO_READ(INT_BASE+0x4));
    xprintf("RAW Status Reg [%x]\n", IO_READ(INT_BASE+0x8));
    xprintf("Int. Sel.  Reg [%x]\n", IO_READ(INT_BASE+0xC));
    xprintf("Int. Ena.  Reg [%x]\n", IO_READ(INT_BASE+0x10));
    xprintf("Soft Int. Reg [%x]\n", IO_READ(INT_BASE+0x18));
}

#endif

void irq_init(void){
#define VA_VIC_BASE 0x10140000
#define VIC_VECT_ADDR           0x30
#define VIC_VECT_CNTL0          0x200   /* 0 to 15 */
#define VIC_VECT_CNTL_ENABLE        (1 << 5)
#define VIC_DEF_VECT_ADDR       0x34 

#define VIC_INT_SELECT          0x0c    /* 1 = FIQ, 0 = IRQ */
#define VIC_INT_ENABLE          0x10    /* 1 = enable, 0 = disable */
#define VIC_INT_ENABLE_CLEAR        0x14 
#define VIC_IRQ_STATUS          0x00
#define VIC_ITCR            0x300   /* VIC test control register */
#define VIC_INT_SOFT_CLEAR      0x1c



#define VA_SIC_BASE 0x10003000
#define SIC_IRQ_ENABLE_CLEAR            0x0C

    /* Disable all interrupts initially. */
    IO_WRITE(VA_VIC_BASE + VIC_INT_SELECT, 0);
    IO_WRITE(VA_VIC_BASE + VIC_INT_ENABLE, 0);
    IO_WRITE(VA_VIC_BASE + VIC_INT_ENABLE_CLEAR, ~0);
    IO_WRITE(VA_VIC_BASE + VIC_IRQ_STATUS, 0);
    IO_WRITE(VA_VIC_BASE + VIC_ITCR, 0);
    IO_WRITE(VA_VIC_BASE + VIC_INT_SOFT_CLEAR, ~0);

    nexx_s32_t i;
    IO_WRITE(VA_VIC_BASE + VIC_VECT_ADDR,0);
    for (i = 0; i < 19; i++) {
        unsigned int value;
        value = IO_READ(VA_VIC_BASE + VIC_VECT_ADDR);
        IO_WRITE(VA_VIC_BASE + VIC_VECT_ADDR,value);
    }
    for (i = 0; i < 16; i++) {
        IO_WRITE(VA_VIC_BASE + VIC_VECT_CNTL0 + (i * 4),VIC_VECT_CNTL_ENABLE | i);
    }

    IO_WRITE(VA_VIC_BASE + VIC_DEF_VECT_ADDR,32);

    IO_WRITE(VA_SIC_BASE + SIC_IRQ_ENABLE_CLEAR,0);
}

void clearIrqs(void)
{
    #define VIC_PL190_VECT_ADDR     0x30    /* PL190 only */
    unsigned int i;

    IO_WRITE(0, VA_VIC_BASE + VIC_PL190_VECT_ADDR);
    for (i = 0; i < 19; i++) {
        unsigned int value;

        value = IO_READ(VA_VIC_BASE + VIC_PL190_VECT_ADDR);
        IO_WRITE(value, VA_VIC_BASE + VIC_PL190_VECT_ADDR);
    }


}
void unknowIrqHandler(){
    xprintf("[CORE] Unexxpected Irq !!!\n");
}
void unknowSwiHandler(nexx_s32_t nHyperCall){
    xprintf("[CORE] Unexxpected HyperCall %d!!!\n",nHyperCall);
}

/* Handlers */
void do_irq(void){
    nexx_s32_t irq = IO_READ(INT_BASE+0x0);
    
    /* Clear IRQ */
    IO_WRITE(0x101E2000+0xc, 0x0);

    switch(irq){
        case (1<<4):
            xprintf("\n[NEXX-LOG] timerIrqHandler(). Context switch ... \n\n");
            timerIrqHandler();
        break;
        default:
            unknowIrqHandler();
        break;
    }

    scheduler();
    
}

void do_swi(nexx_s32_t nHyperCall, void *p1, void *p2){

    switch(nHyperCall&0x000000ff){
        case (0):
           NexWriteConsole(p1, ((nexx_s32_t)p2));
        break;
        case (1):
           NexReadCpuId();
        break;
        case (2):
           NexReadCpuIdCacheType();
        break;
        case (3):
           NexReadCr();
        break;
        case (4):
           NexFlushPmdEntry();
        break;
        case (5):
           NEXXDrainWB();
        break;
        case (6):
           NEXXFmrx_cr8();
        break;
        case(7):
           NEXXFmxr_cr8(((nexx_u32_t)p1));
        break;
        default:
            unknowSwiHandler(nHyperCall);
        break;
    }

    runCurrentPartition();

}

void printUserRegs(void){
    
    cpuCtxt_t *currentCpu;
    currentCpu=&threads[currentP];
//    xprintf("Print regs of partition (%d)\n", currentP);
    //at [0x%x]\n", currentP, currentCpu);
    printRegs(currentCpu);

}

/* Other irqs/exceptions */
void stopUnexxpected(nexx_s8_t *msg){
    xprintf("[NEXX Fatal]\n");
    printUserRegs();
    xprintf("[CORE] *** %s *** Error !!!!!\n",msg);
    xprintf("[CORE] Stoping Nex Hypervisor\n");
    while(1);
}



void do_undefined(void){
    stopUnexxpected("undefined");
}
void do_prefetchAbort(void){
    stopUnexxpected("prefetchAbort");
}
void do_dataAbort(void){
    //printUserRegs();
    //NEXX_mmap(readFAR());
    stopUnexxpected("dataAbort");
}
void do_reserved(void){
    stopUnexxpected("reserved");
}
void do_fiq(void){
    stopUnexxpected("fiq");
}



