ASFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib -I.
TARGET_CPPFLAGS += -ffreestanding -nostdlib -I.
TARGET_LFLAGS += -T linker.ld -nostdlib
TARGET_LIBS += $(BUILD_DIR)/libcore.a -lgcc

HEADERS_ASM := $(shell find $(SRC_DIR)/kernel -name '*.inc')
SOURCES_ASM := $(shell find $(SRC_DIR)/kernel -name '*.asm')
OBJECTS_ASM := $(patsubst $(SRC_DIR)/kernel/%.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SOURCES_ASM))
HEADERS_C := $(shell find $(SRC_DIR)/kernel -name '*.h')
SOURCES_C := $(shell find $(SRC_DIR)/kernel -name '*.c')
OBJECTS_C := $(patsubst $(SRC_DIR)/kernel/%.c, $(BUILD_DIR)/kernel/c/%.obj, $(SOURCES_C))

.PHONY: all kernel clean 

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	@$(TARGET_LD) $(TARGET_LFLAGS) -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/kernel/asm/%.obj: %.asm $(HEADERS_ASM)
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -o $@ $< 

$(BUILD_DIR)/kernel/c/%.obj: %.c $(HEADERS_C) 
	@mkdir -p $(dir $@)
	@$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)/kernel