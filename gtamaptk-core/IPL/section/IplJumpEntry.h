#pragma once

#include "../../main/MapFileSection.h"
#include "../../math/Vector3d.h"
#include "../../math/Box.h"

class IplJumpEntry : public IMapFileSectionEntry {
public:
	Box m_mStartArea;
	Box m_mTargetArea;
	Vector3d m_vCameraPos;
	int32_t m_nReward;

	int8_t GetType() { return IPL_SECTION_JUMP; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};