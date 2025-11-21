TARGET = boot.bin

AS = nasm
BUILD_DIR = build
BOOT_BIN = $(BUILD_DIR)/boot.bin
FLOPPY_IMG = $(BUILD_DIR)/floppy.img

all: $(FLOPPY_IMG)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): boot.asm | $(BUILD_DIR)
	$(AS) boot.asm -f bin -o $(BOOT_BIN)

$(FLOPPY_IMG): $(BOOT_BIN)
	cp $(BOOT_BIN) $(FLOPPY_IMG)
	truncate -s 1440k $(FLOPPY_IMG)

run: $(FLOPPY_IMG)
	qemu-system-i386 -fda $(FLOPPY_IMG)

clean:
	rm -rf $(BUILD_DIR)