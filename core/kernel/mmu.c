/*
 * FILE: mmu.c
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
#define C1_MMU      (1<<0)      /* mmu off/on */
#define C1_ALIGN    (1<<1)      /* alignment faults off/on */

nexx_addr seek;

nexx_s32_t getMmuCtxt(void)
{
    nexx_s32_t val;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c13, c0, 1"
        : "=r" (val)
        :
        : "memory");

    return val;
}


void setMmuCtxt(nexx_s32_t val)
{
#if DEBUG_MMU
    xprintf ("setMmuCtxt(%d)\n", val);
#endif

    __asm__ __volatile__(
        "mcr    p15, 0, %0, c13, c0, 1" 
        :
        : "r" (val)
        : "memory");
}

nexx_addr readTBR(void)
{
    nexx_s32_t addr;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c2, c0, 0"
        : "=r" (addr)
        :
        : "memory");

    return addr&0xFFFFC000;
}

void writeTBR(nexx_addr addr){
  //  xprintf("Writing tbr 0x%x\n", addr);
    addr&=0xFFFFC000;
    __asm__ __volatile__(
        "mcr    p15, 0, %0, c2, c0, 0"
        :
        : "r" (addr)
        : "memory");
 
}


void mmuEnable(void)
{
    write_p15_c1 (0x50101);
}

void mmuDisable(void)
{
    nexx_u32_t reg;

    reg = read_p15_c1 ();
    cp_delay();
    write_p15_c1 (reg & ~C1_MMU);
}

void mapMemoryOneTwoOne(nexx_addr start, nexx_s32_t nPages){
    #define SECTION_PGT 0x12

    #define DOMAIN_0  (0x0<<5) // Domain 3: This domain has 11 (manager)
    #define DOMAIN_2  (0x2<<5) // Domain 3: This domain has 10 bits (Client)
    
    #define AP_RW (0x3<<10) // R/W for all
    #define AP_R (0x2<<10) //  Read only
    #define AP_N (0x1<<10) //  No acces

    nexx_u32_t *startL1=(nexx_u32_t *)start;
    nexx_u32_t cPage;

    xprintf("[NEXX] PGT [0x%x]-[0x%x]\n", startL1, ((nexx_addr)startL1)+(4096*4));

    for(cPage=(start/(1024*1024));cPage<nPages;cPage++){
        *startL1=(nexx_addr)cPage<<20 | 0xc12;
        //*startL1=(nexx_addr)cPage<<20 | SECTION_PGT | DOMAIN_0 | AP_RW;
        startL1++;
    }
}
void createPartitionSectionPGT(nexx_addr entryL1){
    #define SECTION_PGT 0x12

    #define DOMAIN_0  (0x0<<5) // Domain 3: This domain has 11 (manager)
    #define DOMAIN_2  (0x2<<5) // Domain 3: This domain has 10 bits (Client)
    
    #define AP_RW (0x3<<10) // R/W for all
    #define AP_R (0x2<<10) //  Read only
    #define AP_N (0x1<<10) //  No acces

    nexx_u32_t *startL1=(nexx_u32_t *)entryL1;
    nexx_u32_t entryLevel1;

    xprintf("[NEXX] PGT [0x%x]-[0x%x]\n", startL1, ((nexx_addr)startL1)+(4096*4));

    for(entryLevel1=0;entryLevel1<4096;entryLevel1++){
        *startL1=(nexx_addr)entryLevel1<<20 | SECTION_PGT | DOMAIN_2 | AP_RW;
        //xprintf("TBR(0x%x) = [0x%x]\n", startL1, *startL1);
        startL1++;
    }
}
void createHypervisorCoarsCoarsePGT(void){
    #define COARSE_PGT 0x11
    
    nexx_u32_t *startL1=(nexx_u32_t *)0x400000;
    nexx_u32_t *startL2=(nexx_u32_t *)0x404000;
    nexx_u32_t entryLevel1;
    nexx_u32_t entryLevel2;

    xprintf("[NEXX] 1-Level PGT [0x%x]-[0x%x]\n", startL1, ((nexx_addr)startL1)+(4096*4));
    xprintf("[NEXX] 2-Level PGT [0x%x]-[0x%x]\n", startL2, ((nexx_addr)startL2)+(256*4096*4));

    for(entryLevel1=0;entryLevel1<4096;entryLevel1++){
        *startL1=((((nexx_addr)startL2)>>10)<<10)|COARSE_PGT;
        for(entryLevel2=0;entryLevel2<256;entryLevel2++){
            *startL2=((entryLevel1<<8)+entryLevel2)<<12|0xA;
            startL2++;
        }
        startL1++;
    }
}
//volatile nexx_addr seek;
nexx_addr memoryMap(nexx_addr startTbr, nexx_addr startVirAddr, nexx_addr startPhyAddr, nexx_s32_t nMB){
    #define SECTION_LINUX_PGT 0xc12
    nexx_s32_t i;
    nexx_addr sectionBaseAddr = (nexx_addr)(startPhyAddr>>20); // 0xC0
    nexx_u32_t *index = (nexx_u32_t *) (startTbr | (startVirAddr>>20)<<2);

//    xprintf("[NEXX] Linux MAP [0x%x]-[0x%x] -- TBR [0x%x]\n", startVirAddr, startVirAddr+(nMB*0x100000), startTbr);
//    xprintf("[NEXX] Linux MAP. Index used [%x]-[%x]\n", index, index+nMB);
    for(i=0;i<nMB;i++){
        *index=((sectionBaseAddr+i)<<20) | SECTION_LINUX_PGT;
        xprintf("TBR(0x%x) = [0x%x] | Vaddr(0x%x) || PhyAddr(0x%x)\n", index, ((sectionBaseAddr+i)<<20) | SECTION_LINUX_PGT, startVirAddr+(i*0x100000), startPhyAddr+(i*0x100000));
        index++;
    }

    return (startPhyAddr+(nMB*0x100000));

}
nexx_addr memoryMapMmuON(nexx_addr startTbr, nexx_addr startVirAddr, nexx_addr startPhyAddr, nexx_s32_t nMB){
    #define SECTION_LINUX_PGT 0xc12
    nexx_s32_t i;
    nexx_addr sectionBaseAddr = (nexx_addr)(startPhyAddr>>20); // 0xC0
    nexx_u32_t *index = (nexx_u32_t *) (startTbr | (startVirAddr>>20)<<2);

    index =(nexx_addr *) (((nexx_addr)index) - 0xc00000 + 0xc0008000); // physcal to virtual

//    xprintf("[NEXX] Linux MAP [0x%x]-[0x%x] in Index %x \n", startVirAddr, startVirAddr+(nMB*0x100000), index);
    for(i=0;i<nMB;i++){
        *index=((sectionBaseAddr+i)<<20) | SECTION_LINUX_PGT;
        xprintf("TBR(0x%x) = [0x%x]\n", index, ((sectionBaseAddr+i)<<20) | SECTION_LINUX_PGT);
        index++;
    }

    return (startPhyAddr+(nMB*0x100000));

}

