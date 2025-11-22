BUILD_DIR?=build
AS?=nasm
ASFLAGS=-f obj

SOURCES_ASM = $(wildcard *.asm)
OBJECTS_ASM = $(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))

.PHONY: all stage2 clean

all: $(OBJECTS_ASM)

$(BUILD_DIR)/stage2/asm/%.obj: %.asm | always
	$(AS) $(ASFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/stage2/asm
	mkdir -p $(BUILD_DIR)/stage2/c

clean:
	rm -rf $(BUILD_DIR)/stage2