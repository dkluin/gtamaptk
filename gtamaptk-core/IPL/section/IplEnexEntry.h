#pragma once

#include "../../main/MapFileSection.h"
#include "../../math/Misc.h"
#include "../../main/Shared.hpp"
#include "../../math/Vector3d.h"

enum eEnexFlags {
	IPLENEX_UNKNOWN1 = 1,
	IPLENEX_UNKNOWN2 = 2,
	IPLENEX_LINKED = 4,
	IPLENEX_REWARD = 8,
	IPLENEX_REWARD_USED = 16,
	IPLENEX_CARS_AIRCRAFT = 32,
	IPLENEX_BIKES_MOTORCYCLES = 64,
	IPLENEX_DISABLE_ONFOOT = 128,
	IPLENEX_ALLOW_PLAYER_GROUP = 256,
	IPLENEX_DATE_INTERIOR = 512,
	IPLENEX_UNKNOWN3 = 1024,
	IPLENEX_DISABLE_EXIT = 2048,
	IPLENEX_BURGLARY_ACCESS = 4096,
	IPLENEX_ENTERED_WITHOUT_EXIT = 8192,
	IPLENEX_ENABLED_BY_DEFAULT = 16384,
	IPLENEX_DELETE_WHEN_USED = 32768
};

class IplEnexEntry : public IMapFileSectionEntry {
public:
	Vector3d m_vEntrancePos;
	EulerAngle m_fEntranceRot;
	Vector3d m_vSize;
	Vector3d m_vExitPos;
	EulerAngle m_fExitRot;
	uint8_t m_nTargetAreaID;
	uint16_t m_nFlags;
	char m_szName[MAX_GXT_ENTRY_NAME];
	int8_t m_nExtraCol;
	int8_t m_nNumPedsToSpawn;
	int8_t m_nTimeOn;
	int8_t m_nTimeOff;

	int8_t GetType() { return IPL_SECTION_ENEX; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};