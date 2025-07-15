#include "IplFile.h"

void IplFile::ClearEntries() {
	m_vAuzoEntries.clear();
	m_vCarsEntries.clear();
	m_vCullEntries.clear();
	m_vEnexEntries.clear();
	m_vGrgeEntries.clear();
	m_vInstEntries.clear();
	m_vJumpEntries.clear();
	m_vOcclEntries.clear();
	m_vPickEntries.clear();
	m_vTcycEntries.clear();
	m_vZoneEntries.clear();
}

bool IplFile::Read(const char* szFileName, bool bClearExisting) {
	FILE* pFile = fopen(szFileName, "r");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to open IPL: %s\n", szFileName);
		return false;
	}

	if (bClearExisting)	{
		ClearEntries();
	}

	int8_t nSection = IPL_SECTION_NONE;
	for (char* szLine = ReadLine(pFile); szLine != nullptr; szLine = ReadLine(pFile)) {
		if (*szLine == '\0' || *szLine == '#') continue;

		if (nSection == IPL_SECTION_NONE) {
			if (SECTION4(szLine, 'a', 'u', 'z', 'o')) nSection = IPL_SECTION_AUZO;
			else if (SECTION4(szLine, 'c', 'a', 'r', 's')) nSection = IPL_SECTION_CARS;
			else if (SECTION4(szLine, 'c', 'u', 'l', 'l')) nSection = IPL_SECTION_CULL;
			else if (SECTION4(szLine, 'e', 'n', 'e', 'x')) nSection = IPL_SECTION_ENEX;
			else if (SECTION4(szLine, 'g', 'r', 'g', 'e')) nSection = IPL_SECTION_GRGE;
			else if (SECTION4(szLine, 'i', 'n', 's', 't')) nSection = IPL_SECTION_INST;
			else if (SECTION4(szLine, 'j', 'u', 'm', 'p')) nSection = IPL_SECTION_JUMP;
			else if (SECTION4(szLine, 'o', 'c', 'c', 'l')) nSection = IPL_SECTION_OCCL;
			else if (SECTION4(szLine, 'p', 'i', 'c', 'k')) nSection = IPL_SECTION_PICK;
			else if (SECTION4(szLine, 't', 'c', 'y', 'c')) nSection = IPL_SECTION_TCYC;
			else if (SECTION4(szLine, 'z', 'o', 'n', 'e')) nSection = IPL_SECTION_ZONE;
		} else if (SECTION3(szLine, 'e', 'n', 'd')) {
			nSection = IDE_SECTION_NONE;
		} else {
			switch (nSection) {
			case IPL_SECTION_AUZO:
			{
				IplAuzoEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vAuzoEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_CARS:
			{
				IplCarsEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vCarsEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_ENEX:
			{
				IplEnexEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vEnexEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_GRGE:
			{
				IplGrgeEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vGrgeEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_INST:
			{
				IplInstEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vInstEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_JUMP:
			{
				IplJumpEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vJumpEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_OCCL:
			{
				IplOcclEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vOcclEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_PICK:
			{
				IplPickEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vPickEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_TCYC:
			{
				IplTcycEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vTcycEntries.push_back(mEntry);
			}
			break;

			case IPL_SECTION_ZONE:
			{
				IplZoneEntry mEntry;
				if (!mEntry.Read(szLine)) continue;

				m_vZoneEntries.push_back(mEntry);
			}
			break;
			}
		}
	}

	fclose(pFile);
	return true;
}

bool IplFile::ReadBinary(const char* szFileName, bool bClearExisting) {
	static uint8_t gBuffer[1000000]; // 1mb. no sane person would use binary ipls larger than this
	BinaryIplHeader mHeader;

	FILE* pFile = fopen(szFileName, "rb");
	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to open binary IPL: %s\n", szFileName);
		return false;
	}

	if (bClearExisting) {
		ClearEntries();
	}

	fread(gBuffer, sizeof(gBuffer), 1, pFile);
	memcpy(&mHeader, gBuffer, sizeof(mHeader));

	for (int32_t i = 0; i < mHeader.m_nNumInstEntries; i++) {
		IplInstEntry mEntry;
		int32_t nOffset;
		
		nOffset = mHeader.m_nInstBlockOffset + sizeof(BinaryIplInstData) * i;
		mEntry.ReadBinary(gBuffer, nOffset);

		m_vInstEntries.push_back(mEntry);
	}

	for (int32_t i = 0; i < mHeader.m_nNumCarsEntries; i++) {
		IplCarsEntry mEntry;
		int32_t nOffset;

		nOffset = mHeader.m_nCarsBlockOffset + sizeof(BinaryIplCarsData) * i;
		mEntry.ReadBinary(gBuffer, nOffset);

		m_vCarsEntries.push_back(mEntry);
	}

	fclose(pFile);
	return true;
}

bool IplFile::Write(const char* szFileName) {
	FILE* pFile = fopen(szFileName, "w");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to create / overwrite IPL: %s\n", szFileName);
		return false;
	}

	// Section order is identical to files exported by the original R* map exporter
	// Leftovers like the path section are not implemented atm
	WriteSection(pFile, "inst", m_vInstEntries);
	WriteSection(pFile, "cull", m_vCullEntries);
	WriteSection(pFile, "grge", m_vGrgeEntries);
	WriteSection(pFile, "enex", m_vEnexEntries);
	WriteSection(pFile, "pick", m_vPickEntries);
	WriteSection(pFile, "jump", m_vJumpEntries);
	WriteSection(pFile, "tcyc", m_vTcycEntries);
	WriteSection(pFile, "auzo", m_vAuzoEntries);
	WriteSection(pFile, "occl", m_vOcclEntries); // Not exported by the map exporter but appended either way

	fclose(pFile);
	return true;
}

bool IplFile::WriteBinary(const char* szFileName) {
	BinaryIplHeader mHeader;

	mHeader.m_dwIdentifier = 0x626E7279; // bnry
	mHeader.m_nNumInstEntries = m_vInstEntries.size();
	mHeader.m_nNumCullEntries = 0;
	mHeader.m_nNumGrgeEntries = 0;
	mHeader.m_nNumEnexEntries = 0;
	mHeader.m_nNumCarsEntries = m_vCarsEntries.size();
	mHeader.m_nNumPickEntries = 0;

	mHeader.m_nInstBlockOffset = sizeof(BinaryIplHeader);
	mHeader.m_nInstBlockSize = sizeof(BinaryIplInstData) * m_vInstEntries.size();
	mHeader.m_nCullBlockOffset = mHeader.m_nInstBlockOffset + mHeader.m_nInstBlockSize;
	mHeader.m_nCullBlockSize = 0;
	mHeader.m_nGrgeBlockOffset = mHeader.m_nCullBlockOffset + mHeader.m_nCullBlockSize;
	mHeader.m_nGrgeBlockSize = 0;
	mHeader.m_nEnexBlockOffset = mHeader.m_nGrgeBlockOffset + mHeader.m_nGrgeBlockSize;
	mHeader.m_nEnexBlockSize = 0;
	mHeader.m_nCarsBlockOffset = mHeader.m_nEnexBlockOffset + mHeader.m_nEnexBlockSize;
	mHeader.m_nCarsBlockSize = sizeof(BinaryIplCarsData) * m_vCarsEntries.size();

	FILE* pFile = fopen(szFileName, "wb");

	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to create / overwrite binary IPL: %s\n", szFileName);
		return false;
	}

	fwrite(&mHeader, sizeof(mHeader), 1, pFile);

	for (auto it : m_vInstEntries) it.WriteBinary(pFile);
	for (auto it : m_vCarsEntries) it.WriteBinary(pFile);

	fclose(pFile);
	return true;
}