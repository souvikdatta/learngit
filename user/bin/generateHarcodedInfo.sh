#!/bin/bash

FILE=autoconf.h


NEXX_DIR=. # because this script is executed from Makefile of NEXX hypervisor root

NEXX_CORE_BIN=$NEXX_DIR/nexx_core.bin
PARTITION1_BIN=$NEXX_DIR/user/partitions/partition0.bin
PARTITION2_BIN=$NEXX_DIR/user/partitions/partition1.bin

nexx_size=$(du -sb $NEXX_CORE_BIN | cut -f1)
p1_size=$(du -sb $PARTITION1_BIN | cut -f1)
p2_size=$(du -sb $PARTITION2_BIN | cut -f1)
#linux_size=0

echo "/* This file was generated automatically. Please don't modify */" > $FILE
echo "/*  Configuration variables */" >> $FILE
echo "" >> $FILE

echo "#define AUTOCONF_NEXX_SIZE $nexx_size" >> $FILE
echo "#define AUTOCONF_P1_SIZE $p1_size" >> $FILE
echo "#define AUTOCONF_P2_SIZE $p2_size" >> $FILE
