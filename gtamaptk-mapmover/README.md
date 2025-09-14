# gtamaptk-mapmover
This tool can move maps to a new location using three offset coordinates. The files themselves should be put in the `input` directory. If it does not exist, the tool will create it automatically and give an error.

## Supported IPL sections
The following IPL sections are supported right now:

- IPL:auzo
- IPL:cars
- IPL:enex
- IPL:inst
- IPL:jump
- IPL:occl
- IPL:pick
- IPL:tcyc

The remaining IPL sections will be supported later.

## Usage
Note that only integers are supported, and that the input files need to be text IPLs. If you want to move binary IPLs, decompile them first using `gtamaptk-iplcomp`.

```
gtamaptk-mapmover [offset X] [offset Y] [offset Z]
```