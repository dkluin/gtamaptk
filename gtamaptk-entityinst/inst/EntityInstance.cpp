#include "EntityInstance.h"
#include "classdata/LightEntityClassData1.h"
#include "classdata/LightEntityClassData2.h"

const char* gszEntityClassNames[] = {
	"ENTITYCLASS_NONE",				// nothing. no entity class
	"ENTITYCLASS_BASE_TYPE1",		// Base_Inst
	"ENTITYCLASS_BASE_TYPE2",		// Base_Inst
	"ENTITYCLASS_CAMERA",			// CameraData
	"ENTITYCLASS_DOOR_TYPE1",		// Door_Inst
	"ENTITYCLASS_DOOR_TYPE2",		// Door_Inst
	"ENTITYCLASS_HUNTER_TYPE1",		// Hunter_Inst
	"ENTITYCLASS_HUNTER_TYPE2",		// Hunter_Inst
	"ENTITYCLASS_LIGHT_TYPE1",		// Light_Inst
	"ENTITYCLASS_LIGHT_TYPE2",		// Light_Inst
	"ENTITYCLASS_PLAYER",			// Player_Inst
	"ENTITYCLASS_TRIGGER"			// Trigger_Inst
};

const char* gszMeleeWeaponTypeNames[] = {
	"MELEEWEAPON_PIPE",
	"MELEEWEAPON_CLEAVER",
	"MELEEWEAPON_BASEBALLBAT_WOOD",
	"MELEEWEAPON_KNIFE",
	"MELEEWEAPON_BASEBALLBAT1",
	"MELEEWEAPON_BASEBALLBAT2",
	"MELEEWEAPON_UNKNOWN1",
	"MELEEWEAPON_CROWBAR",
	"MELEEWEAPON_SMALLBAT",
	"MELEEWEAPON_NIGHTSTICK",
	"MELEEWEAPON_AXE",
	"MELEEWEAPON_ICEPICK",
	"MELEEWEAPON_MACHETE",
	"MELEEWEAPON_SICKLE",
	"MELEEWEAPON_BASEBALLBAT3",
	"MELEEWEAPON_SPIKEDBAT",
	"MELEEWEAPON_CHAINSAW",
	"MELEEWEAPON_SYRINGE",
	"MELEEWEAPON_SHOVEL",
	"MELEEWEAPON_SLEDGEHAMMER",
	"MELEEWEAPON_UNKNOWN2",
	"MELEEWEAPON_PEN",
	"MELEEWEAPON_ACIDBOTTLE",
	"MELEEWEAPON_FIREARM_1HANDED",
	"MELEEWEAPON_FIREARM_2HANDED",
	"MELEEWEAPON_RAZOR",
	"MELEEWEAPON_BLOWTORCH",
	"MELEEWEAPON_MACE",
	"MELEEWEAPON_HEDGETRIMMER",
	"MELEEWEAPON_METALHOOK",
	"MELEEWEAPON_CIRCULARSAW",
	"MELEEWEAPON_PLIERS",
	"MELEEWEAPON_TORCH",
	"MELEEWEAPON_NEWSPAPER",
	"MELEEWEAPON_BOTTLE",
	"MELEEWEAPON_DILDO",
	"MELEEWEAPON_KATANA",
	"MELEEWEAPON_HACKSAW",
	"MELEEWEAPON_FIREAXE"
};

const char* gszWeaponTypeNames[] = {
	"WEAPONTYPE_GLOCK",
	"WEAPONTYPE_GLOCK2",
	"WEAPONTYPE_DEAGLE",
	"WEAPONTYPE_SHOTGUN1",
	"WEAPONTYPE_UZI",
	"WEAPONTYPE_COLT",
	"WEAPONTYPE_SNIPER",
	"WEAPONTYPE_UNKNOWN",
	"WEAPONTYPE_NAILGUN",
	"WEAPONTYPE_REVOLVER",
	"WEAPONTYPE_SAWNOFF",
	"WEAPONTYPE_TRANQILIZER",
	"WEAPONTYPE_SHOTGUN2",
	"WEAPONTYPE_SNIPER2",
	"WEAPONTYPE_FLARE",
	"WEAPONTYPE_CROSSBOW",
	"WEAPONTYPE_UZI2"
};

