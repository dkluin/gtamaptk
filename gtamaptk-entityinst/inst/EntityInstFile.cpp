#include "EntityInstFile.h"

// entity.inst file format:
// 0x0: Number of entities (uint32_t)
// 0x4: Array of entity struct sizes (uint32_t[number of entities])
// 0xX: Entity structs (variable size, depends)

// Entity struct format:
// 0x0: Entity type (PaddedString, size is aligned to 4 bytes)
// 0xX: Internal entity name (PaddedString, size is aligned to 4 bytes)
// 0xX: Position (Vector3d, 12 bytes)
// 0xX: Rotation (Quaternion, 16 bytes)
// 0xX: Optional: Entity class name (PaddedString, size is aligned to 4 bytes)
// 0xX: Optional: Entity class data
void EntityInstFile::ClearEntries() {
	m_vEntityInstances.clear();
}

bool EntityInstFile::Read(const char* szFileName, bool bClearExisting) {
	FILE* pFile = fopen(szFileName, "r");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to open %s", szFileName);
		return false;
	}

	if (bClearExisting) {
		ClearEntries();
	}

	for (char* szLine = ReadLine(pFile); szLine != nullptr; szLine = ReadLine(pFile)) {
		EntityInstance mInstance;
		if (*szLine == '\0' || *szLine == '#') continue;
		if (!mInstance.Read(szLine)) continue; // errors are set in Read

		m_vEntityInstances.push_back(mInstance);
	}

	return true;
}

bool EntityInstFile::ReadBinary(const char* szFileName, bool bClearExisting) {
	int32_t* apEntityInstDataSizes;
	int32_t nNumInstances;

	FILE* pFile = fopen(szFileName, "rb");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to open %s", szFileName);
		return false;
	}

	if (bClearExisting) {
		ClearEntries();
	}

	fread(&nNumInstances, sizeof(nNumInstances), 1, pFile);

	apEntityInstDataSizes = new int32_t[nNumInstances]();
	fread(apEntityInstDataSizes, sizeof(int32_t) * nNumInstances, 1, pFile);

	for (int32_t i = 0; i < nNumInstances; i++) {
		int32_t nEndOfEntityStructPos;
		EntityInstance mInstance;

		nEndOfEntityStructPos = ftell(pFile) + apEntityInstDataSizes[i];// Next struct start position

		if (!mInstance.ReadBinary(pFile, nEndOfEntityStructPos)) {
			SetLastErrorMsg("Error: INST struct at index %d could not be read", i);
			continue;
		}

		m_vEntityInstances.push_back(mInstance);
	}

	delete[] apEntityInstDataSizes;
	fclose(pFile);
	return true;
}

bool EntityInstFile::Write(const char* szFileName) {
	FILE* pFile = fopen(szFileName, "w");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Failed to open %s", szFileName);
		return false;
	}

	for (auto it : m_vEntityInstances) {
		it.Write(pFile);
	}

	fclose(pFile);
	return true;
}

bool EntityInstFile::WriteBinary(const char* szFileName) {
	int32_t* apEntityInstDataSizes;
	int32_t nNumEntityInstances;
	FILE* pFile;
	
	if (m_vEntityInstances.size() == 0) {
		SetLastErrorMsg("Error: Cannot write empty entity.inst file");
		return false;
	}

	pFile = fopen(szFileName, "wb");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Failed to open %s", szFileName);
		return false;
	}

	nNumEntityInstances = m_vEntityInstances.size();
	fwrite(&nNumEntityInstances, sizeof(int32_t), 1, pFile);

	apEntityInstDataSizes = new int32_t[nNumEntityInstances];
	memset(apEntityInstDataSizes, NULL, sizeof(int32_t) * nNumEntityInstances);

	int32_t nExpectedInstStartPos = sizeof(int32_t) * nNumEntityInstances + sizeof(int32_t);
	for (int32_t i = 0; i < nNumEntityInstances; i++) {
		int32_t nSize = m_vEntityInstances.at(i).GetBinarySize(nExpectedInstStartPos);

		apEntityInstDataSizes[i] = nSize;
		nExpectedInstStartPos += nSize;
	}

	fwrite(apEntityInstDataSizes, sizeof(int32_t) * nNumEntityInstances, 1, pFile);

	for (auto it : m_vEntityInstances) {
		it.WriteBinary(pFile);
	}

	fclose(pFile);
	return true;
}