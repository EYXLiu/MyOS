TARGET = boot.bin

AS = nasm
BUILD_DIR = build
SRC_DIR = src

.PHONY: all floppy kernel bootloader run mdir clean always 

all: floppy

#
# floppy image
#
floppy: $(BUILD_DIR)/floppy.img
$(BUILD_DIR)/floppy.img: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin | $(BUILD_DIR)
	dd if=/dev/zero of=$(BUILD_DIR)/floppy.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" $(BUILD_DIR)/floppy.img
	dd if=$(BUILD_DIR)/boot.bin of=$(BUILD_DIR)/floppy.img conv=notrunc
	mcopy -i $(BUILD_DIR)/floppy.img $(BUILD_DIR)/kernel.bin "::kernel.bin"

#
# bootloader
#
bootloader: $(BUILD_DIR)/boot.bin
$(BUILD_DIR)/boot.bin: | $(BUILD_DIR)
	$(AS) $(SRC_DIR)/bootloader/main.asm -f bin -o $(BUILD_DIR)/boot.bin

#
# kernel
#
kernel: $(BUILD_DIR)/kernel.bin
$(BUILD_DIR)/kernel.bin: | $(BUILD_DIR)
	$(AS) $(SRC_DIR)/kernel/main.asm -f bin -o $(BUILD_DIR)/kernel.bin

#
# build folder
#
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

#
# confirm
#
mdir: $(BUILD_DIR)/floppy.img
	mdir -i $(BUILD_DIR)/floppy.img

#
# run
#
run: $(BUILD_DIR)/floppy.img
	qemu-system-i386 -fda $(BUILD_DIR)/floppy.img

#
# clean
#
clean:
	rm -rf $(BUILD_DIR)