int32_t FindEntityClass(const char* szEntityClassName) {
	for (int32_t i = 0; i < sizeof(gszEntityClassNames) / sizeof(gszEntityClassNames[0]); i++) {
		if (!strcmp(szEntityClassName, gszEntityClassNames[i])) {
			return i;
		}
	}

	return -1;
}

int32_t FindMeleeWeaponType(const char* szMeleeWeaponTypeName) {
	for (int32_t i = 0; i < sizeof(gszMeleeWeaponTypeNames) / sizeof(gszMeleeWeaponTypeNames[0]); i++) {
		if (!strcmp(szMeleeWeaponTypeName, gszMeleeWeaponTypeNames[i])) {
			return i;
		}
	}

	return -1;
}

int32_t FindWeaponType(const char* szWeaponTypeName) {
	for (int32_t i = 0; i < sizeof(gszWeaponTypeNames) / sizeof(gszWeaponTypeNames[0]); i++) {
		if (!strcmp(szWeaponTypeName, gszWeaponTypeNames[i])) {
			return i;
		}
	}

	return -1;
}

void EntityInstance::ReadPaddedString(FILE* pFile, char* szDestination, int32_t nMaxLength) {
	int32_t nNumPaddingBytesToSkip;
	char szTmpStr[MAX_PADDED_STRING];

	memset(szTmpStr, NULL, sizeof(szTmpStr));

	for (int32_t i = 0; i < MAX_PADDED_STRING; i++) {
		fread(&szTmpStr[i], sizeof(char), 1, pFile);

		if (szTmpStr[i] == '\0') break;
	}

	// If the string happens to somehow not fit, cap it (should never happen)
	szTmpStr[MAX_PADDED_STRING - 1] = '\0';

	// Skip the padding bytes
	nNumPaddingBytesToSkip = PADDED_STRING_ALIGNTO - (ftell(pFile) % PADDED_STRING_ALIGNTO);

	if (nNumPaddingBytesToSkip < PADDED_STRING_ALIGNTO) {
		fseek(pFile, nNumPaddingBytesToSkip, SEEK_CUR);
	}

	strcpy(szDestination, szTmpStr);
}

void EntityInstance::WritePaddedString(FILE* pFile, const char* szSource, int32_t nMaxLength) {
	int32_t nNumPaddingBytesToWrite;
	char szPaddingBytes[PADDED_STRING_ALIGNTO];

	memset(szPaddingBytes, 0, sizeof(szPaddingBytes));
	fwrite(szSource, strlen(szSource) + 1, 1, pFile);

	nNumPaddingBytesToWrite = PADDED_STRING_ALIGNTO - (ftell(pFile) % PADDED_STRING_ALIGNTO);

	if (nNumPaddingBytesToWrite < PADDED_STRING_ALIGNTO) {
		fwrite(szPaddingBytes, sizeof(uint8_t), nNumPaddingBytesToWrite, pFile);
	}
}

int32_t EntityInstance::GetPaddedStringSize(int32_t nPosInFile, const char* szStr, int32_t nMaxLength)
{
	int32_t nNumPaddingBytesToWrite;

	nNumPaddingBytesToWrite = PADDED_STRING_ALIGNTO - ((nPosInFile + strlen(szStr) + 1) % PADDED_STRING_ALIGNTO);
	if (nNumPaddingBytesToWrite == PADDED_STRING_ALIGNTO) nNumPaddingBytesToWrite = 0;

	return strlen(szStr) + 1 + nNumPaddingBytesToWrite;
}

