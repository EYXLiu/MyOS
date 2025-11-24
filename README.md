# Custom OS

## QEMU Commands
- `option control 2` for Monitor console <br/>
  - `xp /128bx 0x7e00` to check buffer <br/>
- `option control 1` to switch back <br/>

## To compile for MacOS
Since Macos can't run watcom, we use a linux docker image to run the wcc/wlink commands <br/>
In terminal
```
docker pull arlaneenalra/watcom-docker:latest
docker run --rm -it -v ~/documents/github/myos:/src arlaneenalra/watcom-docker:latest sh
```
In the docker image <br/>
```
make
```
In terminal <br/>
```
make run
```

## How I got started 
[OSDev Wiki](https://wiki.osdev.org/Expanded_Main_Page) <br/>
[Daedalus Playlist](https://www.youtube.com/playlist?list=PLm3B56ql_akNcvH8vvJRYOc7TbYhRs19M) <br/>
[Nanobyte Playlist](https://www.youtube.com/playlist?list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN) <br/>
