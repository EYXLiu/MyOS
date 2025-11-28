#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "stdio.h"
#include <arch/i686/io.h>

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOR = 0x07;

uint8_t* g_ScreenBuffer = (uint8_t*)0xB8000;
int g_ScreenX = 0;
int g_ScreenY = 0;

void VGA_setcursor(int x, int y);


void VGA_putchr(int x, int y, char c) {
    g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)] = c;
}

void VGA_putcolor(int x, int y, uint8_t color) {
    g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

char VGA_getchr(int x, int y) {
    return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t VGA_getcolor(int x, int y) {
    return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

void VGA_clrscr() {
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            VGA_putchr(x, y, '\0');
            VGA_putcolor(x, y, DEFAULT_COLOR);
        }
    g_ScreenX = 0;
    g_ScreenY = 0;
    VGA_setcursor(g_ScreenX, g_ScreenY);
}

void VGA_setcursor(int x, int y) {
    int pos = y * SCREEN_WIDTH + x;

    i686_outb(0x3D4, 0x0F);
    i686_outb(0x3D5, (uint8_t)(pos & 0xFF));
    i686_outb(0x3D4, 0x0E);
    i686_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void VGA_scrollback(int lines) {
    for (int y = lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            VGA_putchr(x, y - lines, VGA_getchr(x, y));
            VGA_putcolor(x, y - lines, VGA_getcolor(x, y));
        }
    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            VGA_putchr(x, y, '\0');
            VGA_putcolor(x, y, DEFAULT_COLOR);
        }
    g_ScreenY -= lines;
}

void VGA_putc(char c) {
    switch (c) {
    case '\n':
        g_ScreenX = 0;
        g_ScreenY++;
        break;
    case '\r':
        g_ScreenX = 0;
        break;
    case '\t':
        for (int i = 0; i < 4 - (g_ScreenX % 4); i++)
            VGA_putc(' ');
        break;
    default:
        VGA_putchr(g_ScreenX, g_ScreenY, c);
        g_ScreenX++;
        break;
    }
    if (g_ScreenX >= SCREEN_WIDTH) {
        g_ScreenX = 0;
        g_ScreenY++;
    }

    if (g_ScreenY >= SCREEN_HEIGHT)
        VGA_scrollback(1);

    VGA_setcursor(g_ScreenX, g_ScreenY);
}

void VGA_puts(const char* str) {
    while (*str) {
        VGA_putc(*str);
        str++;
    }
}

const char VGA_g_HexChars[] = "0123456789abcdef";

void VGA_printf_unsigned(unsigned long long number, int radix)
{
    char buffer[32]; 
    int pos = 0;

    // convert number to ASCII
    do {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = VGA_g_HexChars[rem];
    } while (number > 0);

    // print number in reverse order
    while (--pos >= 0)
        VGA_putc(buffer[pos]);
}

void VGA_printf_signed(long long number, int radix)
{
    if (number < 0) {
        VGA_putc('-');
        return VGA_printf_unsigned(-number, radix);
    }
    return VGA_printf_unsigned(number, radix);
}

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

void VGA_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;
    int radix = 10;
    bool sign = false;
    bool number = false;

    while (*fmt) {
        switch (state) {
            case PRINTF_STATE_NORMAL:
                switch (*fmt) {
                    case '%':
                        state = PRINTF_STATE_LENGTH;
                        break;
                    default:   
                        VGA_putc(*fmt);
                        break;
                }
                break;

            case PRINTF_STATE_LENGTH:
                switch (*fmt) {
                    case 'h':
                        length = PRINTF_LENGTH_SHORT;
                        state = PRINTF_STATE_LENGTH_SHORT;
                        break;
                    case 'l':
                        length = PRINTF_LENGTH_LONG;
                        state = PRINTF_STATE_LENGTH_LONG;
                        break;
                    default:
                        goto PRINTF_STATE_SPEC_;
                }
                break;

            case PRINTF_STATE_LENGTH_SHORT:
                if (*fmt == 'h') {
                    length = PRINTF_LENGTH_SHORT_SHORT;
                    state = PRINTF_STATE_SPEC;
                }
                else goto PRINTF_STATE_SPEC_;
                break;

            case PRINTF_STATE_LENGTH_LONG:
                if (*fmt == 'l') {
                    length = PRINTF_LENGTH_LONG_LONG;
                    state = PRINTF_STATE_SPEC;
                }
                else goto PRINTF_STATE_SPEC_;
                break;

            case PRINTF_STATE_SPEC:
            PRINTF_STATE_SPEC_:
                switch (*fmt) {
                    case 'c':
                        VGA_putc((char)va_arg(args, int));
                        break;

                    case 's':
                        VGA_puts(va_arg(args, const char*));
                        break;

                    case '%':
                        VGA_putc('%');
                        break;

                    case 'd':
                    case 'i':
                        radix = 10;
                        sign = true;
                        number = true;
                        break;

                    case 'u':
                        radix = 10;
                        sign = false;
                        number = true;
                        break;

                    case 'X':
                    case 'x':
                    case 'p':
                        radix = 16;
                        sign = false;
                        number = true;
                        break;

                    case 'o':
                        radix = 8;
                        sign = false;
                        number = true;
                        break;

                    // ignore invalid spec
                    default:
                        break;
                }

                if (number) {
                    if (sign) {
                        switch (length) {
                        case PRINTF_LENGTH_DEFAULT:
                        case PRINTF_LENGTH_SHORT:
                        case PRINTF_LENGTH_SHORT_SHORT:
                            VGA_printf_signed(va_arg(args, int), radix);
                            break;
                        case PRINTF_LENGTH_LONG:
                            VGA_printf_signed(va_arg(args, long), radix);
                            break;
                        case PRINTF_LENGTH_LONG_LONG:
                            VGA_printf_signed(va_arg(args, long long), radix);
                            break;
                        }
                    } else {
                        switch (length) {
                        case PRINTF_LENGTH_DEFAULT:
                        case PRINTF_LENGTH_SHORT:
                        case PRINTF_LENGTH_SHORT_SHORT:
                            VGA_printf_unsigned(va_arg(args, unsigned int), radix);
                            break;
                        case PRINTF_LENGTH_LONG:
                            VGA_printf_unsigned(va_arg(args, unsigned long), radix);
                            break;
                        case PRINTF_LENGTH_LONG_LONG:
                            VGA_printf_unsigned(va_arg(args, unsigned long long), radix);
                            break;
                        }
                    }
                }

                // reset state
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                radix = 10;
                sign = false;
                number = false;
                break;
        }

        fmt++;
    }
    va_end(args);
}


void VGA_print_buffer(const char* msg, const void* buffer, uint32_t count) {
    const uint8_t* u8Buffer = (const uint8_t*) buffer;

    VGA_puts(msg);
    for (uint16_t i = 0; i < count; i++) {
        VGA_putc(VGA_g_HexChars[u8Buffer[i] >> 4]);
        VGA_putc(VGA_g_HexChars[u8Buffer[i] & 0xF]);
    }
    VGA_puts("\n");
}