const char* EntityInstance::GetBinaryEntityClassName() {
	static const char* gszEntityClassTable[] = {
		"Base_Inst",	// ENTITYCLASS_BASE_TYPE1
		"Base_Inst",	// ENTITYCLASS_BASE_TYPE2
		"CameraData",	// ENTITYCLASS_CAMERA
		"Door_Inst",	// ENTITYCLASS_DOOR_TYPE1
		"Door_Inst",	// ENTITYCLASS_DOOR_TYPE2
		"Hunter_Inst",	// ENTITYCLASS_HUNTER_TYPE1
		"Hunter_Inst",	// ENTITYCLASS_HUNTER_TYPE2
		"Light_Inst",	// ENTITYCLASS_LIGHT_TYPE1
		"Light_Inst",	// ENTITYCLASS_LIGHT_TYPE2
		"Player_Inst",	// ENTITYCLASS_PLAYER
		"Trigger_Inst"	// ENTITYCLASS_TRIGGER
	};

	assert(m_nEntityClass != ENTITYCLASS_NONE && m_nEntityClass - 1 < sizeof(gszEntityClassTable) / sizeof(gszEntityClassTable[0]));
	return gszEntityClassTable[m_nEntityClass - 1];
}

bool EntityInstance::ReadEntityClassData(int32_t nEntityClass, const char* szLine) {
	switch (nEntityClass) {
		case ENTITYCLASS_BASE_TYPE1:
		{
			BaseEntityClassData1 mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mBaseEntityClassData1 = mClassData;
		}
		break;

		case ENTITYCLASS_BASE_TYPE2:
		{
			BaseEntityClassData2 mClassData;
				
			if (!mClassData.Read(szLine)) return false;
			m_mBaseEntityClassData2 = mClassData;
		}
		break;

		case ENTITYCLASS_CAMERA:
		{
			CameraEntityClassData mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mCameraEntityClassData = mClassData;
		}
		break;

		case ENTITYCLASS_DOOR_TYPE1:
		{
			DoorEntityClassData1 mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mDoorEntityClassData1 = mClassData;
		}
		break;

		case ENTITYCLASS_DOOR_TYPE2:
		{
			DoorEntityClassData2 mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mDoorEntityClassData2 = mClassData;
		}
		break;

		case ENTITYCLASS_HUNTER_TYPE1:
		{
			HunterEntityClassData1 mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mHunterEntityClassData1 = mClassData;
		}
		break;

		case ENTITYCLASS_HUNTER_TYPE2:
		{
			HunterEntityClassData2 mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mHunterEntityClassData2 = mClassData;
		}
		break;

		case ENTITYCLASS_LIGHT_TYPE1:
		{
			LightEntityClassData1 mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mLightEntityClassData1 = mClassData;
		}
		break;

		case ENTITYCLASS_LIGHT_TYPE2:
		{
			LightEntityClassData2 mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mLightEntityClassData2 = mClassData;
		}
		break;

		case ENTITYCLASS_PLAYER:
		{
			PlayerEntityClassData mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mPlayerEntityClassData = mClassData;
		}
		break;

		case ENTITYCLASS_TRIGGER:
		{
			TriggerEntityClassData mClassData;

			if (!mClassData.Read(szLine)) return false;
			m_mTriggerEntityClassData = mClassData;
		}
		break;
	}

	return true;
}

int32_t EntityInstance::DetermineEntityClassFromStructData(const char* szEntityClassName, int32_t nStructSize) {
	int32_t nEntityClass = ENTITYCLASS_NONE;

	if (!strcmp(szEntityClassName, "Base_Inst")) {
		if (nStructSize == sizeof(BinaryBaseEntityClassData1))
			nEntityClass = ENTITYCLASS_BASE_TYPE1;
		else if (nStructSize == sizeof(BinaryBaseEntityClassData2))
			nEntityClass = ENTITYCLASS_BASE_TYPE2;
	} else if (!strcmp(szEntityClassName, "CameraData"))
		nEntityClass = ENTITYCLASS_CAMERA;
	else if (!strcmp(szEntityClassName, "Door_Inst")) {
		if (nStructSize == sizeof(BinaryDoorEntityClassData1))
			nEntityClass = ENTITYCLASS_DOOR_TYPE1;
		else if (nStructSize == sizeof(BinaryDoorEntityClassData2))
			nEntityClass = ENTITYCLASS_DOOR_TYPE2;
	} else if (!strcmp(szEntityClassName, "Hunter_Inst")) {
		if (nStructSize == sizeof(BinaryHunterEntityClassData1))
			nEntityClass = ENTITYCLASS_HUNTER_TYPE1;
		else if (nStructSize == sizeof(BinaryHunterEntityClassData2))
			nEntityClass = ENTITYCLASS_HUNTER_TYPE2;
	} else if (!strcmp(szEntityClassName, "Light_Inst")) {
		if (nStructSize == sizeof(BinaryLightEntityClassData1))
			nEntityClass = ENTITYCLASS_LIGHT_TYPE1;
		else if (nStructSize == sizeof(BinaryLightEntityClassData2))
			nEntityClass = ENTITYCLASS_LIGHT_TYPE2;
	} else if (!strcmp(szEntityClassName, "Player_Inst"))
		nEntityClass = ENTITYCLASS_PLAYER;
	else if (!strcmp(szEntityClassName, "Trigger_Inst"))
		nEntityClass = ENTITYCLASS_TRIGGER;

	return nEntityClass;
}

