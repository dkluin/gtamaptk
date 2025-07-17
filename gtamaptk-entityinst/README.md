# gtamaptk-entityinst
This tool can compile/decompile a Manhunt entity.inst file. This file contains the following data:

- Placement data for level props
- Pickup locations for weapons
- Spawn points for all of the hunters
- 2dfx data
- ...and more.

## Usage
To decompile an entity.inst file to a text fole:
```
gtamaptk-entityinst -d [path to entity.inst file] [path to decompile file]
```

To compile an entity.inst file:
```
gtamaptk-entityinst -c [path to decompiled file] [path to compiled file]
```

## Support
Right now, it supports PC entity.inst files. I don't have a PS2/Xbox copy so I don't know if the tool works with them. You're welcome to try it and report back the results though.

## Output format
As is the case with gtamaptp-cullzone, the output file format is invented to best represent the binary data stored within entity.inst. Each line represents a given entity.
The vast majority of entities are assigned to an entity class, however there are some exceptions. These may still have an entity class assigned, but have no entity class data struct to accompany them.

Unlike IPL files, I chose to store all of the info on a single line, which would have a variable number of columns depending on the entity class.

It took a while for me to uncover all of this, and I could find virtually no pre-existing documentation online so I'm going to list as much info here as possible. 
I might have made mistakes / overlooked certain things so if you happen to know more I'd love to take in any issues / PRs to flesh things out further.

### Columns shared between all entity classes
All entity instances share the following columns:

```
<entity type>, <internal entity name>, <position X>, <position Y>, <position Z>, <rotation X, <rotation Y>, <rotation Z>, <rotation W>, <entity class>
```

| Identifier                | Description                                                                                  |
| :------------------------ | :------------------------------------------------------------------------------------------- |
| `<entity type>`           | The entity type. This is the internal model name specified in models.dff.                    |
| `<internal entity name>`  | The internal entity name. It may be used in scripts to refer to specific instances, and is thus expected to be unique. |
| `<position X>`            | Entity position X                                                                            |
| `<position Y>`            | Entity position Y                                                                            |
| `<position Z>`            | Entity position Z                                                                            |
| `<rotation X>`            | Entity rotation, stored as quaternion                                                        |
| `<rotation Y>`            | Entity rotation, stored as quaternion                                                        |
| `<rotation Z>`            | Entity rotation, stored as quaternion                                                        |
| `<rotation W>`            | Entity rotation, stored as quaternion                                                        |
| `<entity class>`          | Entity class. When compiled, different values are used, but I chose not to use them as there are multiple structs of differing sizes for a given class. A table describing each one of them is listed below. |

The following entity class names may be used:

| Entity class name         | Description                                                                                  |
| :------------------------ | :------------------------------------------------------------------------------------------- |
| `ENTITYCLASS_BASE_TYPE1`  | Will become `Base_Inst` once compiled, with a struct size of 4 bytes.                        |
| `ENTITYCLASS_BASE_TYPE2`  | Will become `Base_Inst` once compiled, with a struct size of 12 bytes.                       |
| `ENTITYCLASS_CAMERA`      | Will become `CameraData` once compiled, with a struct size of 20 bytes.                      |
| `ENTITYCLASS_DOOR_TYPE1`  | Will become `Door_Inst` once compiled, with a struct size of 12 bytes.                       |
| `ENTITYCLASS_DOOR_TYPE2`  | Will become `Door_Inst` once compiled, with a struct size of 20 bytes.                       |
| `ENTITYCLASS_HUNTER_TYPE1` | Will become `Hunter_Inst` once compiled, with a struct size of 32 bytes.                    |
| `ENTITYCLASS_HUNTER_TYPE2` | Will become `Hunter_Inst` once compiled, with a struct size of 36 bytes.                    |
| `ENTITYCLASS_LIGHT_TYPE1` | Will become `Light_Inst` once compiled, with a struct size of 40 bytes.                      |
| `ENTITYCLASS_LIGHT_TYPE2` | Will become `Light_Inst` once compiled, with a struct size of 60 bytes.                      |
| `ENTITYCLASS_PLAYER`      | Will become `Player_Inst` once compiled, with a struct size of 4 bytes.                      |
| `ENTITYCLASS_TRIGGER`     | Will become `Trigger_Inst` once compiled, with a struct size of 8 bytes.                     |

