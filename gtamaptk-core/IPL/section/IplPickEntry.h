#pragma once

#include "../../main/MapFileSection.h"
#include "../../math/Vector3d.h"

class IplPickEntry : public IMapFileSectionEntry {
public:
	int16_t m_nWeaponType;
	Vector3d m_vPosition;

	int8_t GetType() { return IPL_SECTION_PICK; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};