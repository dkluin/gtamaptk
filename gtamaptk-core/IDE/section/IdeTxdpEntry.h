#pragma once

#include "../../main/Shared.hpp"
#include "../../main/MapFileSection.h"

class IdeTxdpEntry : public IMapFileSectionEntry {
public:
	char m_szTxdName[MAX_IMG_ENTRY_NAME];
	char m_szParentTxdName[MAX_IMG_ENTRY_NAME];

	int8_t GetType() { return IDE_SECTION_TXDP; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};