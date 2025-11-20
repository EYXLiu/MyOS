TARGET = boot.bin
AS = nasm

all: $(TARGET)

$(TARGET): boot.asm
	$(AS) -f bin boot.asm -o boot.bin

run: $(TARGET)
	qemu-system-x86_64 $(TARGET)

clean:
	rm $(TARGET)