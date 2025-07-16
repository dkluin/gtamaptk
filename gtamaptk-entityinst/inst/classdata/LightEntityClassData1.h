#pragma once

#include <gtamaptk-core.h>

// Used in some levels, seems to be used for dynamic lights (e.g. search lights)
#pragma pack(push, 1)
class BinaryLightEntityClassData1 {
public:
	int32_t unknown1;
	int32_t unknown2;
	int32_t unknown3;
	float m_fColour[3];
	int32_t unknown4;
	int32_t unknown5;
	int32_t unknown6;
	int32_t unknown7;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryLightEntityClassData1, 40);

class LightEntityClassData1 {
public:
	int32_t unknown1;
	int32_t unknown2;
	int32_t unknown3;
	float m_fColour[3];
	int32_t unknown4;
	int32_t unknown5;
	int32_t unknown6;
	int32_t unknown7;
	
	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};