#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryPlayerEntityClassData {
public:
	int32_t m_nHitPoints;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryPlayerEntityClassData, 0x4);

class PlayerEntityClassData {
public:
	int32_t m_nHitPoints;
	
	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};