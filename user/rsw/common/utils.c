/*
 * FILE: utils.c
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


#include "md5.h"

void printMD5(nexx_u8_t *msg, nexx_u8_t *hash)
{
   nexx_s32_t res=0;
   xprintf("%s", msg);
   for(res=0;res<16;res++)
      xprintf("%x", hash[res]);
   xprintf("]\n");
}

nexx_s32_t NEXX_MD5(nexx_u8_t * data, nexx_s32_t len, nexx_u8_t * hash)
{

    MD5_CTX mdContext;

    MD5Init(&mdContext);
    MD5Update(&mdContext, data, len);
    MD5Final(&mdContext, hash);


    return 0;
}

