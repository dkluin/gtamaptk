#include "IdeAnimEntry.h"
#include <cstring>

bool IdeAnimEntry::Read(const char* szLine) {
	int32_t nModelIndex;
	char szModelName[MAX_IMG_ENTRY_NAME];
	char szTxdName[MAX_IMG_ENTRY_NAME];
	char szIfpName[MAX_IMG_ENTRY_NAME];
	float fDrawDistance;
	uint32_t dwModelFlags;

	if (sscanf(szLine, "%d %s %s %s %f %d", &nModelIndex, szModelName, szTxdName, szIfpName, &fDrawDistance, &dwModelFlags) == 6) {
		m_nModelIndex = nModelIndex;
		strcpy(m_szModelName, szModelName);
		strcpy(m_szTxdName, szTxdName);
		strcpy(m_szIfpName, szIfpName);
		m_fDrawDistance = fDrawDistance;
		m_dwModelFlags = dwModelFlags;

		return true;
	}

	SetLastErrorMsg("Error: Incorrect IDE:ANIM line: \"%s\"\n", szLine);
	return false;
}

void IdeAnimEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IdeAnimEntry::Write(FILE* pFile) {
	fprintf(pFile, "%d, %s, %s, %s, %.8g, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_szIfpName, m_fDrawDistance, m_dwModelFlags);
}

void IdeAnimEntry::WriteBinary(FILE* pFile) {
	// not implemented
}