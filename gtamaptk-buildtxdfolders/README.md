# gtamaptk-buildtxdfolders
I wrote this tool to automatically generate texture folders based on a predefined IDE file while working on custom GTA maps. Back in the UG days, the earliest assets for Midway had no texture folders, instead, one massive folder containing textures for the entire scene was used.
This is very unfortunate, since this means you're forced to either:

- Use a massive TXD, which is a very bad practice, as the entire TXD would massively spike up streaming memory usage AND the game will slow down drastically as texture lookups take significantly longer
- Write a tool to autobuild a TXD for every 5 models

Neither of them seemed favourable. Blaue / Mario, recognizing this, had already started making texture folders for all of his later assets, so from that point on, it became a crucial point of our workflow.
Whenever we made new buildings, we made sure we set up a new IDE line with the appropriate draw distance and flags accompanied with a texture folder.

However, this still left one drawback - we needed a way to quickly move textures for the older assets into the appropriate folders to allow us to quickly build them into TXDs. **This is where this tool comes in**.
For GTA: Cottonmouth, we faced the exact same problem, so to solve it, we set up an entire workflow, set up IDEs, and left the building of texture folders up to automation via this very handy tool.

## Usage
As explained above, this tool is best utilized as part of your workflow for custom cities. Once your scene models are all defined in an IDE file and are properly named, you just have to use the tool as follows:

```
gtamaptk-buildtxdfolders [path to individual DFF file] [path to an IDE file] [path to original texture folder containing the textures for your scene] [path to folder in which to generate the texture folders]
```

For now, you still have to specify an individual DFF file. You can use the magic of the command line interface to iterate through all DFFs in a given folder:

```
for %%f in (path to folder containing model files]/*.dff) do gtamaptk-buildtxdfolders [path to folder containing model files]/%%f [path to an IDE file] [path to original texture folder containing the textures for your scene] [path to folder in which to generate the texture folders]
```

## Performance
I do plan on making a modification in the future that lets you specify a folder containing the models instead to speed things up. For now, performance is an area of improvement, I welcome any further suggestions and PRs.