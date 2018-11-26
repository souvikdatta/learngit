 #
 # FILE: Makefile
 #
 # VERSION: 0.0.1
 #
 # Author: Hector Marco-Gisbert <hmarco@hmarco.org>
 #
 # LICENSE:
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation; either version 2 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program; if not, write to the Free Software
 # Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 #

XAL_PATH=../libnal
LIB_XAL=-L ../libnal -l nal
INCLUDES=-I $(XAL_PATH)/include/
FILES=partition0.c partition1.c
OBJS=partition0.o partition1.o


include ../../config.mk

FLAGS=-g -c -mcpu=arm926ej-s

COMMON_OBJS=common/boot.o

all: common-objs partition0 partition1 done


START1=0xA00000
START2=0xB00000

common-objs:
	@$(MAKE) -s -C common
	@$(MAKE) -s -C $(XAL_PATH)

partition0:
	@$(CC) -mcpu=arm926ej-s partition0.c -o partition0.elf $(INCLUDES) $(COMMON_OBJS) $(LIB_XAL) -Tcommon/linker.lds -Ttext=$(START1);
	@$(OBJCOPY) -O binary partition0.elf partition0.bin


partition1:
	@$(CC) -mcpu=arm926ej-s partition1.c -o partition1.elf $(INCLUDES) $(COMMON_OBJS) $(LIB_XAL) -Tcommon/linker.lds -Ttext=$(START2);
	@$(OBJCOPY) -O binary partition1.elf partition1.bin

done:
	@cat partition0.bin partition1.bin > partitions.bin
	@echo "> partitions.bin created."

clean:
	@$(MAKE) -s -C common clean
	rm -f *.o *.bin *.elf
