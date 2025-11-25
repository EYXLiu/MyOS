ASFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib
TARGET_LFLAGS += -T linker.ld -nostdlib
TARGET_LIBS += -lgcc

SOURCES_ASM = $(wildcard *.asm)
OBJECTS_ASM = $(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SOURCES_ASM))
SOURCES_C = $(wildcard *.c)
OBJECTS_C = $(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(SOURCES_C))

.PHONY: all kernel clean always

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(TARGET_LD) $(TARGET_LFLAGS) -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/kernel/asm/%.obj: %.asm | always
	$(AS) $(ASFLAGS) -o $@ $< 

$(BUILD_DIR)/kernel/c/%.obj: %.c | always
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/kernel/asm
	mkdir -p $(BUILD_DIR)/kernel/c

clean:
	rm -rf $(BUILD_DIR)/kernel