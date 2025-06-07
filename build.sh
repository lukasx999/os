#!/bin/sh
set -euo pipefail

cflags="-ffreestanding -nostdlib -std=c11 -Og -Wall -Wextra -ggdb -march=x86-64 -mcmodel=kernel -I/usr/include -I./yarl"
linkerflags="-T linker.ld"
cc=~/opt/x86_64/bin/x86_64-elf-gcc
emu=qemu-system-x86_64
liminedir=/usr/share/limine

$cc $cflags -c kernel.c -o kernel.o
$cc $cflags $linkerflags -static kernel.o -o os.bin

mkdir -p iso/boot/limine
cp os.bin iso/boot/
cp limine.conf $liminedir/limine-bios.sys $liminedir/limine-bios-cd.bin iso/boot/limine/

mkdir -p iso/EFI/BOOT
cp $liminedir/BOOTX64.EFI iso/EFI/BOOT/

xorriso -as mkisofs -r -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 \
        iso/ -o os.iso

limine bios-install os.iso
$emu -cdrom os.iso # -S -s
