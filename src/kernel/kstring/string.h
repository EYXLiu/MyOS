#pragma once

#include <stddef.h>
#include <stdint.h>

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
void KS_Remove(KString* ks, uint32_t num);
void KS_RemoveChar(KString* ks);
void KS_ClearString(KString* ks);
