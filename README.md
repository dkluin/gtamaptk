# GTA Mapping Toolkit (gtamaptk)
[![Windows](https://github.com/dkluin/gtamaptk/actions/workflows/build-windows.yml/badge.svg)](https://github.com/dkluin/gtamaptk/actions/workflows/build-windows.yml)

This is a list of all the small little tools I wrote whilst working on GTA: Underground.
These are designed to tackle a specific problem I encountered and/or to do things that cannot easily be done with pre-existing tools. Writing little tools like these cut back on time significantly.

I decided to release them, I gave them all a fresh coat of paint, set up a nice workflow for them so you can download the binaries independently.

## Included tools
The following tools are included:

- gtamaptk-assignlods -> Can be used to assign LODs in one IPL (can be binary or text) which are located in a separate IPL.
- gtamaptk-buildtxdfolders -> Given an IDE file and list of DFF models, this tool can autobuild TXD folders, assuming you have one big texture folder. Used for GTA: Cottonmmouth and has its roots in Midway's development workflow.
- gtamaptk-core -> Shared library that can read/write IDE & IPL files. This library is used by pretty much all of the tools listed here.
- gtamaptk-cullzone -> Can compile/decompile a CULLZONE.DAT file from GTA: III.
- gtamaptk-entityinst -> Prop placement conversion tool for Manhunt entity.inst files. Outputs an IPL file you can use to port the files to GTA.
- gtamaptk-iplcomp -> Custom IPL compiler/decompiler that can autoresolve model names when decompiling.

## Coming later
There are more tools I've written through the ages that I wouldn't consider suitable for public use, and thus need a few tweaks so you will be able to get the most of them. Here's a rough list of them:

- An LOD assignment checker I wrote to help solve shutdown crashes
- A fork of The_Hero's convdff with additional IDE:2dfx conversion support (this one needs some extra work, I didn't convert ped attractors correctly at the time so rotations will be off in-game)
- An IPL:occl converter
- I had a more "complex" converter that could decode a CULLZONE.DAT file and write out the attribute zones to an IPL but I chose to first write a basic compiler/decompiler for the aforementioned reasons. 
- A III/VC paths converter
- A DFF-JSON conversion utility I wrote specifically for the UG website (it can preview all vehicle models)
- A fork of The_Hero's Euryopa that works on UG
- A web-based path previewer that fully supports FLA formats
- I'm working on my own fork of Kams right now with additional features to allow you to export breakable sections etc. It's part of the workflow I have set up for GTA: Cottonmouth.
- ...and probably other stuff.

## Other stuff
Apart from actual commandline tools I also modified numerous scripts over the years to make sure our workflow was up to standards. These will probably be in a separate repository, but here are some examples:

- We modified X's radar exporter to support variable map sizes, and had a massive radar image we composed from the mobile 256x256 tiles. This image was then extended, after which we drew the VC/III maps on top.
- There was a separate 3ds max script I wrote which more or less does the same thing as gtamaptk-buildtxdfolders, except that it never read an IDE. It just copy-pasted textures from one big folder to a destination folder, allowing us to build them with Magic later.

## Permission
You're free to use anything from this project, given attribution (see also LICENSE.md)