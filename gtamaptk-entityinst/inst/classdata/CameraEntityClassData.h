#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryCameraEntityClassData {
public:
	int32_t unknown1;
	int32_t unknown2;
	float unknown3;
	float unknown4;
	int32_t unknown5;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryCameraEntityClassData, 0x14);

class CameraEntityClassData {
public:
	int32_t unknown1;
	int32_t unknown2;
	float unknown3;
	float unknown4;
	int32_t unknown5;

	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};