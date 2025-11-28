ASFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib
TARGET_LFLAGS += -T linker.ld -nostdlib
TARGET_LIBS += -lgcc

SOURCES_ASM = $(wildcard *.asm)
OBJECTS_ASM = $(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))
SOURCES_C = $(wildcard *.c)
OBJECTS_C = $(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(SOURCES_C))

.PHONY: all stage2 clean always

all: stage2

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	@$(TARGET_LD) $(TARGET_LFLAGS) -Wl,-Map=$(BUILD_DIR)/stage2.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/stage2/asm/%.obj: %.asm | always
	@$(AS) $(ASFLAGS) -o $@ $< 

$(BUILD_DIR)/stage2/c/%.obj: %.c | always
	@$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

always:
	@mkdir -p $(BUILD_DIR)/stage2/asm
	@mkdir -p $(BUILD_DIR)/stage2/c

clean:
	@rm -rf $(BUILD_DIR)/stage2