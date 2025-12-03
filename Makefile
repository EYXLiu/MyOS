MAKE = make
SRC_DIR = src
TOOLS_DIR = tools

include make_scripts/config.mk

.PHONY: all floppy kernel bootloader run mdir clean always tools_fat

all: bootloader 

#
# bootloader
#
bootloader: stage1 stage2 core kernel

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin: always
	@$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR))

stage2: $(BUILD_DIR)/stage2.bin core

$(BUILD_DIR)/stage2.bin: always
	@$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR))

#
# libcore
#
core: $(BUILD_DIR)/libcore.a

$(BUILD_DIR)/libcore.a: always
	@$(MAKE) -C $(SRC_DIR)/libs/core BUILD_DIR=$(abspath $(BUILD_DIR)) SRC_DIR=$(abspath $(SRC_DIR))

#
# kernel
#
kernel: $(BUILD_DIR)/kernel.bin core

$(BUILD_DIR)/kernel.bin: always
	@$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) SRC_DIR=$(abspath $(SRC_DIR))

#
# build folder
#
always:
	@mkdir -p $(BUILD_DIR)

#
# tools
#
tools: 

#
# floppy image
#
floppy: $(BUILD_DIR)/floppy.img
$(BUILD_DIR)/floppy.img: bootloader kernel
	@./make_scripts/make_floppy.sh $@

#
# confirm
#
mdir: floppy
	mdir -i $(BUILD_DIR)/floppy.img

#
# gdb
# 
debug: floppy
	qemu-system-i386 -fda $(BUILD_DIR)/floppy.img -S -s -serial stdio & sleep 1 && gdb -ex "target remote localhost:1234"

#
# run
#
run: floppy
	@qemu-system-i386 -debugcon stdio -fda $(BUILD_DIR)/floppy.img

#
# clean
#
clean:
	@$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	@$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	@$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(BUILD_DIR)