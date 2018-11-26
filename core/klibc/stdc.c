/*
 * FILE: stdc.c
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

#define SCRATCH 20	//32Bits go up to 4GB + 1 Byte for \0

//Spare some program space by making a comment of all not used format flag lines
#define USE_LONG	// %lx, %Lu and so on, else only 16 bit integer is allowed
//#define USE_OCTAL // %o, %O Octal output. Who needs this ?
#define USE_STRING			// %s, %S Strings as parameters
#define USE_CHAR	// %c, %C Chars as parameters
#define USE_INTEGER // %i, %I Remove this format flag. %d, %D does the same
#define USE_HEX		// %x, %X Hexadezimal output
#define USE_UPPERHEX	// %x, %X outputs A,B,C... else a,b,c...
#ifndef USE_HEX
# undef USE_UPPERHEX		// ;)
#endif
#define USE_UPPER // uncommenting this removes %C,%D,%I,%O,%S,%U,%X and %L..
												// only lowercase format flags are used
#define PADDING					//SPACE and ZERO padding


typedef struct Fprint Fprint;
struct Fprint {
    nexx_s32_t (*putc)(nexx_s32_t c, void *a);
    void *a;
};

void vrprintf(Fprint *fp, const char *fmt, va_list args) {
    nexx_u8_t scratch[SCRATCH];
    nexx_u8_t fmt_flag;
    nexx_u16_t base;
    nexx_u8_t *ptr;
    nexx_u8_t issigned=0;
    
#ifdef USE_LONG
// #warning "use long"
    nexx_u8_t islong=0;
    nexx_u8_t isvlong=0;
    nexx_u64_t u_val=0;
    nexx_s64_t s_val=0;
#else
    nexx_u32_t u_val=0;
    nexx_s32_t s_val=0;
#endif
    
    nexx_u8_t fill;
    nexx_u8_t width;
    
    for (;;){
	while ((fmt_flag = *(fmt++)) != '%'){			 // Until '%' or '\0' 
	    if (!fmt_flag){ return; }
	    if (fp->putc) fp->putc(fmt_flag, fp->a);
	}
	
	issigned=0; //default unsigned
	base = 10;
	
	fmt_flag = *fmt++; //get char after '%'
	
#ifdef PADDING
	width=0; //no formatting
	fill=0;	 //no formatting
	
	if(fmt_flag=='0' || fmt_flag==' ') //SPACE or ZERO padding	?
	{
	    fill=fmt_flag;
	    fmt_flag = *fmt++; //get char after padding char
	    while(fmt_flag>='0' && fmt_flag<='9')
	    {
		width = 10*width + (fmt_flag-'0');
		fmt_flag = *fmt++; //get char after width char
	    }
	}
#endif
	
#ifdef USE_LONG
	islong=0; //default int value
	isvlong=0;
#ifdef USE_UPPER
	if(fmt_flag=='l' || fmt_flag=='L') //Long value 
#else
        if(fmt_flag=='l') //Long value 
#endif
	{
	    islong=1;
	    fmt_flag = *fmt++; //get char after 'l' or 'L'
	    if (fmt_flag=='l'){
		isvlong = 1;
		fmt_flag = *fmt++; //get char after 'l' or 'L'
	    }
	}
#endif
	
	switch (fmt_flag)
	{
#ifdef USE_CHAR
	case 'c':
#ifdef USE_UPPER
	case 'C':
#endif
	    fmt_flag = va_arg(args, nexx_s32_t);
	    // no break -> run into default
#endif
	    
	default:
	    if (fp->putc)
		fp->putc(fmt_flag, fp->a);
	    continue;
	    
#ifdef USE_STRING
#ifdef USE_UPPER
	case 'S':
#endif
	case 's':
	    ptr = (nexx_u8_t *)va_arg(args, nexx_s8_t *);
	    while(*ptr) { if(fp->putc) fp->putc(*ptr, fp->a); ptr++; }
	    continue;
#endif
	    
#ifdef USE_OCTAL
	case 'o':
#ifdef USE_UPPER
	case 'O':
#endif
	    base = 8;
	    if (fp->putc) fp->putc('0', fp->a);
	    goto CONVERSION_LOOP;
#endif
	    
#ifdef USE_INTEGER //don't use %i, is same as %d
	case 'i':
#ifdef USE_UPPER
	case 'I':
#endif
#endif
	case 'd':
#ifdef USE_UPPER
	case 'D':
#endif
	    issigned=1;
	    // no break -> run into nexxt case
	case 'u':
#ifdef USE_UPPER
	case 'U':
#endif
	    
//don't insert some case below this if USE_HEX is undefined !
//or put			 goto CONVERSION_LOOP;	before nexxt case.
#ifdef USE_HEX
	    goto CONVERSION_LOOP;
	case 'x':
#ifdef USE_UPPER
	case 'X':
#endif
	    base = 16;
#endif
	    
	CONVERSION_LOOP:
	    
	    if(issigned) //Signed types
	    {
#ifdef USE_LONG
		if(isvlong) { s_val = va_arg(args, nexx_s64_t); }
		else if(islong) { s_val = va_arg(args, nexx_s32_t); }
		else { s_val = va_arg(args, nexx_s32_t); }
#else
		s_val = va_arg(args, nexx_s32_t);
#endif
		
		if(s_val < 0) //Value negativ ?
		{
		    s_val = - s_val; //Make it positiv
		    if (fp->putc) fp->putc('-', fp->a);		 //Output sign
		}
		
		if(!isvlong)
		    u_val = (nexx_u32_t)s_val;
		else
		    u_val = (nexx_u64_t)s_val;
	    } else //Unsigned types
	    {
#ifdef USE_LONG
		if(isvlong) {u_val = va_arg(args, nexx_u64_t); }
		else if(islong) { u_val = va_arg(args, nexx_u32_t); }
		else { u_val = va_arg(args, nexx_u32_t); }
#else
		u_val = va_arg(args, nexx_u32_t);
#endif
	    }
	    
	    ptr = scratch + SCRATCH;
	    *--ptr = 0;
	    do
	    {
		nexx_s8_t ch = u_val % base + '0';
#ifdef USE_HEX
		if (ch > '9')
		{
		    ch += 'a' - '9' - 1;
#ifdef USE_UPPERHEX
		    ch-=0x20;
#endif
		}
#endif					
		*--ptr = ch;
		u_val /= base;
		
#ifdef PADDING
		if(width) width--; //calculate number of padding chars
#endif
	    } while (u_val);
	    
#ifdef PADDING
	    while(width--) *--ptr = fill; //insert padding chars					
#endif
	    
	    while(*ptr) { if (fp->putc) fp->putc(*ptr, fp->a); ptr++; }
	}
    }
}

static nexx_s32_t printfputc(nexx_s32_t c, void *a){
    nexx_u32_t * nc = (nexx_u32_t *) a;
    if(c == '\n'){
	xputchar('\r');
	(*nc)++;
    }
    
    xputchar(c);
    (*nc)++;
    return 1;
}

nexx_s32_t xprintf(const char *fmt, ...){
    nexx_s32_t nc=0;
    Fprint fp = {printfputc, (void*)&nc};
    va_list args;
    va_start(args, fmt);
    vrprintf(&fp, fmt, args);
    va_end(args);
    
    return nc;
}
void *memmove(void *dest, void *src, unsigned long count) {
    nexx_u8_t *tmp;
    const nexx_u8_t *s;
    
    if (dest <= src) {
	tmp = dest;
	s = src;
	while (count--)
	    *tmp++ = *s++;
    } else {
	tmp = dest;
	tmp += count;
	s = src;
	s += count;
	while (count--)
	    *--tmp = *--s;
    }
    return dest;
}

void *memset(void *dst, nexx_s32_t s, unsigned long count) {
    register nexx_s8_t *a = dst;
    count++;
    while (--count)
	*a++ = s;
    return dst;
}

void *memcpy(void *dst, const void *src, unsigned long count) {
    register nexx_s8_t *d=dst;
    register const nexx_s8_t *s=src;
    ++count;
    while (--count) {
	*d = *s;
	++d; ++s;
    }
    return dst;
}

