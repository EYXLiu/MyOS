#include "shell_commands.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "kstring/string.h"

typedef struct {
    const char* name;
    const char* usage;
} Command;

const Command shell_command_list[] = {
    { "help", "help" },
    { "froggo", "froggo" },
    { "echo", "echo [-n] [string...] -o [> | >>] [file name]" }, 
    { "clear", "clear" }
};

const int command_count = sizeof(shell_command_list) / sizeof(Command);

void Shell_Froggo(ParsedCommand* pc) {
    printf("%s\n", "doggo");
}

void Shell_Help() {
    printf("Welcome to my os!! Here are the commands currently avaliable:\n");
    for (int i = 0; i < command_count; i++)
        printf("%s\n usage: %s\n", shell_command_list[i].name, shell_command_list[i].usage);
}

void Shell_Echo(ParsedCommand* pc) {
    bool trail = true;
    int arg = 1;

    if (pc->argc > 1 && strncmp(pc->argv[1], "-n", 3) == 0) {
        trail = false;
        arg = 2;
    }
    
    KString ks = KS_Init();
    for(int i = arg; i < pc->argc; i++) {
        if (i != arg) KS_AppendChar(&ks, ' ');
        KS_Append(&ks, pc->argv[i]);
    }
    if (trail) KS_AppendChar(&ks, '\n');

    if (pc->mode == NO_REDIRECT) {
        printf("%s", ks.string);
        return;
    }

    KS_Free(&ks);
}
