#include "CullZoneFile.h"
#include <vcruntime_string.h>

CullZoneFile::CullZoneFile(int32_t nNumCullZonesInBlock, int32_t nNumAttrZonesInBlock, int32_t nNumZoneIndices, int32_t nNumBuildings, int32_t nNumTreadables) {
	m_nNumCullZones = 0;
	m_nNumAttrZones = 0;

	m_nNumCullZonesInBlock = nNumCullZonesInBlock;
	m_nNumAttrZonesInBlock = nNumAttrZonesInBlock;
	m_nNumZoneIndices = nNumZoneIndices;
	m_nNumBuildings = nNumBuildings;
	m_nNumTreadables = nNumTreadables;

	m_pCullZones = new CullZone[nNumCullZonesInBlock];
	m_pAttributeZones = new AttributeZone[nNumAttrZonesInBlock];
	m_pIndices = new uint16_t[nNumZoneIndices];
	m_pPointersToBigBuildingsForBuildings = new int16_t[nNumBuildings];
	m_pPointersToBigBuildingsForTreadables = new int16_t[nNumTreadables];

	memset(m_pCullZones, NULL, sizeof(CullZone) * nNumCullZonesInBlock);
	memset(m_pAttributeZones, NULL, sizeof(AttributeZone) * nNumAttrZonesInBlock);
	memset(m_pIndices, NULL, sizeof(uint16_t) * nNumZoneIndices);
	memset(m_pPointersToBigBuildingsForBuildings, NULL, sizeof(int16_t) * nNumBuildings);
	memset(m_pPointersToBigBuildingsForTreadables, NULL, sizeof(int16_t) * nNumTreadables);
}

CullZoneFile::~CullZoneFile() {
	delete[] m_pCullZones;
	delete[] m_pAttributeZones;
	delete[] m_pIndices;
	delete[] m_pPointersToBigBuildingsForBuildings;
	delete[] m_pPointersToBigBuildingsForTreadables;
}

void CullZoneFile::ClearEntries() {
	m_nNumCullZones = 0;
	m_nNumAttrZones = 0;
	
	memset(m_pCullZones, NULL, sizeof(CullZone) * m_nNumCullZonesInBlock);
	memset(m_pAttributeZones, NULL, sizeof(AttributeZone) * m_nNumAttrZonesInBlock);
	memset(m_pIndices, NULL, sizeof(uint16_t) * m_nNumZoneIndices);
	memset(m_pPointersToBigBuildingsForBuildings, NULL, sizeof(int16_t) * m_nNumBuildings);
	memset(m_pPointersToBigBuildingsForTreadables, NULL, sizeof(int16_t) * m_nNumTreadables);
}

