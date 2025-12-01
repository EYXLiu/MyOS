export AS = nasm
export ASFLAGS=

export TARGET = i686-elf
export TARGET_CC = $(TARGET)-gcc
export TARGET_CPP = $(TARGET)-g++
export TARGET_CFLAGS =-std=c99 -g
export TARGET_CPPFLAGS =-std=c++17 -g 
export TARGET_LD = $(TARGET)-gcc
export TARGET_OBJCOPY = $(TARGET)-objcopy
export TARGET_LFLAGS = 
export TARGET_LIBS =
export TARGET_AR = $(TARGET)-ar

export SOURCE_DIR = $(abspath .)
export SRC_DIR = $(abspath src)
export BUILD_DIR = $(abspath build)