### Additional columns
Columns after `<entity class>` represent class data. The format of which is described in the chapters below.

#### ENTITYCLASS_BASE_TYPE1
Primarily used for prop placements around the level.

```
<hit points>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<hit points>`            | `int32_t`  | The amount of "hits" it takes to destroy this object. It seems a value of 100 is used as a maximum. |

#### ENTITYCLASS_BASE_TYPE2
Secondary format used for some props. Its purpose is unknown.

```
<hit points>, <unknown 1>, <unknown 2>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<hit points>`            | `int32_t`  | The amount of "hits" it takes to destroy this object. It seems a value of 100 is used as a maximum. |
| `<unknown 1>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 2>`             | `int32_t`  | Unknown.                                                                        |

#### ENTITYCLASS_BASE_CAMERA
Probably used for the camera cutscenes. The format is completely unknown.

```
<unknown 1>, <unknown 2>, <unknown 3>, <unknown 4>, <unknown 5>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<unknown 1>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 2>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 3>`             | `float`    | Unknown.                                                                        |
| `<unknown 4>`             | `float`    | Unknown.                                                                        |
| `<unknown 5>`             | `int32_t`  | Unknown.                                                                        |

#### ENTITYCLASS_DOOR_TYPE1
Used for doors that can be opened around the level.

```
<hit points>, <unknown>, <flags>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<hit points>`            | `int32_t`  | The amount of "hits" it takes to destroy the door.                              |
| `<unknown>`               | `int32_t`  | Unknown.                                                                        |
| `<flags>`                 | `int32_t`  | Based on the values stored, these definitely seem to be flags. I didn't uncover any specific flag values though. |

#### ENTITYCLASS_DOOR_TYPE2
Secondary struct used for doors. It contains a bit more data but unfortunately I haven't managed to uncover any of the values.
I wouldn't doubt it shares some values with `ENTITYCLASS_DOOR_TYPE` if I had to take a guess though.

```
<unknown 1>, <unknown 2>, <unknown 3>, <unknown 4>, <unknown 5>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<unknown 1>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 2>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 3>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 4>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 5>`             | `int32_t`  | Unknown.                                                                        |

#### ENTITYCLASS_HUNTER_TYPE1
First struct used to spawn hunters around a level. The data of which is completely unknown, however it may no doubt share some values with `ENTITYCLASS_HUNTER_TYPE2`.

```
<unknown 1>, <unknown 2>, <unknown 3>, <unknown 4>, <unknown 5>, <unknown 6>, <unknown 7>, <unknown 8>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<unknown 1>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 2>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 3>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 4>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 5>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 6>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 7>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 8>`             | `int32_t`  | Unknown.                                                                        |

#### ENTITYCLASS_HUNTER_TYPE2
Second struct describing hunter placements. This one is completely uncovered, with the exception of a few enums. 

```
<hit points>, <slot 1>, <slot 2>, <slot 3>, <melee weapon type>, <weapon type>, <behavior type>, <drop ammo>, <flags>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<hit points>`            | `int32_t`  | The number of "hits" it takes to kill this hunter. A value of 100 seems to denote full health. |
| `<slot 1>`                | `int32_t`  | The exact purpose of this is unknown. It doesn't appear to be tied to weapon slots. |
| `<slot 2>`                | `int32_t`  | The exact purpose of this is unknown. It doesn't appear to be tied to weapon slots. |
| `<slot 3>`                | `int32_t`  | The exact purpose of this is unknown. It doesn't appear to be tied to weapon slots. |
| `<melee weapon type>`     | `char[128]` | The melee weapon this hunter spawns with. Possible values are listed below.    |
| `<weapon type>`           | `char[128]` | The "normal" (that is- non-melee) weapon this hunter spawns with. Possible values are listed below. |
| `<behavior type>`         | `int32_t`  | The behavior type. Exact values are unknown.                                    |
| `<drop ammo>`             | `int32_t` | Probably a boolean. Doesn't seem to be used very often.                          |
| `<flags>`                 | `int32_t` | Flags. Possible values are unknown.                                              |

