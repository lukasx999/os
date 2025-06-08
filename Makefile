CC=~/opt/x86_64/bin/x86_64-elf-gcc
CFLAGS=             \
-ffreestanding      \
-nostdlib           \
-std=c11            \
-Og                 \
-ggdb               \
-Wall               \
-Wextra             \
-march=x86-64       \
-mcmodel=kernel     \
-I/usr/include      \
-I./yarl            \
-mgeneral-regs-only

LINKFLAGS=-T linker.ld

EMU=qemu-system-x86_64
EMUFLAGS=-monitor unix:/tmp/qemu.monitor,server,nowait -serial stdio

OBJS=kernel.o serial.o string.o io.s.o
DEPS=kernel.h serial.h string.h io.h

os.bin: $(OBJS)
	@echo LINK $^
	@$(CC) $(CFLAGS) $(LINKFLAGS) $^ -o $@

%.o: %.c $(DEPS) Makefile
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.s.o: %.asm Makefile
	@echo NASM $<
	@nasm -felf64 -gdwarf $< -o $@

os.iso: os.bin
	@mkdir -p iso/boot/limine
	@cp os.bin iso/boot/
	@cp limine.conf /usr/share/limine/limine-bios.sys /usr/share/limine/limine-bios-cd.bin iso/boot/limine/
	
	@xorriso -as mkisofs -r -b boot/limine/limine-bios-cd.bin \
	        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	        -apm-block-size 2048 \
	        iso/ -o os.iso &>/dev/null

	@limine bios-install os.iso &>/dev/null

rebuild: os.bin
	@echo REBUILD $<
	@echo "system_reset" | socat - UNIX-CONNECT:/tmp/qemu.monitor

emu: os.iso
	@echo EMU $<
	@$(EMU) $(EMUFLAGS) -cdrom os.iso

debug: os.iso
	@echo EMU $<
	@$(EMU) $(EMUFLAGS) -cdrom os.iso -S -s

gdb: os.bin
	@echo GDB $<
	@gdb $< -ex "target remote :1234" -ex "b kernel_main" -ex "c"

clean:
	rm *.o os.bin *.iso

.PHONY: clean
