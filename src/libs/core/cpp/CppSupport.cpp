#include <defs.hpp>
#include <arch/i686/io.hpp>

EXPORT int __cxa_atexit(void (*destructor) (void *), void *arg, void *__dso_handle) { return 0; }

EXPORT void* __dso_handle = 0;

void operator delete(void*, unsigned long) { arch::i686::Panic(); }
