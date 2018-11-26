/*
 * FILE: sched.c
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
#include "pcb.h"
#include "./../../autoconf2.h"

#define TO_STR(x) #x
#define cpuCtxt_t_OFFSET   (18*4)

cpuCtxt_t threads[2];
volatile nexx_s32_t currentP=-1;


#define RESUME_THREAD(cThread) \
   __asm__ __volatile__ (".global .Tbegin_jmp_partition\n\t" \
         ".Tbegin_jmp_partition:\n\t" \
         "mov r1, %0\n\t" \
         "ldr r0, [r1,#0]\n\t" \
         "ldr r2, [r1,#8]\n\t" \
         "ldr r3, [r1,#12]\n\t" \
         "ldr r4, [r1,#16]\n\t" \
         "ldr r5, [r1,#20]\n\t" \
         "ldr r6, [r1,#24]\n\t" \
         "ldr r7, [r1,#28]\n\t" \
         "ldr r8, [r1,#32]\n\t" \
         "ldr r9, [r1,#36]\n\t" \
         "ldr r10, [r1,#40]\n\t" \
         "ldr r11, [r1,#44]\n\t" \
         "ldr r12, [r1,#48]\n\t" \
         "msr CPSR_c, 0x10\n\t" \
         "ldr sp, [r1,#52]\n\t" \
         "ldr lr, [r1,#56]\n\t" \
         "ldr pc,[r1,#60]\n\t" \
         : : "r"(&cThread->r0) :)




int get_nexxt_id(){
#ifdef IS_LINUX_PARTITION
    return 0;
#endif

   if(currentP==-1)
       return 0;

   currentP++;

   if(currentP>1)
       currentP=0;

   return currentP;
}

void print_threads(cpuCtxt_t *currentCpu){
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

void run_partition(){
   cpuCtxt_t *currentCpu;

   currentCpu=&threads[currentP];
   //print_threads(currentCpu);

   writeTBR(currentCpu->tbr);
   RESUME_THREAD(currentCpu);
}

void runCurrentPartition()
{
    run_partition();
}

void scheduler(){

   int id;

   // Return 0 or 1 (test)
   id=get_nexxt_id();

   /* Update current Partition to be executed */
   currentP=id;
   //setMmuCtxt(id);
   xprintf("Run partition id:%d\n", currentP);
//   xprintf("PCB pointer %x\n", &threads[currentP]);
   run_partition();

}
