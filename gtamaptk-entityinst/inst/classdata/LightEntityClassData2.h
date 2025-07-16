#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryLightEntityClassData2 {
public:
	int32_t m_nType;
	float m_fConeAngle;
	float m_fRadius;
	float m_fColour[3];
	int32_t unknown1; // appears to be flags of some kind
	int32_t unknown2; // appears to be flags of some kind
	int32_t unknown3; // appears to be flags of some kind
	int32_t unknown4; // appears to be flags of some kind
	int32_t unknown5; // appears to be flags of some kind
	int32_t unknown6; // appears to be flags of some kind
	int32_t unknown7; // appears to be flags of some kind
	float m_fSize;
	int32_t m_nIntensity;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryLightEntityClassData2, 60);

class LightEntityClassData2 {
public:
	int32_t m_nType;
	float m_fConeAngle;
	float m_fRadius;
	float m_fColour[3];
	int32_t unknown1;
	int32_t unknown2;
	int32_t unknown3;
	int32_t unknown4;
	int32_t unknown5;
	int32_t unknown6;
	int32_t unknown7;
	float m_fSize;
	int32_t m_nIntensity;

	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};