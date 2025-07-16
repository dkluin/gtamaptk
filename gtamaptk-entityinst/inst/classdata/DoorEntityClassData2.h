#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryDoorEntityClassData2 {
public:
	int32_t unknown1;
	int32_t unknown2;
	int32_t unknown3;
	int32_t unknown4;
	int32_t unknown5;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryDoorEntityClassData2, 20);

class DoorEntityClassData2 {
public:
	int32_t unknown1;
	int32_t unknown2;
	int32_t unknown3;
	int32_t unknown4;
	int32_t unknown5;

	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};