# gtamaptk-cullzone
In GTA III, cull/attribute zones are stored in a binary file called CULLZONE.DAT. A text file does exist, however, but there are some differences between the zones defined in them + CULLZONE.DAT also stores a lot of indices in additional sections that thus far have been inaccessible.
This tool can decompile a CULLZONE.DAT file into an IPL like format and recompile it afterwards.

## Usage
One thing to note is that the CULLZONE.DAT file format was not designed to be edited in a text editor. The number of elements stored is hardcoded by default for every section specified, and is independent of the number of actual elements in said zone.
So for example, there are always 512 cull zones defined in CULLZONE.DAT, but the actual number of zones may be less than that, but not more.

The number of elements written/read from the file thus has to be specified to the tool.

### Decompiling CULLZONE.DAT
Decompiling goes as follows:

```
gtamaptk-cullzone -d [path to input file] [path to output file] [number of cull zones in file] [number of attribute zones in file] [number of zone indices] [number of buildings] [number of treadables]
```

### Compiling CULLZONE.DAT
Compiling goes as follows:

```
gtamaptk-cullzone -c [path to input file] [path to output file] [number of cull zones in file] [number of attribute zones in file] [number of zone indices] [number of buildings] [number of treadables]
```

### Default values
To decompile the file correctly, use the following values:

- Number of cull zones in file: 512
- Number of attribute zones in file: 288
- Number of zone indices: 55000
- Number of buildings (size of building pool): 5500
- Number of treadables (size of treadables pool): 1214

## Final notes
In order to make things easier to work with, it may be wise to come up with a way to autogenerate the indices in the final three blocks. I welcome any PRs / suggestions on doing just that.
Furthermore, for UG, I simply wrote a CULLZONE.DAT -> IPL converter. I do want to introduce such an option later on.