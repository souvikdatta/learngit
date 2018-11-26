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

INCLUDES=-I include/
FILES=hypercalls.c klibc.c
OBJS=hypercalls.o klibc.c

include ../../config.mk

FLAGS=-g -mcpu=arm926ej-s

all: compile link done


compile:
	@$(CC) -c $(FILES) $(FLAGS) $(INCLUDES)


link:
	@$(AR) rcs libnal.a $(OBJS)
done:
	@echo "> libnal.a created."

clean:
	rm -f *.o *.bin *.elf *.a