##### Melee weapon types
Internally, melee weapons are stored as an integer. To make lines more readable, they are converted to strings when decompiling. The internal ID is therefore listed alongside the weapon type string you can use in your .inst file.

| ID | Melee Weapon Type                |
|:---|:---------------------------------|
| 0  | MELEEWEAPON_PIPE                 |
| 1  | MELEEWEAPON_CLEAVER              |
| 2  | MELEEWEAPON_BASEBALLBAT_WOOD     |
| 3  | MELEEWEAPON_KNIFE                |
| 4  | MELEEWEAPON_BASEBALLBAT1         |
| 5  | MELEEWEAPON_BASEBALLBAT2         |
| 6  | MELEEWEAPON_UNKNOWN1             |
| 7  | MELEEWEAPON_CROWBAR              |
| 8  | MELEEWEAPON_SMALLBAT             |
| 9  | MELEEWEAPON_NIGHTSTICK           |
| 10 | MELEEWEAPON_AXE                  |
| 11 | MELEEWEAPON_ICEPICK              |
| 12 | MELEEWEAPON_MACHETE              |
| 13 | MELEEWEAPON_DANSICKLE            |
| 14 | MELEEWEAPON_BASEBALLBAT3         |
| 15 | MELEEWEAPON_SPIKEDBAT            |
| 16 | MELEEWEAPON_CHAINSAW             |
| 17 | MELEEWEAPON_SYRINGE              |
| 18 | MELEEWEAPON_SHOVEL               |
| 19 | MELEEWEAPON_SLEDGEHAMMER         |
| 20 | MELEEWEAPON_UNKNOWN2             |
| 21 | MELEEWEAPON_PEN                  |
| 22 | MELEEWEAPON_ACIDBOTTLE           |
| 23 | MELEEWEAPON_FIREARM_1HANDED      |
| 24 | MELEEWEAPON_FIREARM_2HANDED      |
| 25 | MELEEWEAPON_RAZOR                |
| 26 | MELEEWEAPON_BLOWTORCH            |
| 27 | MELEEWEAPON_MACE                 |
| 28 | MELEEWEAPON_HEDGETRIMMER         |
| 29 | MELEEWEAPON_METALHOOK            |
| 30 | MELEEWEAPON_CIRCULARSAW          |
| 31 | MELEEWEAPON_PLIERS               |
| 32 | MELEEWEAPON_TORCH                |
| 33 | MELEEWEAPON_NEWSPAPER            |
| 34 | MELEEWEAPON_BOTTLE               |
| 35 | MELEEWEAPON_DILDO                |
| 36 | MELEEWEAPON_KATANA               |
| 37 | MELEEWEAPON_HACKSAW              |
| 38 | MELEEWEAPON_FIREAXE              |

##### Weapon types
Like melee weapon types, "normal" weapon types are also stored as an integer in binary inst files. Possible values are as follows:

| ID | Weapon Type             |
|:---|:------------------------|
| 0  | WEAPONTYPE_GLOCK        |
| 1  | WEAPONTYPE_GLOCK2       |
| 2  | WEAPONTYPE_DEAGLE       |
| 3  | WEAPONTYPE_SHOTGUN1     |
| 4  | WEAPONTYPE_UZI          |
| 5  | WEAPONTYPE_COLT         |
| 6  | WEAPONTYPE_SNIPER       |
| 7  | WEAPONTYPE_UNKNOWN      |
| 8  | WEAPONTYPE_NAILGUN      |
| 9  | WEAPONTYPE_REVOLVER     |
| 10 | WEAPONTYPE_SAWNOFF      |
| 11 | WEAPONTYPE_TRANQILIZER  |
| 12 | WEAPONTYPE_SHOTGUN2     |
| 13 | WEAPONTYPE_SNIPER2      |
| 14 | WEAPONTYPE_FLARE        |
| 15 | WEAPONTYPE_CROSSBOW     |
| 16 | WEAPONTYPE_UZI2         |

