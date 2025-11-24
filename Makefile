AS = nasm
CC = gcc

MAKE = make
BUILD_DIR = build
SRC_DIR = src
TOOLS_DIR = tools

.PHONY: all floppy kernel bootloader run mdir clean always tools_fat

all: bootloader 

#
# bootloader
#
bootloader: stage1 stage2

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR)) AS=$(AS)

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR)) AS=$(AS)

#
# kernel
#
kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: always
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) AS=$(AS)

#
# build folder
#
always:
	mkdir -p $(BUILD_DIR)

#
# tools
#
tools: tools_fat

tools_fat: $(BUILD_DIR)/tools/fat
$(BUILD_DIR)/tools/fat: $(TOOLS_DIR)/fat/fat.c
	mkdir -p $(BUILD_DIR)/tools
	$(CC) -g -o $(BUILD_DIR)/tools/fat $(TOOLS_DIR)/fat/fat.c

#
# floppy image
#
floppy: $(BUILD_DIR)/floppy.img
$(BUILD_DIR)/floppy.img: bootloader kernel
	mformat -C -v "NBOS" -f 1440 -i $(BUILD_DIR)/floppy.img
	dd if=$(BUILD_DIR)/stage1.bin of=$(BUILD_DIR)/floppy.img conv=notrunc
	mcopy -i $(BUILD_DIR)/floppy.img $(BUILD_DIR)/stage2.bin "::stage2.bin"
	mcopy -i $(BUILD_DIR)/floppy.img $(BUILD_DIR)/kernel.bin "::kernel.bin"
	mcopy -i $(BUILD_DIR)/floppy.img text.txt "::text.txt"

#
# confirm
#
mdir: $(BUILD_DIR)/floppy.img
	mdir -i $(BUILD_DIR)/floppy.img

#
# run
#
run: floppy
	qemu-system-i386 -fda $(BUILD_DIR)/floppy.img

#
# clean
#
clean:
	$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	rm -rf $(BUILD_DIR)/*