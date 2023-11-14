OBJECTS = k/loader.o k/kmain.o k/printf.o
CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T link.ld
AS = i686-elf-as
ASFLAGS =
all: kernel.bin

kernel.bin: $(OBJECTS)
	i686-elf-gcc -ffreestanding -O2 -nostdlib -T link.ld $(OBJECTS) -lgcc -o kernel.bin 

os.iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

run: os.iso
	qemu-system-x86_64 \
	-m 4G \
	-boot d \
	-serial stdio \
	-usb -device usb-kbd \
	-cdrom os.iso

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.asm
	nasm -f elf $< -o $@

clean:
	rm -rf *.o kernel.bin os.iso isodir k/*.o