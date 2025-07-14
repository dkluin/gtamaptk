#pragma once

#include "../../main/MapFileSection.h"
#include "../../main/Shared.hpp"
#include "../../math/Vector3d.h"
#include "../../math/Quaternion.h"

enum eIplInstFlags {
	IPLINST_UNIMPORTANT = 256,
	IPLINST_FORCE_MATRIX_ROT = 512, // behavior is modified in GTA:UG
	IPLINST_UNDERWATER = 1024,
	IPLINST_UNDERGROUND = 2048,
	IPLINST_UNDERGROUND_EXIT = 4096
};

#pragma pack(push, 1)
class BinaryIplInstData {
public:
	Vector3d m_vPosition;
	Quaternion m_qRotation;
	int32_t m_nModelIndex;

	union {
		struct {
			uint8_t m_nAreaID;
			int16_t m_nFlags;
		};

		uint32_t m_dwAreaAndFlags;
	};

	int32_t m_nLodIndex;
};
#pragma pack(pop)

VALIDATE_SIZE(BinaryIplInstData, 0x28);

class IplInstEntry : public IMapFileSectionEntry {
public:
	int32_t m_nModelIndex;
	char m_szModelName[MAX_IMG_ENTRY_NAME];

	union {
		struct {
			uint8_t m_nAreaID;
			int16_t m_nFlags;
		};

		uint32_t m_dwAreaAndFlags;
	};

	Vector3d m_vPosition;
	Quaternion m_qRotation;
	int32_t m_nLodIndex;

	int8_t GetType() { return IPL_SECTION_INST; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};