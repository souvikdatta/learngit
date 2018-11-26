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


INCLUDES=-I ../../../core/include/
CONTAINER_BIN=../../../container.bin

FILES=serial.c boot.S loader.c
OBJS=serial.o boot.o loader.o

COMMON_DIR=../common
COMMON_OBJS=$(COMMON_DIR)/common_objs.o

include ../../../config.mk

FLAGS=-g -mcpu=arm926ej-s

all: compile pack

compile:
	@$(MAKE) -s -C $(COMMON_DIR)
	@for file in $(FILES) ; do \
		${CC} ${FLAGS} -c $$file ${INCLUDES} ; \
		if [ "$$?" -ne 0 ]; then exit 1 ; fi \
	done
	
clean:
	rm -f *.o *.elf


test:
	@qemu-system-arm -M versatilepb -m 128M -nographic -kernel rsw.elf

test2:
	@qemu-system-arm -M versatilepb -m 128M -nographic -kernel rsw.elf -s -S


pack:
	@touch tmp.S
	@$(CC) -c tmp.S
	@$(OBJCOPY) --add-section=.container=$(CONTAINER_BIN) --set-section-flags=.container=load,alloc tmp.o
	@$(LD) -o rsw.elf $(OBJS) tmp.o $(COMMON_OBJS) $(LIBGCC) -Tlinker.lds 
	@$(RM) tmp.S tmp.o
	@echo "> Created \"rsw.elf\""

