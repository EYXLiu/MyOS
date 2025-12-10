# Custom OS

## Features
**Core System** <br/>
- [x] Bootloader <br/>
- [x] GDT <br/>
- [x] IDT <br/>
- [x] ISR <br/>
- [x] IRQ <br/>
- [x] Basic Interrupt handling <br/>
- [x] i686 I/O <br/>
**Memory** <br/>
- [ ] Paging (wip) <br/>
- [x] Memory allocation <br/>
- [x] Strings (in memory) <br/>
**Standard Library** <br/>
- [x] stdio.h <br/>
- [x] string.h <br/>
- [x] memory.h <br/>
**Debugging** <br/>
- [x] Logging <br/>
**Drivers** <br/>
- [x] IDE <br/>
**File System** <br/>
- [x] Basic file system (based off of FAT) <br/>
- [x] Hard drive read/write <br/>
**Shell** <br/>
- [x] Shell parsing <br/>
- [ ] Shell commands <br/>
  - [x] help <br/>
  - [x] ping (froggo) <br/>
  - [x] clear <br/>
  - [ ] echo <br/>
  - [ ] cat <br/>
**Networking** <br/>
- [ ] IPv4 <br/>
- [ ] TCP <br/>
- [ ] SSH <br/>
**User Applications** <br/>
- [ ] Vim <br/>
**Visuals** <br/>
- [x] Basic VGA (turned off for now) <br/>


## QEMU Commands
- `option control 2` for Monitor console <br/>
  - `xp /128bx 0x7e00` to check buffer <br/>
- `option control 1` to switch back <br/>

## To compile for MacOS
Requirements <br/>
- i686.elf-gcc
- NASM
- QEMU
- gdb
To compile <br/>
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
