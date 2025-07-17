# gtamaptk-assignlods
This tool is able to assign LODs in decompiled binary IPLs to text IPLs. Previous LOD links will be overwritten.

## Usage
```
gtamaptk-assignlods [path to text IPL containling LODs] [max allowed position difference between the normal model and LOD model] [path to 1st decompiled binary IPL containing normal models] [path to 2nd decompiled binary IPL containing normal models] ...
```

You can specify as many decompiled binary IPLs as you wish. In the future I may add support for compiled IPLs, but for now, only decompiled IPLs are supported. 
In addition, it is also possible to use a max allowed position value, should the LODs and normal models not share the same exact position. I recommend keeping them aligned, eliminating the risk of some models that are very close to one another being incorrectly linked.