#pragma once
#include "shell_parse.h"
#include <kfat/fat.h>

void Shell_Froggo(ParsedCommand* pc);
void Shell_Help();

void Shell_Echo(ParsedCommand* pc, Directory* dir);
void Shell_Cat(ParsedCommand* pc, Directory* dir);

void Shell_LS(Directory* dir);
void Shell_CD(ParsedCommand* pc, Directory* dir);

void Shell_Mkdir(ParsedCommand* pc, Directory* dir);
void Shell_Mkfile(ParsedCommand* pc, Directory* dir);
void Shell_Rmdir(ParsedCommand* pc, Directory* dir);
void Shell_Rmfile(ParsedCommand* pc, Directory* dir);

