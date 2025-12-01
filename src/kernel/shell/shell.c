#include "shell.h"
#include <stdio.h>
#include <arch/i686/io.h>
#include <arch/i686/irq.h>

void keyboard_handler(Registers* Reg) {
    uint8_t scancode = i686_inb(0x60);
    if (scancode & 0x80) return;
    char c = kbd_US[scancode];
    if (c) printf("%c", c);
    i686_outb(0x20, 0x20);
}

void ShellInitialize() {
    i686_IRQ_RegisterHandler(1, keyboard_handler);
}
