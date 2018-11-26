 #
 # FILE: config.mk
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


ARM_PATH=/opt/arm-2011.09
LINUX_PATH=${HOME}/linux-sources/linux-3.1.1

#End config

CC=${ARM_PATH}/bin/arm-none-eabi-gcc
LD=${ARM_PATH}/bin/arm-none-eabi-ld
AR=${ARM_PATH}/bin/arm-none-eabi-ar
OBJCOPY=${ARM_PATH}/bin/arm-none-eabi-objcopy

tmp=$(shell ${CC} -print-libgcc-file-name | sed 's/libgcc.a//g')

LIBGCC=-L ${tmp} -l gcc


PARTITIONS_BIN=user/partitions/partitions.bin

