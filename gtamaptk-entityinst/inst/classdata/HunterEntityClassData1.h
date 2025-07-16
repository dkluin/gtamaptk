#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryHunterEntityClassData1 {
public:
	int32_t unknown1;
	int32_t	unknown2;
	int32_t	unknown3;
	int32_t	unknown4;
	int32_t	unknown5;
	int32_t	unknown6;
	int32_t	unknown7;
	int32_t	unknown8;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryHunterEntityClassData1, 32);

class HunterEntityClassData1 {
public:
	int32_t	unknown1;
	int32_t	unknown2;
	int32_t	unknown3;
	int32_t	unknown4;
	int32_t	unknown5;
	int32_t	unknown6;
	int32_t	unknown7;
	int32_t unknown8;
	
	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};