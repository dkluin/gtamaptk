#pragma once

#include "../../main/MapFileSection.h"
#include "../../math/Box.h"

class IplTcycEntry : public IMapFileSectionEntry {
public:
	Box m_mBox;
	int32_t m_nFarClipOverride;
	int8_t m_nExtraCol;
	float m_fExtraColIntensity;
	float m_fFallOffDist;
	float m_fLodDistMult_OLD; // the game will use this as the lod distance multiplier if there are less than 12 columns in the ipl line
	float m_fLodDistMult;

	int8_t GetType() { return IPL_SECTION_TCYC; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};