#include "string.h"
#include <mem/block_allocator.h>
#include <stddef.h>
#include <stdio.h>

int strncmp(const char* s1, const char* s2, size_t n) {
    size_t i = 0;

    while (i < n) {
        unsigned char c1 = (unsigned char)s1[i];
        unsigned char c2 = (unsigned char)s2[i];

        if (c1 != c2) {
            return c1 - c2;
        }

        if (c1 == 0) return 0;

        i++;
    }

    return 0;
}

char* strncpy(char* dst, const char* src, size_t n) {
    int i = 0;
    while (i < n && src[i] != 0) {
        dst[i] = src[i];
        i++;
    }
    while (i < n) {
        dst[i] = 0;
        i++;
    }
    return dst;
}

char* strtok(char* str, const char* delim) {
    static char* next = 0;
    if (str) next = str;
    if (!next) return NULL;

    while (*next) {
        int is_delim = 0;
        for (const char* d = delim; *d != 0; d++) {
            if (*next == *d) {
                is_delim = 1;
                break;
            }
        }
        if (!is_delim) break;
        next++;
    }

    if (*next == 0) return NULL;

    char* token_start = next;
    while (*next) {
        int is_delim = 0;
        for (const char* d = delim; *d != 0; d++) {
            if (*next == *d) {
                is_delim = 1;
                break;
            }
        }
        if (is_delim) {
            *next = 0;
            next++;
            break;
        }
        next++;
    }

    return token_start;
}

int strlen(const char* s) {
    int i = 0; 
    while (s[i] != 0) {
        i++;
    }
    return i;
}

char* strdup(const char* src) {
    size_t len = 0;
    while (src[len]) len++;

    char* out = KMalloc(len + 1);
    if (!out) {
        printf("STRING: out of memory\n");
        return NULL;
    }

    for (size_t i = 0; i <= len; i++) 
        out[i] = src[i];
    
    return out;
}
