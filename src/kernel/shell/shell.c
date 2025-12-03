#include "shell.h"
#include <stdio.h>
#include <arch/i686/io.h>
#include <arch/i686/irq.h>
#include <string.h>
#include "shell_commands.h"

#define KEYBOARD_BUFFER_SIZE 128

static char g_KeyboardBuffer[KEYBOARD_BUFFER_SIZE];
static uint32_t g_KBTail = 0;

void keyboard_handler(Registers* Reg) {
    uint8_t scancode = i686_inb(0x60);
    if (!(scancode & 0x80)) {
        char c = kbd_US[scancode];
        if (c) {
            if (c == '\b') {
                if (g_KBTail > 0) {
                    g_KBTail--;
                    printf("\b");
                }
            } else if (g_KBTail < KEYBOARD_BUFFER_SIZE - 1) {
                g_KeyboardBuffer[g_KBTail++] = c;
                printf("%c", c);
            }
        }
    }
    i686_outb(0x20, 0x20);
}

void ShellPrint(const char* cmd) {
    if (cmd[0] == 0) return;

    char dst[KEYBOARD_BUFFER_SIZE];
    strncpy(dst, cmd, KEYBOARD_BUFFER_SIZE);
    char* token = strtok(dst, " ");

    if (strncmp(token, "froggo", 7) == 0) {
        token = strtok(NULL, " ");
        ShellFroggo(token);
    } else if (strncmp(token, "help", 5) == 0) {
        ShellHelp();
    } else if (strncmp(token, "echo", 5) == 0) {
        token = strtok(NULL, " ");
        ShellEcho(token);
    } else if (strncmp(token, "clear", 6) == 0) {
        Clear();
    } else {
        printf("zsh: command not found: %s\n", token);
    }
}

void ShellInitialize() {
    i686_IRQ_RegisterHandler(1, keyboard_handler);
}

void ShellRun() {
    char* path = "/";
    printf("FroggOS%s> ", path);
    for (;;) {
        if (g_KBTail > 0) {
            char c = g_KeyboardBuffer[g_KBTail - 1];

            if (c == '\n' || c == '\r') {
                g_KeyboardBuffer[g_KBTail - 1] = 0;
                ShellPrint(g_KeyboardBuffer);
                g_KBTail = 0;
                printf("FroggOS%s> ", path);
            }
        }

    }
}
