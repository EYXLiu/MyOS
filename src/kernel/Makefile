BUILD_DIR?=build
AS?=nasm

.PHONY: all kernel clean

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin:
	$(AS) main.asm -f bin -o $(BUILD_DIR)/kernel.bin

clean:
	rm -rf $(BUILD_DIR)/kernel.bin