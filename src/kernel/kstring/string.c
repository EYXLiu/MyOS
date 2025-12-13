#include "string.h"
#include <mem/block_allocator.h>
#include <memory.h>
#include <string.h>

KString KS_Init() {
    KString ks;
    ks.cap = 32;
    ks.len = 0;
    ks.string = KMalloc(ks.cap);

    if (!ks.string) {
        ks.cap = 0;
    }

    ks.string[0] = 0;

    return ks;
}

void KS_Free(KString* ks) {
    if (ks->string) KFree(ks->string);
    ks->cap = 0;
    ks->len = 0;
    ks->string = NULL;
}

void KS_Alloc(KString* ks, size_t need) {
    if (need <= ks->cap) return;

    size_t ncap = ks->cap;
    while (ncap < need) 
        ncap += 32;

    char* nstring = KMalloc(ncap);
    if (!nstring) return;

    memcpy(nstring, ks->string, ks->len + 1);
    
    KFree(ks->string);

    ks->string = nstring;
    ks->cap = ncap;
}

void KS_Append(KString* ks, const char* s) {
    size_t slen = strlen(s);

    KS_Alloc(ks, ks->len + slen + 1);

    memcpy(ks->string + ks->len, s, slen + 1);
    ks->len += slen;
}

void KS_AppendChar(KString* ks, char c) {
    KS_Alloc(ks, ks->len + 2);

    ks->string[ks->len++] = c;
    ks->string[ks->len] = 0;
}

void KS_SetString(KString* ks, const char* s) {
    size_t slen = strlen(s);
    KS_Alloc(ks, slen + 1);
    memcpy(ks->string, s, slen + 1);
    ks->len = slen;
}

void KS_Remove(KString* ks, uint32_t num) {
    if (num >= ks->len) {
        KS_ClearString(ks);
    } else {
        ks->len -= num;
        ks->string[ks->len] = 0;
    }
}

void KS_RemoveChar(KString* ks) {
    KS_Remove(ks, 1);
}

void KS_ClearString(KString* ks) {
    ks->len = 0;
    ks->string[0] = 0;
}
