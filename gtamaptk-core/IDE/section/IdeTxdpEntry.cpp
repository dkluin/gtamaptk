#include "IdeTxdpEntry.h"
#include <cstring>

bool IdeTxdpEntry::Read(const char* szLine) {
	char szTxdName[MAX_IMG_ENTRY_NAME];
	char szParentTxdName[MAX_IMG_ENTRY_NAME];

	if (sscanf(szLine, "%s %s", szTxdName, szParentTxdName) == 2) {
		strcpy(m_szTxdName, szTxdName);
		strcpy(m_szParentTxdName, szParentTxdName);

		return true;
	}

	SetLastErrorMsg("Error: IDE:TXDP line \"%s\" is invalid\n", szLine);
	return false;
}

void IdeTxdpEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IdeTxdpEntry::Write(FILE* pFile) {
	fprintf(pFile, "%s, %s\n", m_szTxdName, m_szParentTxdName);
}

void IdeTxdpEntry::WriteBinary(FILE* pFile) {
	// not implemented
}