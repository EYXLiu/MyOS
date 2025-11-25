# Custom OS

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

## How I got started 
[OSDev Wiki](https://wiki.osdev.org/Expanded_Main_Page) <br/>
[x86 Assembly](https://www.felixcloutier.com/x86/) <br/>
[Daedalus Playlist](https://www.youtube.com/playlist?list=PLm3B56ql_akNcvH8vvJRYOc7TbYhRs19M) <br/>
[Nanobyte Playlist](https://www.youtube.com/playlist?list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN) <br/>