bool EntityInstance::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	int32_t nEntityClass;
	Vector3d vPosition;
	Quaternion qRotation;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f", 
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w) == 10) {
		nEntityClass = FindEntityClass(szEntityClassName);

		if (nEntityClass == -1) {
			SetLastErrorMsg("Error: Unknown entity class: %s in line \"%s\"", szEntityClassName, szLine);
			return false;
		}

		if (!ReadEntityClassData(nEntityClass, szLine)) {
			return false;
		}
		
		// doing this at the end to prevent setting the entity data if the class data is invalid
		strcpy(m_szEntityType, szEntityType);
		strcpy(m_szInternalEntityName, szInternalEntityName);
		m_vPosition = vPosition;
		m_qRotation = qRotation;
		m_nEntityClass = nEntityClass;
		return true;
	}

	SetLastErrorMsg("Error: Invalid entity instance line: \"%s\"", szLine);
	return false;
}

bool EntityInstance::ReadBinary(FILE* pFile, int32_t nEndOfStruct) {
	static int32_t gnEntityClassSizes[] = {
		0, // ENTITYCLASS_NONE
		sizeof(BinaryBaseEntityClassData1), // ENTITYCLASS_BASE_TYPE1
		sizeof(BinaryBaseEntityClassData2),	// ENTITYCLASS_BASE_TYPE2
		sizeof(BinaryCameraEntityClassData), // ENTITYCLASS_CAMERA
		sizeof(BinaryDoorEntityClassData1), // ENTITYCLASS_DOOR_TYPE1
		sizeof(BinaryDoorEntityClassData2), // ENTITYCLASS_DOOR_TYPE2
		sizeof(BinaryHunterEntityClassData1), // ENTITYCLASS_HUNTER_TYPE1
		sizeof(BinaryHunterEntityClassData2), // ENTITYCLASS_HUNTER_TYPE2
		sizeof(BinaryLightEntityClassData1), // ENTITYCLASS_LIGHT_TYPE1
		sizeof(BinaryLightEntityClassData2), // ENTITYCLASS_LIGHT_TYPE2
		sizeof(BinaryPlayerEntityClassData), // ENTITYCLASS_PLAYER
		sizeof(BinaryTriggerEntityClassData) // ENTITYCLASS_TRIGGER
	};

	char szEntityClassName[MAX_PADDED_STRING];
	int32_t nSizeOfClassStruct;

	ReadPaddedString(pFile, m_szEntityType, MAX_PADDED_STRING);
	ReadPaddedString(pFile, m_szInternalEntityName, MAX_PADDED_STRING);

	fread(&m_vPosition, sizeof(m_vPosition), 1, pFile);
	fread(&m_qRotation, sizeof(m_qRotation), 1, pFile);

	ReadPaddedString(pFile, szEntityClassName, MAX_PADDED_STRING);

	m_nEntityClass = ENTITYCLASS_NONE;
	nSizeOfClassStruct = nEndOfStruct - ftell(pFile);

	// Some entities have no class data
	if (nSizeOfClassStruct > 0) {
		int32_t nTmpEntityClass = DetermineEntityClassFromStructData(szEntityClassName, nSizeOfClassStruct);

		if (nTmpEntityClass == ENTITYCLASS_NONE) {
			SetLastErrorMsg("Error: Unknown entity class: %s", szEntityClassName);

			fseek(pFile, nSizeOfClassStruct, SEEK_CUR);// Skip this section, go to next line
			return false;
		}

		// Failsafe to detect unknown entity class structs; the size is a nice safeguard we are using here
		m_nEntityClass = nTmpEntityClass;
		assert(nSizeOfClassStruct == gnEntityClassSizes[m_nEntityClass]);

		switch (m_nEntityClass) {
			case ENTITYCLASS_BASE_TYPE1: m_mBaseEntityClassData1.ReadBinary(pFile); break;
			case ENTITYCLASS_BASE_TYPE2: m_mBaseEntityClassData2.ReadBinary(pFile); break;
			case ENTITYCLASS_CAMERA: m_mCameraEntityClassData.ReadBinary(pFile); break;
			case ENTITYCLASS_DOOR_TYPE1: m_mDoorEntityClassData1.ReadBinary(pFile); break;
			case ENTITYCLASS_DOOR_TYPE2: m_mDoorEntityClassData2.ReadBinary(pFile); break;
			case ENTITYCLASS_HUNTER_TYPE1: m_mHunterEntityClassData1.ReadBinary(pFile); break;
			case ENTITYCLASS_HUNTER_TYPE2: m_mHunterEntityClassData2.ReadBinary(pFile); break;
			case ENTITYCLASS_LIGHT_TYPE1: m_mLightEntityClassData1.ReadBinary(pFile); break;
			case ENTITYCLASS_LIGHT_TYPE2: m_mLightEntityClassData2.ReadBinary(pFile); break;
			case ENTITYCLASS_PLAYER: m_mPlayerEntityClassData.ReadBinary(pFile); break;
			case ENTITYCLASS_TRIGGER: m_mTriggerEntityClassData.ReadBinary(pFile); break;
		}
	}

	return true;
}

