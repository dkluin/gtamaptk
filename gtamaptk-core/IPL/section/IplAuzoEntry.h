#pragma once

#include "../../main/Shared.hpp"
#include "../../main/MapFileSection.h"
#include "../../math/Box.h"
#include "../../math/Sphere.h"

#define MAX_AUZO_NAME 8

enum eAuzoEntryType {
	IPLAUZO_BOX,
	IPLAUZO_SPHERE
};

class IplAuzoEntry : public IMapFileSectionEntry {
public:
	eAuzoEntryType m_eType;
	char m_szName[MAX_AUZO_NAME];
	int32_t m_nTrackID;
	bool m_bOnByDefault;

	union {
		Box m_mBox;
		Sphere m_mSphere;
	};

	int8_t GetType() { return IPL_SECTION_AUZO; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};