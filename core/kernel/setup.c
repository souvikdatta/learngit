/*
 * FILE: setup.c
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
#include "kthread.h"
#include "pcb.h"
#include "stdc.h"
#include "processor.h"
#include "./../../autoconf2.h"

#define ONE_SEC_IN_US    (1000*1000) // 500 ms
extern cpuCtxt_t threads[2];

void IdleTask(void) {
    xprintf("IdleTask()\n");
    nexx_s32_t i=0;
    while(1) {
        asm("nop");
    }
}



#ifndef IS_LINUX_PARTITION
nexx_addr getPartPcReg(nexx_s32_t idPart){
    if(idPart==0)
        return (nexx_addr)(START_P0);
    
    if(idPart==1)
        return (nexx_addr)(START_P1);
}

nexx_addr getPartTbr(nexx_s32_t idPart){
    
    if(idPart==0)
        return (nexx_addr)(START_P0+0x100000-0x4000);
    
    if(idPart==1)
        return (nexx_addr)(START_P1+0x100000-0x4000);
}
#endif

void initCtxtLinux(nexx_s32_t idPartition){
    
    cpuCtxt_t *currentCpu;
    currentCpu=&threads[idPartition];

    currentCpu->r0=0;
    currentCpu->r1=0;
    currentCpu->r2=0;
    currentCpu->r3=0;
    currentCpu->r4=0;
    currentCpu->r5=0;
    currentCpu->r6=0;
    currentCpu->r7=0;
    currentCpu->r8=0;
    currentCpu->r9=0;
    currentCpu->r10=0;
    currentCpu->fp=0;
    currentCpu->r12=0;

    currentCpu->sp=0;
    currentCpu->lr=0;
    currentCpu->pc=0xc0008000; // Because the mapping is done before
    //currentCpu->pc=AUTOCONF_LINUX_START; // start_kernel Because the mapping is done before

//    currentCpu->psr=0x400001d3; // SVC to test linux
    
    currentCpu->tbr=0xc04000;

}

#ifndef IS_LINUX_PARTITION
void initCtxtPartition(nexx_s32_t idPartition){
    cpuCtxt_t *currentCpu;
    currentCpu=&threads[idPartition];

    currentCpu->r0=0;
    currentCpu->r1=0;
    currentCpu->r2=0;
    currentCpu->r3=0;
    currentCpu->r4=0;
    currentCpu->r5=0;
    currentCpu->r6=0;
    currentCpu->r7=0;
    currentCpu->r8=0;
    currentCpu->r9=0;
    currentCpu->r10=0;
    currentCpu->fp=0;
    currentCpu->r12=0;

    currentCpu->sp=0;
    currentCpu->lr=0;
    currentCpu->pc=getPartPcReg(idPartition);

    //currentCpu->psr=0x400001d3; // SVC to test linux

    currentCpu->tbr=getPartTbr(idPartition);

}
#endif

#ifndef IS_LINUX_PARTITION
void initPartitions(){
    nexx_s32_t nPartitions=2;
    nexx_s32_t i;

    for(i=0; i<nPartitions;i++){
        initCtxtPartition(i);
    }
}
#endif 

nexx_addr read_mem(nexx_addr addr){
    nexx_u32_t *ptr=(nexx_u32_t *)addr;
    xprintf("0x%x -> 0x%x\n", addr, *ptr);
    return *ptr;
}

nexx_addr emulate_walk(nexx_addr tbr, nexx_addr addr){
    nexx_addr addrLevel1;
    nexx_addr addrLevel2;
    
    nexx_addr entry1;
    nexx_addr entry2;
    nexx_addr phyAddr;

    xprintf("Emulte walk: VA: 0x%x\n",addr);
    addrLevel1=addr>>20;
    addrLevel2=(addr>>12)&0x1FF;
    tbr&=0xFFFFC000;

    /* Get 1 level content */
    entry1=read_mem(tbr|(addrLevel1<<2));
    if(entry1&0x00000002){
        xprintf("Section VA: (0x%x) --> Phy (0x%x) Entry[0x%x]\n", addr,  (entry1&0xfff00000)|(addr&0x000fffff), entry1);
        return (entry1&0xfff00000)|(addr&0x000fffff);
    }

    /* Get 2 level content */
    entry2=read_mem((entry1&0xFFFFFC00)|(addrLevel2<<2));

    /* Create physical address */
    phyAddr = (entry2&0xFFFFF000)|(addr&0x00000FFF);
    xprintf("VA Coarse: 0x%x Phy 0x%x\n", addr, phyAddr);

    return phyAddr;

}


void initMmu(void){
    #define NEXX_TBR 0x400000
    #define LINUX_TBR 0xc04000

    setDomain(0,0x1f);   
    writeTBR(NEXX_TBR);
    setMmuCtxt(0);

}

void startPeriodicTimer(void){
    /* For now just timer 1 each 1 sec */
    setTimer(1000*1000); // 1sec
    enableVicIrq(4);
}


void Setup(nexx_s32_t cpuId, kThread_t *idle) {
    //serial_init();
    xprintf("\n[NEXX] Starting NEXX Hypervisor ...\n");
    xprintf("[NEXX] CPU ID:%x\n", read_cpuid(0));
    xprintf("[NEXX] CR:%x\n", get_cr());
    //serial_init();
  

/* Hypervisor */
createPartitionSectionPGT(0x400000);

#ifndef IS_LINUX_PARTITION
    timer_init();

   // irq_init();
    //print_sizes();
    
    //createHypervisorPGT();
    //createPartitionSectionPGT(0x400000);
    

    /* P1 */
    createPartitionSectionPGT(0xA00000+0x100000-0x4000);
    /* P2 */
    createPartitionSectionPGT(0xB00000+0x100000-0x4000);
#else

    createLinuxSectionPGT(0xc04000);
#endif
    
    //emulate_walk(0x400000, 0x0);
//    emulate_walk(0xc04000, 0xc07ff0);

    //icacheEnable();
    //dcacheEnable();
    initMmu();
    mmuEnable();

#ifndef IS_LINUX_PARTITION
    startPeriodicTimer();
        
    initPartitions();

#else
    initCtxtLinux(0);

    setup_tags();
#endif

    xprintf("[NEXX] Calling scheduler()\n");
    scheduler();

    xprintf("This code is never reached !!!\n");
    while(1);
}
