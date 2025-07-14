# GTA Mapping Toolkit (gtamaptk)
[![CMake Autobuild](https://github.com/dkluin/gtamaptk/actions/workflows/cmake-autobuild.yml/badge.svg)](https://github.com/dkluin/gtamaptk/actions/workflows/cmake-autobuild.yml)

This is a list of all the small little tools I wrote whilst working on GTA: Underground.
These are designed to tackle a specific problem I encountered and/or to do things that cannot easily be done with pre-existing tools. Writing little tools like these cut back on time significantly.

I decided to release them, I gave them all a fresh coat of paint, set up a nice workflow for them so you can download the binaries independently.

## Included tools
The following tools are included:

- gtamaptk-assignlods -> Can be used to assign LODs in one IPL (can be binary or text) which are located in a separate IPL.
- gtamaptk-buildtxdfolders -> Given an IDE file and list of DFF models, this tool can autobuild TXD folders, assuming you have one big texture folder. Used for GTA: Cottonmmouth and has its roots in Midway's development workflow.
- gtamaptk-core -> Shared library that can read/write IDE & IPL files. This library is used by pretty much all of the tools listed here.
- gtamaptk-cullzone -> Can compile/decompile a CULLZONE.DAT file from GTA: III.
- gtamaptk-dff2dfx -> Bit of a fork of The_Hero's convdff, except that this one is specifically designed for III/VC to SA DFF conversion with the ability to generate 2dfx sections for every given DFF.
- gtamaptk-entityinst -> Prop placement conversion tool for Manhunt entity.inst files. Outputs an IPL file you can use to port the files to GTA.
- gtamaptk-iplcomp -> Custom IPL compiler/decompiler that can autoresolve model names when decompiling.