/*
 * FILE: loader.c
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
#include "stdc.h"
#include "../../autoconf.h"
   
extern nexx_u32_t containerPtr;
nexx_u32_t *containerAddr=&containerPtr;

#define ADDR2PTR(x) ((void *)((nexx_addr)x))

/* Hardcoded for now */
#define nexxEntryPoint (0x600000+0x8000)

#define NEXX_PHYSICAL (0x600000+0x8000)
#define NEXX_OFFSET_CONTAINER 0x0
#define NEXX_SIZE  AUTOCONF_NEXX_SIZE

#define P1_PHYSICAL 0xA00000
#define P1_OFFSET_CONTAINER NEXX_SIZE
#define P1_SIZE AUTOCONF_P1_SIZE

#define P2_PHYSICAL 0xB00000
#define P2_OFFSET_CONTAINER (NEXX_SIZE+P1_SIZE)
#define P2_SIZE AUTOCONF_P2_SIZE

#define LINUX_PHYSICAL 0xC08000
#define LINUX_OFFSET_CONTAINER (NEXX_SIZE+P1_SIZE+P2_SIZE)
#define LINUX_SIZE AUTOCONF_LINUX_SIZE

#if 0
#define INITRD_PHYSICAL 0x800000
#define INITRD_OFFSET_CONTAINER (NEXX_SIZE+P1_SIZE+P2_SIZE+LINUX_SIZE)
#define INITRD_SIZE  AUTOCONF_INITRD_SIZE
#endif

void loadPartitions(){
   /* Load Partition 1 */
   nexx_u8_t *PartitionPhysicalAddr=(nexx_u8_t *)P1_PHYSICAL;
   nexx_u8_t *PartitionContainerAddr=(nexx_u8_t *)(containerAddr+P1_OFFSET_CONTAINER/4);

   xprintf("[Loader] P1 [0x%x]--[0x%x]\n", PartitionPhysicalAddr, PartitionPhysicalAddr+P1_SIZE);
   memcpy(PartitionPhysicalAddr, PartitionContainerAddr, P1_SIZE);

   /* Load Partition 2 */
   PartitionPhysicalAddr=(nexx_u8_t *)P2_PHYSICAL;
   PartitionContainerAddr=(nexx_u8_t *)(containerAddr+P2_OFFSET_CONTAINER/4);

   xprintf("[Loader] P2 [0x%x]--[0x%x]\n", PartitionPhysicalAddr, PartitionPhysicalAddr+P2_SIZE);
   memcpy(PartitionPhysicalAddr, PartitionContainerAddr, P2_SIZE);

}
#ifdef IS_LINUX_PARTITION
void loadLinux(void){

    /* Load Partition 1 */
   nexx_u8_t *LinuxPhysicalAddr=(nexx_u8_t *)LINUX_PHYSICAL;
   nexx_u8_t *LinuxContainerAddr=(nexx_u8_t *)(containerAddr+LINUX_OFFSET_CONTAINER/4);

   xprintf("[Loader] Linux [0x%x]--[0x%x]\n", LinuxPhysicalAddr, LinuxPhysicalAddr+LINUX_SIZE);
   memcpy(LinuxPhysicalAddr, LinuxContainerAddr, LINUX_SIZE);

}
#endif

void loadNexHypervisor(void){
   nexx_u8_t *nexxPhysicalAddr=(nexx_u8_t *)NEXX_PHYSICAL;
   nexx_u8_t *nexxContainerAddr=(nexx_u8_t *)(containerAddr+NEXX_OFFSET_CONTAINER/4);

   xprintf("[Loader] NEXX [0x%x]--[0x%x]\n", nexxPhysicalAddr, nexxPhysicalAddr+NEXX_SIZE);
   memcpy(nexxPhysicalAddr,nexxContainerAddr, NEXX_SIZE);
}

#if 0
void loadInitrd(void){
   /* Load initrd */
   nexx_u8_t *InitrdPhysicalAddr=(nexx_u8_t *)INITRD_PHYSICAL;
   nexx_u8_t *InitrdContainerAddr=(nexx_u8_t *)(containerAddr+INITRD_OFFSET_CONTAINER/4);

   xprintf("[Loader] Initrd Linux [0x%x]--[0x%x]\n", InitrdPhysicalAddr, InitrdPhysicalAddr+LINUX_SIZE);
   memcpy(InitrdPhysicalAddr, InitrdContainerAddr, INITRD_SIZE);
}
#endif


void BUG(nexx_u8_t *msg){
    xprintf("%s\n",msg);
    while(1);

}

void checkRamArea(void){
   nexx_u8_t hash[16];
   nexx_s32_t res;
   nexx_u8_t *nexxPhysicalAddr=(nexx_u8_t *)NEXX_PHYSICAL;

   /* Check NEXX hypervisor */
   xprintf("NEXX_MD5(%x , %d, hash);\n", nexxPhysicalAddr , NEXX_SIZE);
   res = NEXX_MD5(nexxPhysicalAddr , NEXX_SIZE, hash);
   if(res){
      BUG("Check NEXX hypervisor Integrity failed !!!!!\n");
   }

   printMD5("RAM MD5 [", hash);
}

void loader(void){
    extern nexx_u32_t _sLoaderPtr;
    extern nexx_u32_t _eLoaderPtr;
    nexx_u32_t *sLoader=&_sLoaderPtr;
    nexx_u32_t *eLoader=&_eLoaderPtr;

    xprintf("[Loader] PackedMemory used: [0x%x]--[0x%x]\n",sLoader,eLoader);
    if((nexx_addr)eLoader>(nexxEntryPoint-0x8000)){
        xprintf("[Loader] Overlaping memory area !!!\n");
        xprintf("[Loader] Packet is solaped with NEXX hypervisor\n");
        while(1);
    }


    xprintf("[Loader] Loading NEXX hypervisor ...\n");
    loadNexHypervisor();
    

    xprintf("[Loader] Loading Partitions ...\n");
    loadPartitions();
#ifdef IS_LINUX_PARTITION
    xprintf("[Loader] Linux ...\n");
    loadLinux();
#endif

//    xprintf("[Loader] Initrd for linux ...\n");
//    loadInitrd();

    checkRamArea(); // Check if binaries were loaded correctly into memory

    xprintf("[Loader] Starting NEXX at 0x%x...\n",nexxEntryPoint);
    ((void (*)(void))ADDR2PTR(nexxEntryPoint))();


    xprintf("[Loader] Panic: This code should never be reached !!\n");
    while(1);
}
