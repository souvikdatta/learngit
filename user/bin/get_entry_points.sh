#!/bin/bash


_start0=$(nm --defined-only user/partitions/partition0.elf| grep start | cut -d" " -f1)
_start1=$(nm --defined-only user/partitions/partition1.elf| grep start | cut -d" " -f1)

echo "#define START_P0 0x${_start0}" >> autoconf2.h
echo "#define START_P1 0x${_start1}" >> autoconf2.h
