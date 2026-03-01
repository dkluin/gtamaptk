# gtamaptk-colsplit
This tool can split a large COL archive into multiple COL archives based on the entries that exist in a given IDE or IPL file.
The input archive is not modified. The tool produces new archives based on matching entries.

## Usage
There are two possible modes for this tool:

- Splitting off entries present in IDE
- Splitting off entries present in IDE, but also present in IPL

These can be used as follows:

Splitting off entries present in IDE:
```
gtamaptk-colsplit -ide [input COL archive path] [input IDE file path] [output COL archive path]
```

Splitting off entries present in IDE, but also present in IPL:
```
gtamaptk-colsplit -ipl [input COL archive path] [input IDE file path] [output COL archive path] [input IPL file path]
```