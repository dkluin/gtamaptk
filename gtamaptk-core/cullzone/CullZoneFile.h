#pragma once

#include "../main/Shared.hpp"
#include "../math/Box.h"
#include <stdint.h>

// Default values, for reference, not used in the code
#define NUM_CULL_ZONES 512
#define NUM_ATTRIB_ZONES 288
#define NUM_ZONE_INDICES 55000
#define NUM_BUILDINGS 5500 // same as building pool size, obviously
#define NUM_TREADABLES 1214 // same as treadable pool size, obviously

#pragma pack(push, 1)
class CullZone {
public:
	Vector3d m_vCenter;
	Box m_mBox;
	int32_t m_nIndexStart;
	int16_t m_nGroupIndexCount[3];
	int16_t m_nNumBuildings;
	int16_t m_nNumTreadablesPlus10m;
	int16_t m_nNumTreadables;
};
#pragma pack(pop)

VALIDATE_SIZE(CullZone, 52);

#pragma pack(push, 1)
class AttributeZone {
public:
	Box	m_mBox;
	int16_t m_nAttributes;// The IDs used here can be found in eCullZoneAttrs
	int16_t m_nWantedLevelDrop;
};
#pragma pack(pop)

VALIDATE_SIZE(AttributeZone, 28);

enum eCullZoneSection {
	CULLZONE_SECTION_NONE = 0,
	CULLZONE_SECTION_CULL,
	CULLZONE_SECTION_ATTR,
	CULLZONE_SECTION_ZIND,
	CULLZONE_SECTION_BBPB,
	CULLZONE_SECTION_BBPT
};

class CullZoneFile {
public:
	int32_t	m_nNumCullZonesInBlock; // by default, there are 512 zones in the cull section, but you can pass a different value to the constructor
	int32_t m_nNumAttrZonesInBlock; // by default, there are 288 zones in the attr section, but you can pass a different value to the constructor
	int32_t m_nNumZoneIndices;
	int32_t m_nNumBuildings;
	int32_t m_nNumTreadables;

	int32_t m_nNumCullZones; // ACTUAL number of cull zones
	CullZone* m_pCullZones;
	int32_t m_nNumAttrZones; // ACTUAL number of attribute zones
	AttributeZone* m_pAttributeZones;
	uint16_t* m_pIndices;
	int16_t* m_pPointersToBigBuildingsForBuildings;
	int16_t* m_pPointersToBigBuildingsForTreadables;

	CullZoneFile(int32_t nNumCullZonesInBlock, int32_t nNumAttrZonesInBlock, int32_t nNumZoneIndices, int32_t nNumBuildings, int32_t nNumTreadables);
	~CullZoneFile();

	void ClearEntries();

	bool Read(const char* szFileName, bool bClearExisting = true);
	bool ReadBinary(const char* szFileName, bool bClearExisting = true);
	bool Write(const char* szFileName);
	bool WriteBinary(const char* szFileName);
};