bool EntityInstance::Write(FILE* pFile) {
	fprintf(pFile, "%s, %s, %s, %.8g, %.8g, %.8g, %.10g, %.10g, %.10g, %.10g, ",
		m_szEntityType, m_szInternalEntityName, gszEntityClassNames[m_nEntityClass],
		m_vPosition.x, m_vPosition.y, m_vPosition.z,
		m_qRotation.x, m_qRotation.y, m_qRotation.z, m_qRotation.w);

	switch (m_nEntityClass) {
	case ENTITYCLASS_BASE_TYPE1: m_mBaseEntityClassData1.Write(pFile); break;
	case ENTITYCLASS_BASE_TYPE2: m_mBaseEntityClassData2.Write(pFile); break;
	case ENTITYCLASS_CAMERA: m_mCameraEntityClassData.Write(pFile); break;
	case ENTITYCLASS_DOOR_TYPE1: m_mDoorEntityClassData1.Write(pFile); break;
	case ENTITYCLASS_DOOR_TYPE2: m_mDoorEntityClassData1.Write(pFile); break;
	case ENTITYCLASS_HUNTER_TYPE1: m_mHunterEntityClassData1.Write(pFile); break;
	case ENTITYCLASS_HUNTER_TYPE2: m_mHunterEntityClassData2.Write(pFile); break;
	case ENTITYCLASS_LIGHT_TYPE1: m_mLightEntityClassData1.Write(pFile); break;
	case ENTITYCLASS_LIGHT_TYPE2: m_mLightEntityClassData2.Write(pFile); break;
	case ENTITYCLASS_PLAYER: m_mPlayerEntityClassData.Write(pFile); break;
	case ENTITYCLASS_TRIGGER: m_mTriggerEntityClassData.Write(pFile); break;
	}

	fprintf(pFile, "\n");
	return true;
}

