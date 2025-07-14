#pragma once

#include "../../main/MapFileSection.h"
#include "../../math/Vector3d.h"

enum eCullEntryType {
	IPLCULL_ATTR,
	IPLCULL_MIRROR
};

// https://github.com/aap/librwgta/blob/master/tools/euryopa/zones.cpp#L96
enum eCullZoneAttrs {
	ATTRZONE_CAMCLOSEIN = 1,
	ATTRZONE_STAIRS = 2,
	ATTRZONE_1STPERSON = 4,
	ATTRZONE_NORAIN = 8,
	ATTRZONE_NOPOLICE = 0x10,
	ATTRZONE_NOTCULLZONE = 0x20,
	ATTRZONE_DOINEEDCOLLISION = 0x40,
	ATTRZONE_SUBWAYVISIBLE = 0x80,
	ATTRZONE_POLICEABANDONCARS = 0x100,
	ATTRZONE_INROOMFORAUDIO = 0x200,
	ATTRZONE_WATERFUDGE = 0x400,
	ATTRZONE_FEWERPEDS = 0x400,
	ATTRZONE_TUNNEL = 0x800,
	ATTRZONE_MILITARYZONE = 0x1000,
	ATTRZONE_UNKNOWN = 0x2000,
	ATTRZONE_EXTRAAIRRESISTANCE = 0x4000,
	ATTRZONE_FEWERCARS = 0x8000
};

class IplCullEntry : public IMapFileSectionEntry {
public:
	eCullEntryType m_eType;
	Vector3d m_vCenter;
	float m_fS1x, m_fS1y;
	float m_fZmin;
	float m_fS2x, m_fS2y;
	float m_fZmax;
	int32_t m_nFlags;

	union {
		int8_t m_nWantedLevelDrop;

		struct {
			Vector3d m_vNormal;
			float m_fDistance;
		} m_mMirror;
	};

	int8_t GetType() { return IPL_SECTION_CULL; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};