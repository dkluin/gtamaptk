# gtamaptk-breakable
In the past, I used a fork of librwgta for which I added an option that can autogenerate breakable info for a specific DFF.
This tool revives this functionality through the gtamaptk library.

## How the breakable section works in GTA:SA
Since I noticed the breakable section is not properly documented anywhere I decided to give a short description on how it is used.
Basically, the breakable seciton contains a copy of the geometry data including UV mapping and prelighting. 
When you break a certain object, the game groups every part of the object by its material ID, and the game will give each mesh group its own "breaking animation".
This means that if your object only has one material, it will just fall over without breaking into multiple parts.

So simply put, the game will split off the geometry based on material ID and do a little breaking animation for each resulting mesh group.

## Usage
Normally, you'd use the tool as follows:

```
gtamaptk-breakable -m [DFF filename]
```


Now, since Kams does not write out the material ID properly in the triangle data itself, you may need to let the tool figure out the material ID manually from the mesh group data inside the DFF.
This is where the `-a` option comes in:

```
gtamaptk-breakable -a [DFF filename]
```

After running, the existing DFF will have a breakable section. All you then need to do is give it a solid object.dat entry and it will be properly breakable in-game.