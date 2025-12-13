#include "shell_parse.h"
#include <string.h>
#include <stddef.h>
#include <memory.h>
#include <mem/block_allocator.h>
#include <kstring/string.h>

#include <stdio.h>

ParsedCommand Shell_Parse(KString* ks) {
    ParsedCommand cmd;
    cmd.mode = NO_REDIRECT;
    cmd.argc = 0;
    cmd.cmd = NULL;
    cmd.out_file = NULL;

    char* input = KMalloc(ks->len + 1);
    memcpy(input, ks->string, ks->len + 1);

    char* token = strtok(input, " ");
    cmd.cmd = strdup(token);
    token = strtok(NULL, " ");
    while (token) {
        if (strncmp(token, ">", 2) == 0) {
            cmd.mode = REDIRECT_TRUNC;
            cmd.out_file = strdup(strtok(NULL, " "));
            break;
        } else if (strncmp(token, ">>", 3) == 0) {
            cmd.mode = REDIRECT_APPEND;
            cmd.out_file = strdup(strtok(NULL, " "));
            break;
        } else {
            cmd.argv[cmd.argc++] = strdup(token);
        }
        token = strtok(NULL, " ");
    }
    cmd.argv[cmd.argc] = NULL;
    KFree(input);
    return cmd;
}

void Shell_Free(ParsedCommand* pc) {
    for (int i = 0; i < pc->argc; i++) {
        if (pc->argv[i]) KFree(pc->argv[i]);
    }
    if (pc->out_file) KFree(pc->out_file);
}
