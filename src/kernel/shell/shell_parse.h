#pragma once

#include <stdint.h>

#define NO_REDIRECT 0
#define REDIRECT_TRUNC 1
#define REDIRECT_APPEND 2

typedef struct {
    char* argv[64];
    uint32_t argc;
    uint32_t mode;
    char* cmd;
    char* out_file;
} ParsedCommand;

ParsedCommand Shell_Parse(char* input);