bool CullZoneFile::Read(const char* szFileName, bool bClearExisting) {
	int32_t nNumZoneIndicesRead, nNumPointersForBuildingsRead, nNumPointersForTreadablesRead;

	FILE* pFile = fopen(szFileName, "r");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Failed to open %s", szFileName);
		return false;
	}

	if (bClearExisting) {
		ClearEntries();
	}

	nNumZoneIndicesRead = 0;
	nNumPointersForBuildingsRead = 0;
	nNumPointersForTreadablesRead = 0;

	int8_t nSection = CULLZONE_SECTION_NONE;
	for (char* szLine = ReadLine(pFile); szLine != nullptr; szLine = ReadLine(pFile)) {
		if (*szLine == '\0' || *szLine == '#') continue;

		if (nSection == CULLZONE_SECTION_NONE) {
			if (SECTION4(szLine, 'c', 'u', 'l', 'l')) nSection = CULLZONE_SECTION_CULL;
			else if (SECTION4(szLine, 'a', 't', 't', 'r'))
				nSection = CULLZONE_SECTION_ATTR;
			else if (SECTION4(szLine, 'z', 'i', 'n', 'd'))
				nSection = CULLZONE_SECTION_ZIND;
			else if (SECTION4(szLine, 'b', 'b', 'p', 'b'))
				nSection = CULLZONE_SECTION_BBPB;
			else if (SECTION4(szLine, 'b', 'b', 'p', 't'))
				nSection = CULLZONE_SECTION_BBPT;
		} else if (SECTION3(szLine, 'e', 'n', 'd')) {
			nSection = CULLZONE_SECTION_NONE;
		} else {
			switch (nSection) {
				case CULLZONE_SECTION_CULL:
				{
					if (m_nNumCullZones > m_nNumCullZonesInBlock) {
						SetLastErrorMsg("Error: Number of cull zones (%d) exceeds the total number of zones stored in CULLZONE.DAT (%d)\n", m_nNumCullZones, m_nNumCullZonesInBlock);

						fclose(pFile);
						return false;
					}

					CullZone mEntry;
					if (sscanf(szLine, "%f %f %f %f %f %f %f %f %f %d %hd %hd %hd %hd %hd %hd",
						&mEntry.m_vCenter.x, &mEntry.m_vCenter.y, &mEntry.m_vCenter.z,
						&mEntry.m_mBox.m_vMin.x, &mEntry.m_mBox.m_vMin.y, &mEntry.m_mBox.m_vMin.z,
						&mEntry.m_mBox.m_vMax.x, &mEntry.m_mBox.m_vMax.y, &mEntry.m_mBox.m_vMax.z,
						&mEntry.m_nIndexStart,
						&mEntry.m_nGroupIndexCount[0], &mEntry.m_nGroupIndexCount[1], &mEntry.m_nGroupIndexCount[2],
						&mEntry.m_nNumBuildings,
						&mEntry.m_nNumTreadablesPlus10m,
						&mEntry.m_nNumTreadables) == 16) {
						memcpy(&m_pCullZones[m_nNumCullZones], &mEntry, sizeof(CullZone));
						m_nNumCullZones++;
						break;
					}

					SetLastErrorMsg("Error: CULLZONE:CULL line \"%s\" is invalid\n", szLine);
				}
				break;

				case CULLZONE_SECTION_ATTR:
				{
					if (m_nNumAttrZones > m_nNumAttrZonesInBlock) {
						SetLastErrorMsg("Error: Number of attribute zones (%d) exceeds the total number of zones stored in CULLZONE.DAT (%d)\n", m_nNumAttrZones, m_nNumAttrZonesInBlock);

						fclose(pFile);
						return false;
					}

					AttributeZone mEntry;
                    if (sscanf(szLine, "%f %f %f %f %f %f %hd %hd",
                        &mEntry.m_mBox.m_vMin.x, &mEntry.m_mBox.m_vMin.y, &mEntry.m_mBox.m_vMin.z,
                        &mEntry.m_mBox.m_vMax.x, &mEntry.m_mBox.m_vMax.y, &mEntry.m_mBox.m_vMax.z,
                        &mEntry.m_nAttributes, &mEntry.m_nWantedLevelDrop) == 8) {
                        memcpy(&m_pAttributeZones[m_nNumAttrZones], &mEntry, sizeof(AttributeZone));
                        m_nNumAttrZones++;
						break;
                    }

					SetLastErrorMsg("Error: CULLZONE:ATTR line \"%s\" is invalid\n", szLine);
				}
				break;

				case CULLZONE_SECTION_ZIND:
				{
					if (nNumZoneIndicesRead > m_nNumZoneIndices) {
						SetLastErrorMsg("Error: Number of zone indices (%d) exceeds the total number of indices stored in CULLZONE.DAT (%d)\n", nNumZoneIndicesRead, m_nNumZoneIndices);

						fclose(pFile);
						return false;
					}

					int32_t nZoneIndex;
					if (sscanf(szLine, "%d", &nZoneIndex) == 1) {
						m_pIndices[nNumZoneIndicesRead] = nZoneIndex;
						nNumZoneIndicesRead++;
						break;
					}

					SetLastErrorMsg("Error: CULLZONE:ZIND line \"%s\" is invalid\n", szLine);
				}
				break;

				case CULLZONE_SECTION_BBPB:
				{
					if (nNumPointersForBuildingsRead > m_nNumBuildings) {
						SetLastErrorMsg("Error: Number of big building pointers for buildings (%d) exceeds the total number of ptrs stored in CULLZONE.DAT (%d)\n", nNumPointersForBuildingsRead, m_nNumBuildings);

						fclose(pFile);
						return false;
					}

					int32_t nPointerToBigBuilding;
					if (sscanf(szLine, "%d", &nPointerToBigBuilding) == 1) {
						m_pPointersToBigBuildingsForBuildings[nNumPointersForBuildingsRead] = nPointerToBigBuilding;
						nNumPointersForBuildingsRead++;
						break;
					}

					SetLastErrorMsg("Error: CULLZONE:BBPB line \"%s\" is invalid\n", szLine);
				}
				break;

				case CULLZONE_SECTION_BBPT:
				{
					if (nNumPointersForTreadablesRead > m_nNumTreadables) {
						SetLastErrorMsg("Error: Number of big building pointers for treadables (%d) exceeds the total number of ptrs stored in CULLZONE.DAT (%d)\n", nNumPointersForTreadablesRead, m_nNumTreadables);

						fclose(pFile);
						return false;
					}

					int32_t nPointerToBigBuilding;
					if (sscanf(szLine, "%d", &nPointerToBigBuilding) == 1) {
						m_pPointersToBigBuildingsForTreadables[nNumPointersForTreadablesRead] = nPointerToBigBuilding;
						nNumPointersForTreadablesRead++;
						break;
					}

					SetLastErrorMsg("Error: CULLZONE:BBPT line \"%s\" is invalid\n", szLine);
				}
				break;
			}
		}
	}

	if (nNumZoneIndicesRead != m_nNumZoneIndices)
		SetLastErrorMsg("Warning: Number of zone indices read (%d) does not match the expected number (%d)\n", nNumZoneIndicesRead, m_nNumZoneIndices);
	if (nNumPointersForBuildingsRead != m_nNumBuildings)
		SetLastErrorMsg("Warning: Number of big building pointers for buildings read (%d) does not match the expected number (%d)\n", nNumPointersForBuildingsRead, m_nNumBuildings);
	if (nNumPointersForTreadablesRead != m_nNumTreadables)
		SetLastErrorMsg("Warning: Number of big building pointers for treadables read (%d) does not match the expected number (%d)\n", nNumPointersForTreadablesRead, m_nNumTreadables);

	fclose(pFile);
	return true;
}

