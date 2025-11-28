ASFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib -I.
TARGET_CPPFLAGS += -ffreestanding -nostdlib -I.
TARGET_LFLAGS += -T linker.ld -nostdlib
TARGET_LIBS += -lgcc

HEADERS_ASM := $(shell find $(SRC_DIR)/libs/core -name '*.inc')
SOURCES_ASM := $(shell find $(SRC_DIR)/libs/core -name '*.asm')
OBJECTS_ASM := $(patsubst $(SRC_DIR)/libs/core/%.asm, $(BUILD_DIR)/libs/core/asm/%.o, $(SOURCES_ASM))
HEADERS_C := $(shell find $(SRC_DIR)/libs/core -name '*.h')
SOURCES_C := $(shell find $(SRC_DIR)/libs/core -name '*.c')
OBJECTS_C := $(patsubst $(SRC_DIR)/libs/core/%.c, $(BUILD_DIR)/libs/core/c/%.o, $(SOURCES_C))
HEADERS_CPP := $(shell find $(SRC_DIR)/libs/core -name '*.hpp')
SOURCES_CPP := $(shell find $(SRC_DIR)/libs/core -name '*.cpp')
OBJECTS_CPP := $(patsubst $(SRC_DIR)/libs/core/%.cpp, $(BUILD_DIR)/libs/core/cpp/%.o, $(SOURCES_CPP))

.PHONY: all libcore clean always

libcore: $(BUILD_DIR)/libcore.a always
$(BUILD_DIR)/libcore.a: $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_CPP)
	@echo $(OBJECTS_CPP)
	@$(AR) rcs $@ $^

$(BUILD_DIR)/libs/core/asm/%.o: %.asm $(HEADERS_ASM) | always
	@$(AS) $(ASFLAGS) -o $@ $< 

$(BUILD_DIR)/libs/core/c/%.o: %.c $(HEADERS_C) | always
	@$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/libs/core/cpp/%.o: %.cpp $(HEADERS_CPP) | always
	@$(TARGET_CPP) $(TARGET_CPPFLAGS) -c -o $@ $<

always:
	@mkdir -p $(BUILD_DIR)/libs/core/asm
	@mkdir -p $(BUILD_DIR)/libs/core/c
	@mkdir -p $(BUILD_DIR)/libs/core/cpp
	@mkdir -p $(BUILD_DIR)/libs/core/cpp/fs

clean:
	@rm -rf $(BUILD_DIR)/libs/core