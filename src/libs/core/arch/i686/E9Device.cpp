#include "E9Device.hpp"
#include "io.hpp"

size_t arch::i686::E9Device::Read(uint8_t* data, size_t size) {
    return 0;
}

size_t arch::i686::E9Device::Write(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        Out(0xE9, data[i]);
    
    return size;
}
