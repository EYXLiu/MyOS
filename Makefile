TARGET = boot.bin

AS = nasm
CC = gcc
BUILD_DIR = build
SRC_DIR = src
TOOLS_DIR = tools

.PHONY: all floppy kernel bootloader run mdir clean always tools_fat

all: floppy tools_fat

#
# floppy image
#
floppy: $(BUILD_DIR)/floppy.img
$(BUILD_DIR)/floppy.img: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin | $(BUILD_DIR)
	mformat -C -v "NBOS" -f 1440 -i $(BUILD_DIR)/floppy.img
	dd if=$(BUILD_DIR)/boot.bin of=$(BUILD_DIR)/floppy.img conv=notrunc
	mcopy -i $(BUILD_DIR)/floppy.img $(BUILD_DIR)/kernel.bin "::kernel.bin"
	mcopy -i $(BUILD_DIR)/floppy.img text.txt "::text.txt"

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
# tools
#
tools_fat: $(BUILD_DIR)/tools/fat
$(BUILD_DIR)/tools/fat: $(TOOLS_DIR)/fat/fat.c
	mkdir -p $(BUILD_DIR)/tools
	$(CC) -g -o $(BUILD_DIR)/tools/fat $(TOOLS_DIR)/fat/fat.c

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