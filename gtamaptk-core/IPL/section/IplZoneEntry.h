#pragma once

#include "../../main/MapFileSection.h"

class IplZoneEntry : public IMapFileSectionEntry {
public:
	// ...

	int8_t GetType() { return IPL_SECTION_ZONE; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};