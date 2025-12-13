# Custom OS
Tech Stack: C, possibly C++, Assembly, Make, Qemu, GDB, Bochs, Bash

## Features
- **Core Systems**
  - [x] Bootloader 
  - [x] GDT 
  - [x] IDT 
  - [x] ISR 
  - [x] IRQ 
  - [x] Basic Interrupt handling 
  - [x] i686 I/O
  - [x] PCI
- **Memory**  
  - [x] Paging 
  - [x] Memory allocation 
  - [x] Strings (in memory)
  - [ ] Garbage collector/memory scanner
- **Standard Library**
  - [x] stdio.h 
  - [x] string.h 
  - [x] memory.h  
- **Debugging**
  - [x] Logging 
- **Drivers**
  - [x] IDE
  - [ ] RTL8139
- **File System**
  - [x] Basic file system (based off of FAT)
  - [x] Hard drive read/write  
- **Shell**
  - [x] Shell parsing 
  - Shell commands 
    - [x] help 
    - [x] ping (froggo) 
    - [x] clear 
    - [x] echo 
    - [x] cat  
    - [x] mkdir 
    - [x] rmdir 
    - [x] mkfile
    - [x] rmfile
    - [ ] curl
- **Networking**
  - [ ] IPv4 
  - [ ] TCP 
  - [ ] SSH  
- **User Applications**
  - [ ] Vim
  - [ ] doom
- **Visuals**
  - [x] Basic VGA (turned off for now)
- **Emulators**
  - [x] QEMU
  - [ ] GDB (need to change from .img to .elf)
  - [ ] Bochs


## QEMU Commands
- `option control 2` for Monitor console <br/>
  - `xp /128bx 0x7e00` to check buffer <br/>
- `option control 1` to switch back <br/>

## To compile for MacOS
#### Requirements <br/>
- i686.elf-gcc
- NASM
- QEMU
- gdb
#### To compile <br/>
```
make
make run
```
To test (in gdb) <br/>
```
make debug
```

## commands for gdb
```
info registers
print $register
set $register
stepi
nexti
continue
break
```

## How I got started 
[OSDev Wiki](https://wiki.osdev.org/Expanded_Main_Page) <br/>
[x86 Assembly](https://www.felixcloutier.com/x86/) <br/>
[Daedalus Playlist](https://www.youtube.com/playlist?list=PLm3B56ql_akNcvH8vvJRYOc7TbYhRs19M) <br/>
[Nanobyte Playlist](https://www.youtube.com/playlist?list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN) <br/>