#### ENTITYCLASS_LIGHT_TYPE1
The first struct used by objects that have 2dfx lights assigned to them. Seems to be used by "dynamic" lights such as searchlights. Other props like lampposts appear to use ENTITYCLASS_LIGHT_TYPE2.

```
<unknown 1>, <unknown 2>, <unknown 3>, <colour R>, <colour G>, <colour B>, <unknown 4>, <unknown 5>, <unknown 6>, <unknown 7>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<unknown 1>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 2>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 3>`             | `int32_t`  | Unknown.                                                                        |
| `<colour R>`              | `uint8_t`  | The light colour. It is internally stored as a float, but is converted to `uint8_t` when decompiling to enhance readability. |
| `<colour G>`              | `uint8_t`  | The light colour. It is internally stored as a float, but is converted to `uint8_t` when decompiling to enhance readability. |
| `<colour B>`              | `uint8_t`  | The light colour. It is internally stored as a float, but is converted to `uint8_t` when decompiling to enhance readability. |
| `<unknown 4>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 5>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 6>`             | `int32_t`  | Unknown.                                                                        |
| `<unknown 7>`             | `int32_t`  | Unknown.                                                                        |

#### ENTITYCLASS_LIGHT_TYPE2
The second struct is used by most other entities, like lampposts.

```
<type>, <cone angle>, <colour R>, <colour G>, <colour B>, <unknown 1>, <unknown 2>, <unknown 3>, <unknown 4>, <unknown 5>, <unknown 6>, <unknown 7>, <size>, <intensity>
```

| Identifier                | Type        | Description                                                                    |
| :------------------------ | :---------- | :----------------------------------------------------------------------------- |
| `<type>`                  | `int32_t`   | Unknown.                                                                       |
| `<cone angle>`            | `float`     | The angle of the light cone.                                                   |
| `<colour R>`              | `uint8_t`   | The light colour. It is internally stored as a float, but is converted to `uint8_t` when decompiling to enhance readability. |
| `<colour G>`              | `uint8_t`   | The light colour. It is internally stored as a float, but is converted to `uint8_t` when decompiling to enhance readability. |
| `<colour B>`              | `uint8_t`   | The light colour. It is internally stored as a float, but is converted to `uint8_t` when decompiling to enhance readability. |
| `<unknown 1>`             | `int32_t`   | Unknown. Appears to be a set of flags.                                         |
| `<unknown 2>`             | `int32_t`   | Unknown. Appears to be a set of flags.                                         |
| `<unknown 3>`             | `int32_t`   | Unknown. Appears to be a set of flags.                                         |
| `<unknown 4>`             | `int32_t`   | Unknown. Appears to be a set of flags.                                         |
| `<unknown 5>`             | `int32_t`   | Unknown. Appears to be a set of flags.                                         |
| `<unknown 6>`             | `int32_t`   | Unknown. Appears to be a set of flags.                                         |
| `<unknown 7>`             | `int32_t`   | Unknown. Appears to be a set of flags.                                         |
| `<size>`                  | `float`     | The size/radius of the light.                                                  |
| `<intensity>`             | `float`     | The intensity/brightness of the light.                                         |

#### ENTITYCLASS_PLAYER
Describes the spawn point for the player.

```
<unknown>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<unknown>`               | `int32_t`  | Unknown.                                                                        |

#### ENTITYCLASS_TRIGGER
Describes a trigger point, used by the level script.

```
<type>, <size>
```

| Identifier                | Type       | Description                                                                     |
| :------------------------ | :--------- | :------------------------------------------------------------------------------ |
| `<type>`                  | `int32_t`  | The trigger type. The possible values are unknown.                              |
| `<size>`                  | `float`    | The trigger size.                                                               |