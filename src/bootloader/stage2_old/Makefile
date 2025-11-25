ASFLAGS += -f obj

SOURCES_ASM = $(wildcard *.asm)
OBJECTS_ASM = $(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))
SOURCES_C = $(wildcard *.c)
OBJECTS_C = $(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(SOURCES_C))

.PHONY: all stage2 clean always

all: stage2

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD16) NAME $(BUILD_DIR)/stage2.bin FILE \{ $(OBJECTS_ASM) $(OBJECTS_C) \} OPTION MAP=$(BUILD_DIR)/stage2.map @linker.lnk

$(BUILD_DIR)/stage2/asm/%.obj: %.asm | always
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/stage2/c/%.obj: %.c | always
	$(CC16) $(CFLAGS16) $< -fo=$@

always:
	mkdir -p $(BUILD_DIR)/stage2/asm
	mkdir -p $(BUILD_DIR)/stage2/c

clean:
	rm -rf $(BUILD_DIR)/stage2