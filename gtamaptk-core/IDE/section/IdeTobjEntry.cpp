#include "IdeTobjEntry.h"
#include <cstring>

bool IdeTobjEntry::Read(const char* szLine) {
	int32_t nModelIndex;
	char szModelName[MAX_IMG_ENTRY_NAME];
	char szTxdName[MAX_IMG_ENTRY_NAME];
	float fDrawDistance[3];
	int32_t nObjectCount;
	uint32_t dwModelFlags;
	int32_t nTimeOn, nTimeOff;

	fDrawDistance[0] = fDrawDistance[1] = fDrawDistance[2] = 0.0;
	nObjectCount = 1;
	nTimeOn = 0;
	nTimeOff = 24;

	if (sscanf(szLine, "%d %s %s %f %d %d %d", &nModelIndex, szModelName, szTxdName, &fDrawDistance[0], &dwModelFlags, &nTimeOn, &nTimeOff) != 7 || fDrawDistance[0] < 4.0)
	{
		if (sscanf(szLine, "%d %s %s %d %d %d", &nModelIndex, szModelName, szTxdName, &nObjectCount, &nTimeOn, &nTimeOff) != 6) {
			SetLastErrorMsg("Error: Incorrect IDE:TOBJ line: \"%s\"\n", szLine);
			return false;
		}

		switch (nObjectCount)
		{
		case 1:
			if (sscanf(szLine, "%d %s %s %d %f %d %d %d", &nModelIndex, szModelName, szTxdName, &nObjectCount, &fDrawDistance[0], &dwModelFlags, &nTimeOn, &nTimeOff) != 8) {
				SetLastErrorMsg("Error: IDE:TOBJ line \"%s\" has an object count of %d, this line does not have the expected 8 columns\n", szLine, nObjectCount);
				return false;
			}

			m_eType = IDETOBJ_TYPE_1;
			break;
		case 2:
			if (sscanf(szLine, "%d %s %s %d %f %f %d %d %d", &nModelIndex, szModelName, szTxdName, &nObjectCount, &fDrawDistance[0], &fDrawDistance[1], &dwModelFlags, &nTimeOn, &nTimeOff) != 9) {
				SetLastErrorMsg("Error: IDE:TOBJ line \"%s\" has an object count of %d, this line does not have the expected 9 columns\n", szLine,nObjectCount);
				return false;
			}

			m_eType = IDETOBJ_TYPE_2;
			break;
		case 3:
			if (sscanf(szLine, "%d %s %s %d %f %f %f %d %d %d", &nModelIndex, szModelName, szTxdName, &nObjectCount, &fDrawDistance[0], &fDrawDistance[1], &fDrawDistance[2], &dwModelFlags, &nTimeOn, &nTimeOff) != 10) {
				SetLastErrorMsg("Error: IDE:TOBJ line \"%s\" has an object count of %d, this line does not have the expected 10 columns\n", szLine, nObjectCount);
				return false;
			}

			m_eType = IDETOBJ_TYPE_3;
			break;
		}
	} else {
		m_eType = IDETOBJ_TYPE_4;
	}

	m_nModelIndex = nModelIndex;
	strcpy(m_szModelName, szModelName);
	strcpy(m_szTxdName, szTxdName);
	m_nObjectCount = nObjectCount;
	m_fDrawDistance[0] = fDrawDistance[0];
	m_fDrawDistance[1] = fDrawDistance[1];
	m_fDrawDistance[2] = fDrawDistance[2];
	m_dwModelFlags = dwModelFlags;
	m_nTimeOn = nTimeOn;
	m_nTimeOff = nTimeOff;

	return true;
}

void IdeTobjEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IdeTobjEntry::Write(FILE* pFile) {
	switch (m_eType) {
	case IDETOBJ_TYPE_1:
		fprintf(pFile, "%d, %s, %s, %d, %.8g, %d, %d, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_nObjectCount, m_fDrawDistance[0], m_dwModelFlags, m_nTimeOn, m_nTimeOff);
		break;

	case IDETOBJ_TYPE_2:
		fprintf(pFile, "%d, %s, %s, %d, %.8g, %.8g, %d, %d, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_nObjectCount, m_fDrawDistance[0], m_fDrawDistance[1], m_dwModelFlags, m_nTimeOn, m_nTimeOff);
		break;

	case IDETOBJ_TYPE_3:
		fprintf(pFile, "%d, %s, %s, %d, %.8g, %.8g, %.8g, %d, %d, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_nObjectCount, m_fDrawDistance[0], m_fDrawDistance[1], m_fDrawDistance[2], m_dwModelFlags, m_nTimeOn, m_nTimeOff);
		break;

	case IDETOBJ_TYPE_4:
		fprintf(pFile, "%d, %s, %s, %.8g, %d, %d, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_fDrawDistance[0], m_dwModelFlags, m_nTimeOn, m_nTimeOff);
		break;
	}
}

void IdeTobjEntry::WriteBinary(FILE* pFile) {
	// not implemented
}