#pragma once

#include "../../main/Shared.hpp"
#include "../../main/MapFileSection.h"

class IdeAnimEntry : public IMapFileSectionEntry {
public:
	int32_t m_nModelIndex;
	char m_szModelName[MAX_IMG_ENTRY_NAME];
	char m_szTxdName[MAX_IMG_ENTRY_NAME];
	char m_szIfpName[MAX_IMG_ENTRY_NAME];
	float m_fDrawDistance;
	uint32_t m_dwModelFlags;

	int8_t GetType() { return IDE_SECTION_ANIM; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};