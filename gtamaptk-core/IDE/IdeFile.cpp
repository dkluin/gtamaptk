#include "IdeFile.h"

void IdeFile::ClearEntries() {
	m_v2dfxEntries.clear();
	m_vAnimEntries.clear();
	m_vObjsEntries.clear();
	m_vTobjEntries.clear();
	m_vTxdpEntries.clear();
}

bool IdeFile::Read(const char* szFileName, bool bClearExisting) {
	FILE* pFile = fopen(szFileName, "r");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to open IDE: %s\n", szFileName);
		return false;
	}

	if (bClearExisting) {
		ClearEntries();
	}

	int8_t nSection = IDE_SECTION_NONE;
	for (char* szLine = ReadLine(pFile); szLine != nullptr; szLine = ReadLine(pFile)) {
		if (*szLine == '\0' || *szLine == '#') continue;

		if (nSection == IDE_SECTION_NONE) {
			if (SECTION4(szLine, 'o', 'b', 'j', 's')) nSection = IDE_SECTION_OBJS;
			else if (SECTION4(szLine, 't', 'o', 'b', 'j')) nSection = IDE_SECTION_TOBJ;
			else if (SECTION4(szLine, '2', 'd', 'f', 'x')) nSection = IDE_SECTION_2DFX;
			else if (SECTION4(szLine, 'a', 'n', 'i', 'm')) nSection = IDE_SECTION_ANIM;
			else if (SECTION4(szLine, 't', 'x', 'd', 'p')) nSection = IDE_SECTION_TXDP;
		} else if (SECTION3(szLine, 'e', 'n', 'd')) {
			nSection = IDE_SECTION_NONE;
		} else {
			switch (nSection) {
			case IDE_SECTION_OBJS:
			{
				IdeObjsEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vObjsEntries.push_back(mEntry);
			}
			break;

			case IDE_SECTION_TOBJ:
			{
				IdeTobjEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vTobjEntries.push_back(mEntry);
			}
			break;

			case IDE_SECTION_2DFX:
			{
				Ide2dfxEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_v2dfxEntries.push_back(mEntry);
			}
			break;

			case IDE_SECTION_ANIM:
			{
				IdeAnimEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vAnimEntries.push_back(mEntry);
			}
			break;

			case IDE_SECTION_TXDP:
			{
				IdeTxdpEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vTxdpEntries.push_back(mEntry);
			}
			break;
			}
		}
	}

	fclose(pFile);
	return true;
}

bool IdeFile::Write(const char* szFileName) {
	FILE* pFile = fopen(szFileName, "w");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to create / overwrite IDE: %s\n", szFileName);
		return false;
	}

	// Section order is identical to files exported by the original R* map exporter
	WriteSection(pFile, "objs", m_vObjsEntries);
	WriteSection(pFile, "tobj", m_vTobjEntries);
	WriteSection(pFile, "2dfx", m_v2dfxEntries);
	WriteSection(pFile, "anim", m_vAnimEntries);
	WriteSection(pFile, "txdp", m_vTxdpEntries);

	fclose(pFile);
	return true;
}