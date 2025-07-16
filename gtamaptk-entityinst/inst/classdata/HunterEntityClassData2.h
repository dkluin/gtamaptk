#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryHunterEntityClassData2 {
public:
	int32_t m_nHitPoints;
	int32_t m_nSlots[3];
	int32_t m_nMeleeWeaponType;
	int32_t m_nWeaponType;
	int32_t m_nBehaviorType;// like ped stats
	int32_t m_nDropAmmo;
	int32_t m_nFlags;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryHunterEntityClassData2, 36);

class HunterEntityClassData2 {
public:
	int32_t m_nHitPoints;
	int32_t m_nSlots[3];
	int32_t m_nMeleeWeaponType;
	int32_t m_nWeaponType;
	int32_t m_nBehaviorType; // like ped stats
	int32_t m_nDropAmmo;
	int32_t m_nFlags;

	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};