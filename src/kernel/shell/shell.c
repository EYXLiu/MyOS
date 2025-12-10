#include "shell.h"
#include <stdio.h>
#include <arch/i686/io.h>
#include <arch/i686/irq.h>
#include <string.h>
#include "shell_commands.h"
#include <kstring/string.h>
#include "shell_parse.h"
#include <stdbool.h>

#define KEYBOARD_BUFFER_SIZE 128

static char g_KeyboardBuffer[KEYBOARD_BUFFER_SIZE];
static uint32_t g_KBTail = 0;
static bool g_ShiftPressed = false;
static bool g_CtrlPressed = false;

Directory* g_Dir;
char* g_Path;
uint32_t g_PathEnd;

void keyboard_handler(Registers* Reg) {
    uint8_t scancode = i686_inb(0x60);
    switch (scancode) {
    case 0x2A:
    case 0x36:
        g_ShiftPressed = true;
        break;
    case 0xAA:
    case 0xB6:
        g_ShiftPressed = false;
        break;
    case 0x1D:
        g_CtrlPressed = true;
    case 0x9D:
        g_CtrlPressed = false;
    default:
        if (!(scancode & 0x80)) {
            char c;
            if (g_ShiftPressed) { 
                c = kbd_US_shift[scancode];
            } else {
                c = kbd_US[scancode];
            }
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
        break;
    }
    i686_outb(0x20, 0x20);
}

void Shell_Print(char* cmd) {
    if (cmd[0] == 0) return;

    ParsedCommand pc = Shell_Parse(cmd);
    char* command = pc.cmd;

    if (strncmp(command, "froggo", 7) == 0) {
        Shell_Froggo(&pc);
    } else if (strncmp(command, "help", 5) == 0) {
        Shell_Help();
    } else if (strncmp(command, "echo", 5) == 0) {
        Shell_Echo(&pc);
    } else if (strncmp(command, "clear", 6) == 0) {
        Clear();
    } else {
        printf("zsh: command not found: %s\n", command);
    }
}

void Shell_Initialize(Directory* dir) {
    i686_IRQ_RegisterHandler(1, keyboard_handler);
    g_Dir = dir;
    g_Path = dir->name;
    g_PathEnd = sizeof(dir->name);
}

void Shell_Run() {
    printf("FroggOS:%s> ", g_Path);
    for (;;) {
        if (g_KBTail > 0) {
            char c = g_KeyboardBuffer[g_KBTail - 1];

            if (c == '\n' || c == '\r') {
                g_KeyboardBuffer[g_KBTail - 1] = 0;
                Shell_Print(g_KeyboardBuffer);
                g_KBTail = 0;
                printf("FroggOS:%s> ", g_Path);
            }
        }

    }
}
