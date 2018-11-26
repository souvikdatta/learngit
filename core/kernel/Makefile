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

INCLUDES=-I../include/
FILES=board.c  irqs.c  regs.c  sched.c  setup.c  timers.c processor.c mmu.c head.S linux_boot.c hypercalls.c pcb.c
OBJS=board.o  irqs.o  regs.o  sched.o  setup.o  timers.o processor.o mmu.o head.o linux_boot.o hypercalls.o pcb.o
FLAGS=-g -mcpu=arm926ej-s

include ../../config.mk

all: compile link

compile:
	@for file in $(FILES) ; do \
        $(CC) $(FLAGS) -c $$file $(INCLUDES) ; \
        if [ "$$?" -ne 0 ]; then exit 1 ; fi \
    done
	@echo "> kern.o created."

link:
	@$(LD) -r -o kern.o $(OBJS)

clean:
	rm -f *.o
