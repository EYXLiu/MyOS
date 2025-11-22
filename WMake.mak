CC16 = wcc
LD16 = wlink

BUILD_DIR = build
SRC_DIR = src
TOOLS_DIR = tools

all: stage2

stage2:
    wmake /f D:$(SRC_DIR)\bootlo~1\stage2\WMake.mak

clean:
    wmake /f D:$(SRC_DIR)\bootlo~1\stage2\WMake.mak clean

