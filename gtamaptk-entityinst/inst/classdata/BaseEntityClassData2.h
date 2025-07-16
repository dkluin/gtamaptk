#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryBaseEntityClassData2 {
public:
	int32_t m_nHitPoints;
	int32_t unknown1;
	int32_t unknown2;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryBaseEntityClassData2, 0xC);

class BaseEntityClassData2 {
public:
	int32_t m_nHitPoints;
	int32_t unknown1;
	int32_t unknown2;

	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};