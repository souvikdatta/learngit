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

include config.mk


all: help

linux: clean prepareLinux nexx_core users create_rsw_linux

bare: clean preparePartitions users nexx_core create_rsw_partitions

prepareLinux:
	@echo "Compiling NEXX with LINUX Support" 
	@echo ""
	@echo '#define IS_LINUX_PARTITION 1' > autoconf2.h
preparePartitions:
	@echo "Compiling NEXX with Partitions support" 
	@echo ""
	@echo ' ' > autoconf2.h

nexx_core:
	@$(MAKE) -s -C core
	@$(LD) -Tcore/kernel/linker.lds -o nexx_core.elf core/nexx_core.o $(LIBGCC)
	@$(OBJCOPY) -O binary nexx_core.elf nexx_core.bin
	@echo "> Created \"nexx_core.bin\""

users:
	@$(MAKE) -s -C user/partitions
	@./user/bin/get_entry_points.sh

create_rsw_partitions:
	@cat nexx_core.bin $(PARTITIONS_BIN) > container.bin
	@./user/bin/generateHarcodedInfo.sh
	@$(MAKE) -s -C user/rsw/arm
	@echo "> container.bin created."

create_rsw_linux:
	@$(OBJCOPY) -O binary $(LINUX_PATH)/vmlinux vmlinux.bin
	@cp $(LINUX_PATH)/vmlinux .
	@cat nexx_core.bin $(PARTITIONS_BIN) vmlinux.bin > container.bin
	@./user/bin/generateHarcodedInfoLinux.sh
	@$(MAKE) -s -C user/rsw/arm
	@echo "> container.bin created."

distclean: clean

clean:
	@exec echo -e "  - Removing *.o *.a *~ files";
	@find \( -name "*~" -o -name "*.o" -o -name "*.a" -o -name "*.xo" \) -exec rm '{}' \;
	@find \( -name "*.bin" -o -name "*.elf" \) -exec rm '{}' \;
	@$(RM) -f partitions.c flash.bin image.bin autoconf.h vmlinux autoconf2.h

flash:
	@dd if=/dev/zero of=./flash.bin bs=1024K count=16
	@$(OBJCOPY) -j ROM -O binary image.elf image.bin
	@dd if=image.bin of=flash.bin bs=4096 conv=notrunc

help:
	@echo "\nCompile NEXX for Linux:"
	@echo '$$> make linux'
	@echo ""
	@echo "Compile NEXX for Bare partitions"
	@echo '$$> make bare' 
	@echo ""

test:
	@qemu-system-arm -M versatilepb -m 128M -kernel user/rsw/arm/rsw.elf -serial stdio

test2:
	@qemu-system-arm -M versatilepb -m 128M -kernel user/rsw/arm/rsw.elf -serial stdio -s -S

test3:
	@qemu-system-arm -M versatilepb -m 128M -kernel user/rsw/arm/rsw.elf -serial stdio -append "console=ttyAMA0 root=/dev/ram rdinit=/sbin/init"

test4:
	@qemu-system-arm -M versatilepb -m 128M -kernel user/rsw/arm/rsw.elf -serial stdio -append "console=ttyAMA0 root=/dev/ram rdinit=/test"


