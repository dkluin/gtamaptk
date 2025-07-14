#pragma once

#include "../../math/Vector3d.h"
#include "../../math/Misc.h"
#include "../../main/MapFileSection.h"
#include <Windows.h>

#pragma pack(push, 1)
class BinaryIplCarsData {
public:
	Vector3d m_vPosition;
	RadianAngle m_fRotation;
	int32_t m_nModelIndex;
	int32_t m_nPrimaryColour;
	int32_t m_nSecondaryColour;
	BOOL m_bForceSpawn;
	int32_t m_nAlarmChance;
	int32_t m_nLockedChance;
	int32_t m_nMinDelay;
	int32_t m_nMaxDelay;
};
#pragma pack(pop)

class IplCarsEntry : public IMapFileSectionEntry {
public:
	Vector3d m_vPosition;
	RadianAngle m_fRotation;
	int32_t m_nModelIndex;
	int32_t m_nPrimaryColour;
	int32_t m_nSecondaryColour;
	bool m_bForceSpawn;
	int8_t m_nAlarmChance;
	int8_t m_nLockedChance;
	int16_t m_nMinDelay;
	int16_t m_nMaxDelay;

	int8_t GetType() { return IPL_SECTION_CARS; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};