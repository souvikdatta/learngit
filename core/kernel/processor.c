/*
 * FILE: processor.c
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

#define C1_DC       (1<<2)      /* dcache off/on */
#define C1_IC       (1<<12)     /* icache off/on */

#if 0
nexx_u32_t readCpuId(void)
{
    nexx_s32_t val;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c0, c0, 0   @ read control reg\n"
        : "=r" (val)
        :
        : "memory");

    return val;
}

nexx_u32_t readCpuIdCacheType(void){
}
#endif

nexx_u32_t read_p15_c1(void)
{
    nexx_s32_t val;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c1, c0, 0   @ read control reg\n"
        : "=r" (val)
        :
        : "memory");

    return val;
}

void write_p15_c1(nexx_u32_t val)
{
    __asm__ __volatile__(
        "mcr    p15, 0, %0, c1, c0, 0   @ write it back\n"
        :
        : "r" (val)
        : "memory");
    asm("nop");
    asm("nop");
    asm("nop");

}


nexx_u32_t read_p15_c3(void)
{
    nexx_s32_t val;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c3, c0, 0   @ read control reg\n"
        : "=r" (val)
        :
        : "memory");

//    xprintf ("p15/c3 is = %08lx\n", val);
    
    return val;
}

void write_p15_c3(nexx_u32_t val)
{
  //  xprintf ("write %08lx to p15/c1\n", val);

    __asm__ __volatile__(
        "mcr    p15, 0, %0, c3, c0, 0   @ write it back\n"
        :
        : "r" (val)
        : "memory");

    read_p15_c3();
}

nexx_u32_t readDFSR(void)
{
    nexx_s32_t val;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c5, c0, 0   @ read control reg\n"
        : "=r" (val)
        :
        : "memory");

    return val;
}

nexx_u32_t readIFSR(void)
{
    nexx_s32_t val;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c5, c0, 1   @ read control reg\n"
        : "=r" (val)
        :
        : "memory");

    return val;
}

nexx_u32_t readFAR(void)
{
    nexx_s32_t val;

    __asm__ __volatile__(
        "mrc    p15, 0, %0, c6, c0, 0   @ read control reg\n"
        : "=r" (val)
        :
        : "memory");

    return val;
}


void enable_interrupts (void)
{
    unsigned long temp;
    __asm__ __volatile__("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp)
                 :
                 : "memory");
}


/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
    unsigned long old,temp;
    __asm__ __volatile__("mrs %0, cpsr\n"
                 "orr %1, %0, #0xc0\n"
                 "msr cpsr_c, %1"
                 : "=r" (old), "=r" (temp)
                 :
                 : "memory");
    return (old & 0x80) == 0;
}

void cp_delay (void)
{
    volatile nexx_s32_t i;
    /* copro seems to need some delay between reading and writing */
    for (i = 0; i < 100; i++);
}


void forceVicIrq(nexx_s32_t nIrq){

    IO_WRITE(0x10140018, (1<<nIrq));
}
void enableVicIrq(nexx_s32_t nIrq){

    IO_WRITE(0x10140010, (1<<nIrq));
}

void disableVicIrq(nexx_s32_t nIrq){

    IO_WRITE(0x10140014, (1<<nIrq));
}

void dcacheEnable (void)
{
    nexx_u32_t reg;

    reg = read_p15_c1();
    cp_delay ();
    write_p15_c1(reg | C1_DC);
}

void dcacheDisable (void)
{
    nexx_u32_t reg;

    reg = read_p15_c1();
    cp_delay();
    write_p15_c1(reg & ~C1_DC);
}

void icacheEnable (void)
{
    nexx_u32_t reg;

    reg = read_p15_c1 ();
    cp_delay ();
    write_p15_c1 (reg | C1_IC);
}

void icacheDisable (void)
{
    nexx_u32_t reg;

    reg = read_p15_c1();
    cp_delay();
    write_p15_c1(reg & ~C1_IC);
}

int icacheStatus (void)
{
    return (read_p15_c1 () & C1_IC) != 0;
}
void setDomain(nexx_s32_t domain, nexx_u8_t val)
{
    nexx_u32_t reg;

    val=val<<(domain<<1);
    reg = read_p15_c3();
    cp_delay ();
    write_p15_c3(val);
}

nexx_addr getDomainReg(void)
{
    return read_p15_c3();
}

int cleanup_before_linux (void)
{
    unsigned long i;
    
    disable_interrupts ();
    
    /* turn off I/D-cache */
    asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));
    i &= ~(C1_DC | C1_IC);
    asm ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));

    /* flush I/D-cache */
    i = 0;
    asm ("mcr p15, 0, %0, c7, c7, 0": :"r" (i));

    return (0);
}
