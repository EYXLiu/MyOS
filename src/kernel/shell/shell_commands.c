#include "shell_commands.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <kstring/string.h>
#include <kfat/fat.h>

typedef struct {
    const char* name;
    const char* usage;
} Command;

const Command shell_command_list[] = {
    { "help", "help" },
    { "froggo", "froggo" },
    { "clear", "clear" },
    { "ls", "ls" },
    { "mkdir", "mkdir [dir name]" },
    { "mkfile", "mkfile [file name]" },
    { "rmdir", "rmdir [dir name]" },
    { "rmfile", "rmfile [file name]" },
    { "echo", "echo -o [-n] [string...] -o [> | >>] [file name]" }, 
    { "cat", "cat [file name...]"}
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

void Shell_LS(Directory* dir) {
    FS_LS(dir);
}

void Shell_Echo(ParsedCommand* pc, Directory* dir) {
    bool trail = pc->mode == NO_REDIRECT;
    int arg = 0;

    if (pc->argc > 1 && strncmp(pc->argv[0], "-n", 3) == 0) {
        trail = !trail;
        arg = 1;
    }
    
    KString ks = KS_Init();
    for(int i = arg; i < pc->argc; i++) {
        if (i != arg) KS_AppendChar(&ks, ' ');
        KS_Append(&ks, pc->argv[i]);
    }
    if (trail) KS_AppendChar(&ks, '\n');
    
    if (pc->mode == REDIRECT_APPEND) {
        FS_FileAppend(dir, pc->out_file, ks.string, ks.len);
    } else if (pc->mode == REDIRECT_TRUNC) {
        FS_FileWrite(dir, pc->out_file, ks.string, ks.len);
    } else {
        printf("%s", ks.string);
        return;
    }
    KS_Free(&ks);
}

void Shell_Cat(ParsedCommand* pc, Directory* dir) {
    if (pc->argc == 0) {
        printf("usage: cat [file name]\n");
        return;
    }
    for (int i = 0; i < pc->argc; i++) {
        FS_FilePrint(dir, pc->argv[i]);
    }
}

void Shell_CD(ParsedCommand* pc, Directory* dir) {
    if (pc->argc == 0) {
        printf("usage: cd [dir name]\n");
        return;
    }
    FS_CD(dir, pc->argv[0]);
}

void Shell_Mkdir(ParsedCommand* pc, Directory* dir) {
    if (pc->argc == 0) {
        printf("usage: mkdir [dir name]\n");
        return;
    }
    FS_DirCreate(dir, pc->argv[0]);
}

void Shell_Mkfile(ParsedCommand* pc, Directory* dir) {
    if (pc->argc == 0) {
        printf("usage: mkfile [file name]\n");
        return;
    }
    FS_FileCreate(dir, pc->argv[0]);
}

void Shell_Rmdir(ParsedCommand* pc, Directory* dir) {
    if (pc->argc == 0) {
        printf("usage: mkdir [dir name]\n");
        return;
    }
    FS_DirDelete(dir, pc->argv[0]);
}

void Shell_Rmfile(ParsedCommand* pc, Directory* dir) {
    if (pc->argc == 0) {
        printf("usage: mkfile [file name]\n");
        return;
    }
    FS_FileDelete(dir, pc->argv[0]);
}
