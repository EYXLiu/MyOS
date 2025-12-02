#include "shell_commands.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    const char* name;
    const char* usage;
} Command;

const Command shell_command_list[] = {
    { "help", "help" },
    { "froggo", "froggo" },
    { "echo", "echo [-n] [string...]" }, 
    { "clear", "clear" }
};

const int command_count = sizeof(shell_command_list) / sizeof(Command);

void ShellInvalid(const char* src, char* token) {
    while (token) {
        printf("%s: invalid argument: %s\n", src, token);
        token = strtok(NULL, " ");
    }
}

void ShellFroggo(char* token) {
    printf("%s\n", "doggo");
    ShellInvalid("froggo", token);
}

void ShellHelp() {
    printf("Welcome to my os!! Here are the commands currently avaliable:\n");
    for (int i = 0; i < command_count; i++)
        printf("%s\n usage: %s\n", shell_command_list[i].name, shell_command_list[i].usage);
}

void ShellEcho(char* token) {
    bool trail = true;
    if (strncmp(token, "-n", 3) == 0) {
        trail = false;
        token = strtok(NULL, " ");
    }
    printf("%s", token);
    token = strtok(NULL, " ");
    while (token) {
        printf(" %s", token);
        token = strtok(NULL, " ");
    }
    if (trail) printf("\n");
}
