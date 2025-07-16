#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryDoorEntityClassData1 {
public:
	int32_t m_nHitPoints;
	int32_t unknown;
	int32_t m_nFlags; // No idea what these are... but they're flags alright!
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryDoorEntityClassData1, 0xC);

class DoorEntityClassData1 {
public:
	int32_t m_nHitPoints;
	int32_t unknown;
	int32_t m_nFlags;

	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};