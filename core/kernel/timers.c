/*
 * FILE: timers.c
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

typedef unsigned long       ulong;

#define CFG_TIMERBASE           0x101E2000  /* Timer 0 and 1 base */
    #define TIMER_RIS           0x10
    #define TIMER_MIS           0x14

#define TIMER_CFG_HZ                  (1000000)

#define TIMER_LOAD_VAL 0xffffffff

/* macro to read the 32 bit timer */
#define READ_TIMER (*(volatile int *)(CFG_TIMERBASE+4))

/* Timer Configuration */
#define TIMER_PERIODIC  0xE3 //  one-shot/32bit/interrupt
    #define ONE_SHOT    0x01
    #define WRAP_MODE   0x00
    #define COUNT_32    0x10
    #define COUNT_16    0x00
    #define TIMER_PRE   0x00
    #define INT_ENABLE  0x20
    #define PERIODIC    0x40
    #define TIMER_EN    0x80



static ulong timestamp;
static ulong lastdec;

void reset_timer_masked (void)
{
    /* reset time */
    lastdec = READ_TIMER;  /* capure current decrementer value time */
    timestamp = 0;         /* start "advancing" time stamp from 0 */
}



int timer_init (void)
{
    //xprintf("Timer antes %x\n",  *(volatile int *)(CFG_TIMERBASE + 8));
    *(volatile int *)(CFG_TIMERBASE + 0) = TIMER_LOAD_VAL;  /* TimerLoad */
    *(volatile int *)(CFG_TIMERBASE + 8) = TIMER_PERIODIC;

    //xprintf("Timer despues %x\n",  *(volatile int *)(CFG_TIMERBASE + 8));
    /* init the timestamp and lastdec value */
    reset_timer_masked();

    return 0;
}

void setHwTimer(ulong ticks){
    *(volatile int *)(CFG_TIMERBASE + 0) = ticks;  /* TimerLoad */
    *(volatile int *)(CFG_TIMERBASE + 8) = TIMER_PERIODIC;
}

/* setTimer inuseconds  */
void setTimer(unsigned long usec)
{
    ulong tmo, tmp;

    if(usec >= 1000){       /* if "big" number, spread normalization to seconds */
        tmo = usec / 1000;  /* start to normalize for usec to ticks per sec */
        tmo *= TIMER_CFG_HZ;      /* find number of "ticks" to wait to achieve target */
        tmo /= 1000;        /* finish normalize. */
    }else{              /* else small number, don't kill it prior to HZ multiply */
        tmo = usec * TIMER_CFG_HZ;
        tmo /= (1000*1000);
    }

    setHwTimer(tmo);
}

void setTimerLoadReg(unsigned long val){

    *(volatile int *)(CFG_TIMERBASE + 0) = val;  /* TimerLoad */
}


ulong get_timer_reg(void){

    return READ_TIMER;

}
ulong get_timer_masked (void)
{
    ulong now = READ_TIMER;     /* current tick value */

    if (lastdec >= now){        /* normal mode (non roll) */
        timestamp += lastdec - now; /* move stamp fordward with absoulte diff ticks */
        //xprintf("Normal\n");
    }
    else {
        timestamp += lastdec + TIMER_LOAD_VAL - now;
        //xprintf("[WARNING] Overflow\n");
    }
    
    lastdec = now;

    return timestamp;
}


ulong get_timer (ulong base)
{
        return get_timer_masked () - base;
}

/* delay x useconds AND perserve advance timstamp value */
void udelay (unsigned long usec)
{
    ulong tmo, tmp;

    if(usec >= 1000){       /* if "big" number, spread normalization to seconds */
        tmo = usec / 1000;  /* start to normalize for usec to ticks per sec */
        tmo *= TIMER_CFG_HZ;      /* find number of "ticks" to wait to achieve target */
        tmo /= 1000;        /* finish normalize. */
    }else{              /* else small number, don't kill it prior to HZ multiply */
        tmo = usec * TIMER_CFG_HZ;
        tmo /= (1000*1000);
    }

    tmp = get_timer (0);        /* get current timestamp */
    if( (tmo + tmp + 1) < tmp ) /* if setting this fordward will roll time stamp */
        reset_timer_masked ();  /* reset "advancing" timestamp to 0, set lastdec value */
    else
        tmo += tmp;     /* else, set advancing stamp wake up time */

    while (get_timer_masked () < tmo)/* loop till event */
        /*NOP*/;
}

void timerIrqHandler(void){
    /* Set timer to 1 sec */
    setTimer(1000*1000); // 1sec
}

