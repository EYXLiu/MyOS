#include "ctype.h"

bool islower(char chr) {
    return chr >= 'a' && chr <= 'z';
}

char toupper(char chr) {
    if (islower(chr))
        return chr - 'a' + 'A';
    return chr;
}
