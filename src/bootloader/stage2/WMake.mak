BUILD_DIR=build
SRC_DIR=src
CC16=wcc
CFLAGS16=-4 -d3 -wx -ms -zl -zq
LD16=wlink

OBJECTS_ASM = $(BUILD_DIR)\stage2\asm\main.obj $(BUILD_DIR)\stage2\asm\x86.obj

OBJECTS_C = $(BUILD_DIR)\stage2\c\main.obj $(BUILD_DIR)\stage2\c\std.obj

all: $(BUILD_DIR)\stage2.bin 

$(BUILD_DIR)\stage2\c\main.obj: $(SRC_DIR)\bootlo~1\stage2\main.c
	$(CC16) $(CFLAGS16) -I$(SRC_DIR)\bootlo~1\stage2 -fo=$@ $<

$(BUILD_DIR)\stage2\c\std.obj: $(SRC_DIR)\bootlo~1\stage2\std.c
	$(CC16) $(CFLAGS16) -I$(SRC_DIR)\bootlo~1\stage2 -fo=$@ $<

$(BUILD_DIR)\stage2.bin: $(OBJECTS_C) $(OBJECTS_ASM)
	wlib stage2_objs_c.lib $(OBJECTS_C)
	wlib stage2_objs_asm.lib $(OBJECTS_ASM)
	$(LD16) NAME $(BUILD_DIR)\stage2.bin FILE { stage2_objs_c.lib stage2_objs_asm.lib } OPTION MAP=$(BUILD_DIR)\sta.map 

clean:
	del $(BUILD_DIR)\stage2\c\main.obj
	del $(BUILD_DIR)\stage2\c\std.obj
	del stage2_objs_c.lib
	del stage2_objs_asm.lib
	del $(BUILD_DIR)\stage2.bin
	del $(BUILD_DIR)\sta.map
