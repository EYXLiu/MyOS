#pragma once

#include <stddef.h>

typedef struct {
    size_t len;
    size_t cap;
    char* string;
} KString;

KString KS_Init();
void KS_Free(KString* ks);
void KS_Append(KString* ks, const char* s);
void KS_AppendChar(KString* ks, char c);
void KS_SetString(KString* ks, const char* s);
