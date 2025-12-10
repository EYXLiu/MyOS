#include "shell_parse.h"
#include <string.h>
#include <stddef.h>
#include <memory.h>

#include <stdio.h>

ParsedCommand Shell_Parse(char* input) {
    ParsedCommand cmd;
    cmd.mode = NO_REDIRECT;
    cmd.argc = 0;
    cmd.cmd = NULL;
    cmd.out_file = NULL;

    char* token = strtok(input, " ");
    cmd.cmd = token;
    token = strtok(NULL, " ");
    while (token) {
        if (strncmp(token, ">", 2) == 0) {
            cmd.mode = REDIRECT_TRUNC;
            cmd.out_file = strtok(NULL, " ");
            break;
        } else if (strncmp(token, ">>", 3) == 0) {
            cmd.mode = REDIRECT_APPEND;
            cmd.out_file = strtok(NULL, " ");
            break;
        } else {
            cmd.argv[cmd.argc++] = token;
        }
        token = strtok(NULL, " ");
    }
    cmd.argv[cmd.argc] = NULL;
    return cmd;
}
