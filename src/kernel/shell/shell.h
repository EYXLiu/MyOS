#pragma once

#include <arch/i686/irq.h>
#include <kfat/fat.h>

void Shell_Initialize(Directory* dir);
void Shell_Run();
