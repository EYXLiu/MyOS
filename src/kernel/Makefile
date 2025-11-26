ASFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib -I.
TARGET_LFLAGS += -T linker.ld -nostdlib
TARGET_LIBS += -lgcc

HEADERS_ASM := $(shell find $(SRC_DIR)/kernel -name '*.inc')
SOURCES_ASM := $(shell find $(SRC_DIR)/kernel -name '*.asm')
OBJECTS_ASM := $(patsubst $(SRC_DIR)/kernel/%.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SOURCES_ASM))
HEADERS_C := $(shell find $(SRC_DIR)/kernel -name '*.h')
SOURCES_C := $(shell find $(SRC_DIR)/kernel -name '*.c')
OBJECTS_C := $(patsubst $(SRC_DIR)/kernel/%.c, $(BUILD_DIR)/kernel/c/%.obj, $(SOURCES_C))

.PHONY: all kernel clean always

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(TARGET_LD) $(TARGET_LFLAGS) -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/kernel/asm/%.obj: %.asm $(HEADERS_ASM) | always
	$(AS) $(ASFLAGS) -o $@ $< 

$(BUILD_DIR)/kernel/c/%.obj: %.c $(HEADERS_C) | always
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/kernel/asm/arch/i686
	mkdir -p $(BUILD_DIR)/kernel/asm
	mkdir -p $(BUILD_DIR)/kernel/c/arch/i686
	mkdir -p $(BUILD_DIR)/kernel/c/hal
	mkdir -p $(BUILD_DIR)/kernel/c

clean:
	rm -rf $(BUILD_DIR)/kernel