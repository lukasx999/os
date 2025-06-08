#!/bin/sh
set -euo pipefail

cflags="
-ffreestanding
-nostdlib
-std=c11
-Og
-ggdb
-Wall
-Wextra
-march=x86-64
-mcmodel=kernel
-I/usr/include
-I./yarl
-mgeneral-regs-only
"
linkerflags="-T linker.ld"
cc=~/opt/x86_64/bin/x86_64-elf-gcc
emu=qemu-system-x86_64
liminedir=/usr/share/limine

nasm -felf64 -gdwarf something.asm -o something.o
$cc $cflags -c kernel.c -o kernel.o
$cc $cflags $linkerflags kernel.o something.o -o os.bin

mkdir -p iso/boot/limine
cp os.bin iso/boot/
cp limine.conf $liminedir/limine-bios.sys $liminedir/limine-bios-cd.bin iso/boot/limine/

xorriso -as mkisofs -r -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 \
        iso/ -o os.iso

limine bios-install os.iso
# echo "system_reset" | socat - UNIX-CONNECT:/tmp/qemu.monitor
$emu -monitor unix:/tmp/qemu.monitor,server,nowait -serial stdio -cdrom os.iso # -S -s
