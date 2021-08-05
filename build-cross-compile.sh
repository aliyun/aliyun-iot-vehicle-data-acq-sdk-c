#!/bin/sh

# the type of host and arch
export HOST=arm
export ARCH=arm-linux-gnueabi
export TARGET=${ARCH}

# inject cross compile tool-chain
#the root dir path of cross compile tool-chain

export CROSS_ROOT=/usr/bin
export CROSS_COMPILE=${CROSS_ROOT}/arm-linux-gnueabi-
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}g++
export LD=${CROSS_COMPILE}ld
export AR=${CROSS_COMPILE}ar
export RANLIB=${CROSS_COMPILE}ranlib
export STRIP=${CROSS_COMPILE}strip

# compile third-party lib
make -C third-party -f Makefile prepare

cmake . && make
