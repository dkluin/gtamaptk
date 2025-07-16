#pragma once

#include <gtamaptk-core.h>
#include "classdata/BaseEntityClassData1.h"
#include "classdata/BaseEntityClassData2.h"
#include "classdata/CameraEntityClassData.h"
#include "classdata/DoorEntityClassData1.h"
#include "classdata/DoorEntityClassData2.h"
#include "classdata/HunterEntityClassData1.h"
#include "classdata/HunterEntityClassData2.h"
#include "classdata/LightEntityClassData1.h"
#include "classdata/LightEntityClassData2.h"
#include "classdata/PlayerEntityClassData.h"
#include "classdata/TriggerEntityClassData.h"

// I used 128 as the maximum size in the old entity.inst -> entity.ipl converter so I'm just going to assume this as the maximum length a model name can have
#define MAX_PADDED_STRING 128
#define PADDED_STRING_ALIGNTO 4 // padded strings are aliged to 4 bytes relative to the entire file

enum eEntityClass {
	ENTITYCLASS_NONE = 0,
	ENTITYCLASS_BASE_TYPE1,
	ENTITYCLASS_BASE_TYPE2,
	ENTITYCLASS_CAMERA,
	ENTITYCLASS_DOOR_TYPE1,
	ENTITYCLASS_DOOR_TYPE2,
	ENTITYCLASS_HUNTER_TYPE1,
	ENTITYCLASS_HUNTER_TYPE2,
	ENTITYCLASS_LIGHT_TYPE1,
	ENTITYCLASS_LIGHT_TYPE2,
	ENTITYCLASS_PLAYER,
	ENTITYCLASS_TRIGGER
};

enum eMeleeWeaponType {
	MELEEWEAPON_PIPE,
	MELEEWEAPON_CLEAVER,
	MELEEWEAPON_BASEBALLBAT_WOOD,
	MELEEWEAPON_KNIFE,
	MELEEWEAPON_BASEBALLBAT1,
	MELEEWEAPON_BASEBALLBAT2,
	MELEEWEAPON_UNKNOWN1,
	MELEEWEAPON_CROWBAR,
	MELEEWEAPON_SMALLBAT,
	MELEEWEAPON_NIGHTSTICK,
	MELEEWEAPON_AXE,
	MELEEWEAPON_ICEPICK,
	MELEEWEAPON_MACHETE,
	MELEEWEAPON_DANSICKLE,
	MELEEWEAPON_BASEBALLBAT3,
	MELEEWEAPON_SPIKEDBAT,
	MELEEWEAPON_CHAINSAW,
	MELEEWEAPON_SYRINGE,
	MELEEWEAPON_SHOVEL,
	MELEEWEAPON_SLEDGEHAMMER,
	MELEEWEAPON_UNKNOWN2,
	MELEEWEAPON_PEN,
	MELEEWEAPON_ACIDBOTTLE,
	MELEEWEAPON_FIREARM_1HANDED,
	MELEEWEAPON_FIREARM_2HANDED,
	MELEEWEAPON_RAZOR,
	MELEEWEAPON_BLOWTORCH,
	MELEEWEAPON_MACE,
	MELEEWEAPON_HEDGETRIMMER,
	MELEEWEAPON_METALHOOK,
	MELEEWEAPON_CIRCULARSAW,
	MELEEWEAPON_PLIERS,
	MELEEWEAPON_TORCH,
	MELEEWEAPON_NEWSPAPER,
	MELEEWEAPON_BOTTLE,
	MELEEWEAPON_DILDO,
	MELEEWEAPON_KATANA,
	MELEEWEAPON_HACKSAW,
	MELEEWEAPON_FIREAXE
};

enum eWeaponType {
	WEAPONTYPE_GLOCK,
	WEAPONTYPE_GLOCK2,
	WEAPONTYPE_DEAGLE,
	WEAPONTYPE_SHOTGUN1,
	WEAPONTYPE_UZI,
	WEAPONTYPE_COLT,
	WEAPONTYPE_SNIPER,
	WEAPONTYPE_UNKNOWN,
	WEAPONTYPE_NAILGUN,
	WEAPONTYPE_REVOLVER,
	WEAPONTYPE_SAWNOFF,
	WEAPONTYPE_TRANQILIZER,
	WEAPONTYPE_SHOTGUN2,
	WEAPONTYPE_SNIPER2,
	WEAPONTYPE_FLARE,
	WEAPONTYPE_CROSSBOW,
	WEAPONTYPE_UZI2
};

class EntityInstance {
public:
	char m_szEntityType[MAX_PADDED_STRING];
	char m_szInternalEntityName[MAX_PADDED_STRING];
	int8_t m_nEntityClass;
	Vector3d m_vPosition;
	Quaternion m_qRotation;

	union {
		BaseEntityClassData1 m_mBaseEntityClassData1;
		BaseEntityClassData2 m_mBaseEntityClassData2;
		CameraEntityClassData m_mCameraEntityClassData;
		DoorEntityClassData1 m_mDoorEntityClassData1;
		DoorEntityClassData2 m_mDoorEntityClassData2;
		HunterEntityClassData1 m_mHunterEntityClassData1;
		HunterEntityClassData2 m_mHunterEntityClassData2;
		LightEntityClassData1 m_mLightEntityClassData1;
		LightEntityClassData2 m_mLightEntityClassData2;
		PlayerEntityClassData m_mPlayerEntityClassData;
		TriggerEntityClassData m_mTriggerEntityClassData;
	};

private:
	void ReadPaddedString(FILE* pFile, char* szDestination, int32_t nMaxLength);
	void WritePaddedString(FILE* pFile, const char* szSource, int32_t nMaxLength);
	int32_t GetPaddedStringSize(int32_t nPosInFile, const char* szStr, int32_t nMaxLength);

	const char* GetBinaryEntityClassName();
	bool ReadEntityClassData(int32_t nEntityClass, const char* szLine);
	int32_t DetermineEntityClassFromStructData(const char* szEntityClassName, int32_t nStructSize);

public:
	bool Read(const char* szLine);
	bool ReadBinary(FILE* pFile, int32_t nEndOfStruct);
	bool Write(FILE* pFile);
	bool WriteBinary(FILE* pFile);

	int32_t GetBinarySize(int32_t nExpectedStartPosn); // calculate the size of this instance for a given start offset
};

extern const char* gszEntityClassNames[];
extern const char* gszMeleeWeaponTypeNames[];
extern const char* gszWeaponTypeNames[];

extern int32_t FindEntityClass(const char* szEntityClassName);
extern int32_t FindMeleeWeaponType(const char* szMeleeWeaponTypeName);
extern int32_t FindWeaponType(const char* szWeaponTypeName);