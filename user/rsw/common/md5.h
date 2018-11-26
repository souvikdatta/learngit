/*
 * FILE: md5.h
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

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
    nexx_u32_t i[2];                 /* number of _bits_ handled mod 2^64 */
    nexx_u32_t buf[4];               /* scratch buffer */
    nexx_u8_t in[64];            /* input buffer */
    nexx_u8_t digest[16];        /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init(MD5_CTX * mdContext);
void MD5Update(MD5_CTX * mdContext, nexx_u8_t * inBuf, nexx_u32_t inLen);
void MD5Final(MD5_CTX * mdContext, nexx_u8_t hash[]);

void printMD5(nexx_u8_t *msg, nexx_u8_t *hash);
