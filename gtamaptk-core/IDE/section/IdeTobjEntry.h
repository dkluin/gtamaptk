#pragma once

#include "../../main/Shared.hpp"
#include "../../main/MapFileSection.h"

enum eTobjEntryType {
	IDETOBJ_TYPE_1, // Id, ModelName, TxdName, MeshCount, DrawDistance, Flags, TimeOn, TimeOff
	IDETOBJ_TYPE_2, // Id, ModelName, TxdName, MeshCount, DrawDistance1, DrawDistance2, Flags, TimeOn, TimeOff
	IDETOBJ_TYPE_3, // Id, ModelName, TxdName, MeshCount, DrawDistance1, DrawDistance2, DrawDistance3, Flags, TimeOn, TimeOff
	IDETOBJ_TYPE_4 // Id, ModelName, TxdName, DrawDistance, Flags, TimeOn, TimeOff
};

class IdeTobjEntry : public IMapFileSectionEntry {
public:
	eTobjEntryType m_eType;
	int32_t m_nModelIndex;
	char m_szModelName[MAX_IMG_ENTRY_NAME];
	char m_szTxdName[MAX_IMG_ENTRY_NAME];
	int8_t m_nObjectCount;
	float m_fDrawDistance[3];
	uint32_t m_dwModelFlags;
	int8_t m_nTimeOn;
	int8_t m_nTimeOff;

	int8_t GetType() { return IDE_SECTION_TOBJ; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};