bool EntityInstance::WriteBinary(FILE* pFile) {
	WritePaddedString(pFile, m_szEntityType, MAX_PADDED_STRING);
	WritePaddedString(pFile, m_szInternalEntityName, MAX_PADDED_STRING);

	fwrite(&m_vPosition, sizeof(m_vPosition), 1, pFile);
	fwrite(&m_qRotation, sizeof(m_qRotation), 1, pFile);

	WritePaddedString(pFile, GetBinaryEntityClassName(), MAX_PADDED_STRING);

	switch (m_nEntityClass) {
	case ENTITYCLASS_BASE_TYPE1: m_mBaseEntityClassData1.WriteBinary(pFile); break;
	case ENTITYCLASS_BASE_TYPE2: m_mBaseEntityClassData2.WriteBinary(pFile); break;
	case ENTITYCLASS_CAMERA: m_mCameraEntityClassData.WriteBinary(pFile); break;
	case ENTITYCLASS_DOOR_TYPE1: m_mDoorEntityClassData1.WriteBinary(pFile); break;
	case ENTITYCLASS_DOOR_TYPE2: m_mDoorEntityClassData1.WriteBinary(pFile); break;
	case ENTITYCLASS_HUNTER_TYPE1: m_mHunterEntityClassData1.WriteBinary(pFile); break;
	case ENTITYCLASS_HUNTER_TYPE2: m_mHunterEntityClassData2.WriteBinary(pFile); break;
	case ENTITYCLASS_LIGHT_TYPE1: m_mLightEntityClassData1.WriteBinary(pFile); break;
	case ENTITYCLASS_LIGHT_TYPE2: m_mLightEntityClassData2.WriteBinary(pFile); break;
	case ENTITYCLASS_PLAYER: m_mPlayerEntityClassData.WriteBinary(pFile); break;
	case ENTITYCLASS_TRIGGER: m_mTriggerEntityClassData.WriteBinary(pFile); break;
	}

	return true;
}

int32_t EntityInstance::GetBinarySize(int32_t nExpectedStartPosn) {
	int32_t nPaddedStringSize;
	int32_t nExpectedPosInFile;
	int32_t nSize = 0;

	nExpectedPosInFile = nExpectedStartPosn;

	nPaddedStringSize = GetPaddedStringSize(nExpectedPosInFile, m_szEntityType, MAX_PADDED_STRING);
	nExpectedPosInFile += nPaddedStringSize;
	nSize += nPaddedStringSize;

	nPaddedStringSize = GetPaddedStringSize(nExpectedPosInFile, m_szInternalEntityName, MAX_PADDED_STRING);
	nExpectedPosInFile += nPaddedStringSize;
	nSize += nPaddedStringSize;

	nSize += sizeof(m_vPosition);
	nExpectedPosInFile += sizeof(m_vPosition);
	nSize += sizeof(m_qRotation);
	nExpectedPosInFile += sizeof(m_qRotation);

	nPaddedStringSize = GetPaddedStringSize(nExpectedPosInFile, GetBinaryEntityClassName(), MAX_PADDED_STRING);
	nSize += nPaddedStringSize;

	switch (m_nEntityClass) {
	case ENTITYCLASS_BASE_TYPE1: nSize += sizeof(BinaryBaseEntityClassData1); break;
	case ENTITYCLASS_BASE_TYPE2: nSize += sizeof(BinaryBaseEntityClassData2); break;
	case ENTITYCLASS_CAMERA: nSize += sizeof(BinaryCameraEntityClassData); break;
	case ENTITYCLASS_DOOR_TYPE1: nSize += sizeof(BinaryDoorEntityClassData1); break;
	case ENTITYCLASS_DOOR_TYPE2: nSize += sizeof(BinaryDoorEntityClassData2); break;
	case ENTITYCLASS_HUNTER_TYPE1: nSize += sizeof(BinaryHunterEntityClassData1); break;
	case ENTITYCLASS_HUNTER_TYPE2: nSize += sizeof(BinaryHunterEntityClassData2); break;
	case ENTITYCLASS_LIGHT_TYPE1: nSize += sizeof(BinaryLightEntityClassData1); break;
	case ENTITYCLASS_LIGHT_TYPE2: nSize += sizeof(BinaryLightEntityClassData2); break;
	case ENTITYCLASS_PLAYER: nSize += sizeof(BinaryPlayerEntityClassData); break;
	case ENTITYCLASS_TRIGGER: nSize += sizeof(BinaryTriggerEntityClassData); break;
	}

	return nSize;
}