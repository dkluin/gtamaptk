#pragma once

#include "../../main/MapFileSection.h"
#include "../../math./Vector2d.h"
#include "../../math/Misc.h"

class IplOcclEntry : public IMapFileSectionEntry {
public:
	Vector2d m_vCenter;
	float m_fBottom;
	float m_fWidth, m_fLength, m_fHeight;
	EulerAngle m_fRotX, m_fRotY, m_fRotZ;
	uint16_t m_nListIndex;

	int8_t GetType() { return IPL_SECTION_OCCL; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};