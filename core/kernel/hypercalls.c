/*
 * FILE: hypercalls.c
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
#include "hypercalls.h"
#include "processor.h"

#define NR_HYPERCALLS   2
nexx_addr hypercallPtr2Functions[NR_HYPERCALLS];

void NexWriteConsole(nexx_s8_t *ptr, nexx_s32_t n){
   nexx_s32_t i;
   for(i=0;i<n;i++){
      xputchar(ptr[i]);
   }
}

void NexReadCpuId(void){
   setUserR0(read_cpuid(CPUID_ID));
}

void NexReadCpuIdCacheType(void){
    setUserR0(read_cpuid(CPUID_CACHETYPE));
}

void NexReadCr(void){
    setUserR0(get_cr());
}

void NexFlushPmdEntry(unsigned long *pmd){
    //asm("mcr    p15, 0, %0, c7, c10, 1  @ flush_pmd"
    //        : : "r" (pmd) : "cc");
    ;
}

void NEXXDrainWB(void){
    __asm__ __volatile__ ("mcr p15, 0, r0, c7, c10, 4");
}

void NEXXFmrx_cr8(void){
    nexx_u32_t __v;            
    asm("mrc p10, 7, %0, cr8, cr0, 0 @ fmrx   %0, "
        : "=r" (__v) : : "cc");
    setUserR0(__v);
}

void NEXXFmxr_cr8(nexx_u32_t val){
    //xprintf("Usant NEXXFmxr !!!!!!!!!!!\n");
    asm("mcr p10, 7, %0, cr8, cr0, 0 @ fmxr"
       : : "r" (val) : "cc");
}
