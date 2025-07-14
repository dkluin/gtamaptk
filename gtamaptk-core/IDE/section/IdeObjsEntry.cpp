#include "IdeObjsEntry.h"
#include <cstring>

bool IdeObjsEntry::Read(const char* szLine) {
	int32_t nModelIndex;
	char szModelName[MAX_IMG_ENTRY_NAME];
	char szTxdName[MAX_IMG_ENTRY_NAME];
	float fDrawDistance[3];
	int32_t nObjectCount;
	uint32_t dwModelFlags;

	fDrawDistance[0] = fDrawDistance[1] = fDrawDistance[2] = 0.0;
	nObjectCount = 1;

	if (sscanf(szLine, "%d %s %s %f %d", &nModelIndex, szModelName, szTxdName, &fDrawDistance[0], &dwModelFlags) != 5 || fDrawDistance[0] < 4.0)
	{
		if (sscanf(szLine, "%d %s %s %d", &nModelIndex, szModelName, szTxdName, &nObjectCount) != 4) {
			SetLastErrorMsg("Error: Incorrect IDE:OBJS line: \"%s\"\n", szLine);
			return false;
		}

		switch (nObjectCount)
		{
		case 1:
			if (sscanf(szLine, "%d %s %s %d %f %d", &nModelIndex, szModelName, szTxdName, &nObjectCount, &fDrawDistance[0], &dwModelFlags) != 6) {
				SetLastErrorMsg("Error: IDE:OBJS line \"%s\" has an object count of %d, this line does not have the expected 6 columns\n", szLine, nObjectCount);
				return false;
			}

			m_eType = IDEOBJS_TYPE_1;
			break;
		case 2:
			if (sscanf(szLine, "%d %s %s %d %f %f %d", &nModelIndex, szModelName, szTxdName, &nObjectCount, &fDrawDistance[0], &fDrawDistance[1], &dwModelFlags) != 7) {
				SetLastErrorMsg("Error: IDE:OBJS line \"%s\" has an object count of %d, this line does not have the expected 7 columns\n", szLine, nObjectCount);
				return false;
			}

			m_eType = IDEOBJS_TYPE_2;
			break;
		case 3:
			if (sscanf(szLine, "%d %s %s %d %f %f %f %d", &nModelIndex, szModelName, szTxdName, &nObjectCount, &fDrawDistance[0], &fDrawDistance[1], &fDrawDistance[2], &dwModelFlags) != 8) {
				SetLastErrorMsg("Error: IDE:OBJS line \"%s\" has an object count of %d, this line does not have the expected 8 columns\n", szLine, nObjectCount);
				return false;
			}

			m_eType = IDEOBJS_TYPE_3;
			break;
		}
	} else {
		m_eType = IDEOBJS_TYPE_4;
	}

	m_nModelIndex = nModelIndex;
	strcpy(m_szModelName, szModelName);
	strcpy(m_szTxdName, szTxdName);
	m_nObjectCount = nObjectCount;
	m_fDrawDistance[0] = fDrawDistance[0];
	m_fDrawDistance[1] = fDrawDistance[1];
	m_fDrawDistance[2] = fDrawDistance[2];
	m_dwModelFlags = dwModelFlags;

	return true;
}

void IdeObjsEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IdeObjsEntry::Write(FILE* pFile) {
	switch (m_eType) {
	case IDEOBJS_TYPE_1:
		fprintf(pFile, "%d, %s, %s, %d, %.8g, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_nObjectCount, m_fDrawDistance[0], m_dwModelFlags);
		break;

	case IDEOBJS_TYPE_2:
		fprintf(pFile, "%d, %s, %s, %d, %.8g, %.8g, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_nObjectCount, m_fDrawDistance[0], m_fDrawDistance[1], m_dwModelFlags);
		break;

	case IDEOBJS_TYPE_3:
		fprintf(pFile, "%d, %s, %s, %d, %.8g, %.8g, %.8g, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_nObjectCount, m_fDrawDistance[0], m_fDrawDistance[1], m_fDrawDistance[2], m_dwModelFlags);
		break;

	case IDEOBJS_TYPE_4:
		fprintf(pFile, "%d, %s, %s, %.8g, %d\n", m_nModelIndex, m_szModelName, m_szTxdName, m_fDrawDistance[0], m_dwModelFlags);
		break;
	}
}

void IdeObjsEntry::WriteBinary(FILE* pFile) {
	// not implemented
}