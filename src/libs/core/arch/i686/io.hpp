#pragma once

#include <stdint.h>
#include <defs.hpp>

namespace arch {
    namespace i686 {
        EXPORT uint8_t ASMCALL In(uint16_t port);
        EXPORT void ASMCALL Out(uint16_t port, uint8_t value);
        EXPORT void ASMCALL Panic();
    }
}

