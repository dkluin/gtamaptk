# gtamaptk-conv2dfx
I've written a 2dfx data converter a decade ago by now, but as I probably stated before, the code for it is not up to my current standards so I've rewritten it to make it suitable for public use.
This version uses a (at the time of writing) not-yet included PR for librwgta which lets you append 2DFX sections & uses the previously established library to parse IDE files.

## Usage
You can use the the tool as follows:

```
gtamaptk-conv2dfx [IDE file name] [path to folder containing DFF files] [output folder where the combined DFF files will be written to]
```