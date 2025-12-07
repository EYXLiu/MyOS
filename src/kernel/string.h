#pragma once

#include <stddef.h>

int strncmp(const char* s1, const char* s2, size_t n);
char* strncpy(char* dst, const char* src, size_t n);
char* strtok(char* str, const char* delim);
int strlen(const char* s);