void memoryMapAddr(nexx_addr startTbr, nexx_addr startVirAddr){
    seek = memoryMapMmuON(startTbr, startVirAddr, seek, 1);
}

void createLinuxSectionPGT(nexx_addr startTbr){
    #define SECTION_LINUX_PGT 0xc12

    #define DOMAIN_0  (0x0<<5) // Domain 3: This domain has 11 (Manager)

    #define START_PHYSICAL   0xc00000
    #define START_VIRTUAL  0xc0000000

    #define N_PAGES 10

    nexx_s32_t i;

    nexx_addr sectionBaseAddr = (nexx_addr)(START_PHYSICAL>>20); // 0xC0
    nexx_u32_t *index= (nexx_u32_t *) (startTbr | (START_VIRTUAL>>20)<<2);

    /* MAP LINUX */
    xprintf("------------ START\n");
    seek=memoryMap(startTbr, START_VIRTUAL, START_PHYSICAL, 64);
    xprintf("------------\n");
    seek=memoryMap(startTbr, 0xffff0ff0, seek, 1);
    xprintf("------------\n");

   // seek=memoryMap(startTbr, 0xa0000190, seek, 1);

   // seek=memoryMap(startTbr, 0xe121f007, seek, 1);
    
    seek=memoryMap(startTbr, 0xc4800000, seek, 1);
    seek=memoryMap(startTbr, 0xc4c40000, seek, 1);
    xprintf("------------ END\n");

   /* MAP INITRD */
   seek=memoryMap(startTbr, 0xbff40000, seek, 1);

   /* MAP NEXX */
   memoryMap(startTbr, 0x0, 0x0, 10);


/*    sectionBaseAddr = 0x0;
    index= startTbr;
    for(i=0;i<10;i++){
        *index=((sectionBaseAddr+i)<<20) | SECTION_LINUX_PGT;
        xprintf("TBR(0x%x) = [0x%x]\n", index, ((sectionBaseAddr+i)<<20) | SECTION_LINUX_PGT);
        index++;
    }
*/
}
