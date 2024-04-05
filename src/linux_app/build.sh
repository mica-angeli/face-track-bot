#!/usr/bin/env bash

RASPBERRY_PI_SYSROOT=$HOME/Developer/pizero-sysroot

`brew --prefix llvm`/bin/clang \
    --target=arm-linux-gnueabihf \
    --sysroot=$RASPBERRY_PI_SYSROOT \
    -isysroot=$RASPBERRY_PI_SYSROOT \
    -isystem=$RASPBERRY_PI_SYSROOT/usr/include/c++/10 \
    -isystem=$RASPBERRY_PI_SYSROOT/usr/include/arm-linux-gnueabihf/c++/10 \
    -L$RASPBERRY_PI_SYSROOT/usr/lib/gcc/arm-linux-gnueabihf/10 \
    -B$RASPBERRY_PI_SYSROOT/usr/lib/gcc/arm-linux-gnueabihf/10 \
    --gcc-toolchain=`brew --prefix arm-linux-gnueabihf-binutils` \
    -o robot_server \
    robot_server.c