bool CullZoneFile::ReadBinary(const char* szFileName, bool bClearExisting) {
	FILE* pFile = fopen(szFileName, "rb");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Failed to open %s", szFileName);
		return false;
	}

	if (bClearExisting) {
		ClearEntries();
	}

	fread(&m_nNumCullZones, sizeof(m_nNumCullZones), 1, pFile);
	fread(m_pCullZones, sizeof(CullZone) * m_nNumCullZonesInBlock, 1, pFile);
	fread(&m_nNumAttrZones, sizeof(m_nNumAttrZones), 1, pFile);
	fread(m_pAttributeZones, sizeof(AttributeZone) * m_nNumAttrZonesInBlock, 1, pFile);
	fread(m_pIndices, sizeof(uint16_t) * m_nNumZoneIndices, 1, pFile);
	fread(m_pPointersToBigBuildingsForBuildings, sizeof(int16_t) * m_nNumBuildings, 1, pFile);
	fread(m_pPointersToBigBuildingsForTreadables, sizeof(int16_t) * m_nNumTreadables, 1, pFile);

	fclose(pFile);
	return true;
}

bool CullZoneFile::Write(const char* szFileName) {
	FILE* pFile = fopen(szFileName, "w");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Failed to open %s", szFileName);
		return false;
	}

	fprintf(pFile, "cull\n");
	for (int32_t i = 0; i < m_nNumCullZones; i++) {
		fprintf(pFile, "%g, %g, %g, %g, %g, %g, %g, %g, %g, %d, %d, %d, %d, %d, %d, %d\n",
			m_pCullZones[i].m_vCenter.x, m_pCullZones[i].m_vCenter.y, m_pCullZones[i].m_vCenter.z,
			m_pCullZones[i].m_mBox.m_vMin.x, m_pCullZones[i].m_mBox.m_vMin.y, m_pCullZones[i].m_mBox.m_vMin.z,
			m_pCullZones[i].m_mBox.m_vMax.x, m_pCullZones[i].m_mBox.m_vMax.y, m_pCullZones[i].m_mBox.m_vMax.z,
			m_pCullZones[i].m_nIndexStart,
			m_pCullZones[i].m_nGroupIndexCount[0], m_pCullZones[i].m_nGroupIndexCount[1], m_pCullZones[i].m_nGroupIndexCount[2],
			m_pCullZones[i].m_nNumBuildings,
			m_pCullZones[i].m_nNumTreadablesPlus10m,
			m_pCullZones[i].m_nNumTreadables);
	}
	fprintf(pFile, "end\n");

	fprintf(pFile, "attr\n");
	for (int32_t i = 0; i < m_nNumAttrZones; i++) {
		fprintf(pFile, "%g, %g, %g, %g, %g, %g, %d, %d\n",
			m_pAttributeZones[i].m_mBox.m_vMin.x, m_pAttributeZones[i].m_mBox.m_vMin.y, m_pAttributeZones[i].m_mBox.m_vMin.z,
			m_pAttributeZones[i].m_mBox.m_vMax.x, m_pAttributeZones[i].m_mBox.m_vMax.y, m_pAttributeZones[i].m_mBox.m_vMax.z,
			m_pAttributeZones[i].m_nAttributes,
			m_pAttributeZones[i].m_nWantedLevelDrop);
	}
	fprintf(pFile, "end\n");

	fprintf(pFile, "zind\n");// Zone Indices
	for (int32_t i = 0; i < m_nNumZoneIndices; i++) {
		fprintf(pFile, "%d\n", m_pIndices[i]);
	}
	fprintf(pFile, "end\n");

	fprintf(pFile, "bbpb\n");// Big Building pointer - for buildings
	for (int32_t i = 0; i < m_nNumBuildings; i++) {
		fprintf(pFile, "%d\n", m_pPointersToBigBuildingsForBuildings[i]);
	}
	fprintf(pFile, "end\n");

	fprintf(pFile, "bbpt\n");// Big Building pointer - for treadables
	for (int32_t i = 0; i < m_nNumTreadables; i++) {
		fprintf(pFile, "%d\n", m_pPointersToBigBuildingsForTreadables[i]);
	}
	fprintf(pFile, "end");

	fclose(pFile);
	return true;
}

bool CullZoneFile::WriteBinary(const char* szFileName) {
	FILE* pFile = fopen(szFileName, "wb");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Failed to open %s", szFileName);
		return false;
	}

	fwrite(&m_nNumCullZones, sizeof(m_nNumCullZones), 1, pFile);
	fwrite(m_pCullZones, sizeof(CullZone) * m_nNumCullZonesInBlock, 1, pFile);
	fwrite(&m_nNumAttrZones, sizeof(m_nNumAttrZones), 1, pFile);
	fwrite(m_pAttributeZones, sizeof(AttributeZone) * m_nNumAttrZonesInBlock, 1, pFile);
	fwrite(m_pIndices, sizeof(uint16_t) * m_nNumZoneIndices, 1, pFile);
	fwrite(m_pPointersToBigBuildingsForBuildings, sizeof(int16_t) * m_nNumBuildings, 1, pFile);
	fwrite(m_pPointersToBigBuildingsForTreadables, sizeof(int16_t) * m_nNumTreadables, 1, pFile);

	fclose(pFile);
	return true;
}