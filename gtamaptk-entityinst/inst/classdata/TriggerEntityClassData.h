#pragma once

#include <gtamaptk-core.h>

#pragma pack(push, 1)
class BinaryTriggerEntityClassData {
public:
	int32_t m_nType;
	float m_fSize;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryTriggerEntityClassData, 0x8);

class TriggerEntityClassData {
public:
	int32_t m_nType;
	float m_fSize;
		
	bool Read(const char* szLine);
	void ReadBinary(FILE* pFile);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};