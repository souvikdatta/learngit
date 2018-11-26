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

all: compile

include ../config.mk

compile:
	@$(MAKE) -C kernel
	@$(MAKE) -C klibc
	@${LD} -r -o nexx_core.o kernel/kern.o klibc/klibc.o
	@echo "   - nexx_core.o"

clean:
	@rm -f *.elf *.bin flash *.o
	@echo "Clean Done"

msg:
	@echo ""
	@echo "Executa: qemu-system-arm -M versatilepb -m 256M -nographic -kernel final.bin"
	@echo ""
