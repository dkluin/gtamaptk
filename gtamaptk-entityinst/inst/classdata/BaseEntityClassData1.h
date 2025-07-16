#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryBaseEntityClassData1 {
public:
	int32_t m_nHitPoints;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryBaseEntityClassData1, 0x4);

class BaseEntityClassData1 {
public:
	int32_t m_nHitPoints;

	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};