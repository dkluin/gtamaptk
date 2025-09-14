# gtamaptk-idmover
This tool can remap existing IDEs and IPLs from the `input` folders to a new ID range. If the `input` folder does not exist, the tool will create it automatically and give an error.

## Supported IDE sections
Right now only IDE:anim, IDE:objs and IDE:tobj are supported. More IDE sections will be supported later.

## Usage
Do note: The input IPLs need to be text IPLs. If you want to move binary IPLs, decompile them first using `gtamaptk-iplcomp`.

```
gtamaptk-buildtxdfolders [start model ID]
```