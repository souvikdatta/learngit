/*
 * FILE: serial.c
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


#define UART0  0x101f1000

#define IO_READ(addr) (*(volatile unsigned int *)(addr))
#define IO_WRITE(addr, val) (*(volatile unsigned int *)(addr) = (val))

#define UART_PL01x_DR                   0x00     /*  Data read or written from the interface. */
#define UART_PL01x_FR                   0x18     /*  Flag register (Read only). */
#define UART_PL01x_FR_TXFF              0x20


#define CONFIG_BAUDRATE 38400
#define baudRate CONFIG_BAUDRATE
#define CONFIG_PL011_CLOCK  24000000

#define UART_PL011_IBRD                 0x24
#define UART_PL011_FBRD                 0x28
#define UART_PL011_LCRH                 0x2C
#define UART_PL011_CR                   0x30
    
#define UART_PL011_CR_UARTEN            (1 << 0)
#define UART_PL011_CR_TXE               (1 << 8)
#define UART_PL011_CR_RXE               (1 << 9)

#define UART_PL011_LCRH_WLEN_8          (3 << 5)
#define UART_PL011_LCRH_FEN             (1 << 4)
 

void xputchar(char c) {
    /* Wait until there is space in the FIFO */
    while (IO_READ (UART0 + UART_PL01x_FR) & UART_PL01x_FR_TXFF);

    /* Send the character */
    IO_WRITE (UART0 + UART_PL01x_DR, c);
}

int serial_init (void)
{
    unsigned int temp;
    unsigned int divider;
    unsigned int remainder;
    unsigned int fraction;

    /* First, disable everything.*/
    IO_WRITE (UART0 + UART_PL011_CR, 0x0);

    /*
     ** Set baud rate
     **
     ** IBRD = UART_CLK / (16 * BAUD_RATE)
     ** FBRD = ROUND((64 * MOD(UART_CLK,(16 * BAUD_RATE))) / (16 * BAUD_RATE))
     */
    temp = 16 * baudRate;
    divider = CONFIG_PL011_CLOCK / temp;
    remainder = CONFIG_PL011_CLOCK % temp;
    temp = (8 * remainder) / baudRate;
    fraction = (temp >> 1) + (temp & 1);

    IO_WRITE (UART0 + UART_PL011_IBRD, divider);
    IO_WRITE (UART0 + UART_PL011_FBRD, fraction);

    /* Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled. */
    IO_WRITE (UART0 + UART_PL011_LCRH,
          (UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN));

    /* Finally, enable the UART */
    IO_WRITE (UART0 + UART_PL011_CR,
          (UART_PL011_CR_UARTEN | UART_PL011_CR_TXE |
           UART_PL011_CR_RXE));

    return 0;
}
