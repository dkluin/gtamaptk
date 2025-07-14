#pragma once

#include "section/IplAuzoEntry.h"
#include "section/IplCarsEntry.h"
#include "section/IplCullEntry.h"
#include "section/IplEnexEntry.h"
#include "section/IplGrgeEntry.h"
#include "section/IplInstEntry.h"
#include "section/IplJumpEntry.h"
#include "section/IplOcclEntry.h"
#include "section/IplPickEntry.h"
#include "section/IplTcycEntry.h"
#include "section/IplZoneEntry.h"
#include <vector>

#pragma pack(push, 1)
struct BinaryIplHeader {
	uint32_t m_dwIdentifier;
	int32_t m_nNumInstEntries;
	int32_t m_nNumCullEntries; // guessing based on text IPL format. it's very plausible that R* implemented this at one point but we know for certain that the game does not read this at all
	int32_t m_nNumGrgeEntries; // guessing based on text IPL format. it's very plausible that R* implemented this at one point but we know for certain that the game does not read this at all
	int32_t m_nNumEnexEntries; // guessing based on text IPL format. it's very plausible that R* implemented this at one point but we know for certain that the game does not read this at all
	int32_t m_nNumCarsEntries; 
	int32_t m_nNumPickEntries; // guessing based on text IPL format. it's very plausible that R* implemented this at one point but we know for certain that the game does not read this at all
	
	int32_t m_nInstBlockOffset;
	int32_t m_nInstBlockSize; // unused
	int32_t m_nCullBlockOffset; // unused
	int32_t m_nCullBlockSize; // unused
	int32_t m_nGrgeBlockOffset; // unused
	int32_t m_nGrgeBlockSize; // unused
	int32_t m_nEnexBlockOffset; // unused
	int32_t m_nEnexBlockSize; // unused
	int32_t m_nCarsBlockOffset; 
	int32_t m_nCarsBlockSize; // unused
	int32_t m_nPickBlockOffset; // unused
	int32_t m_nPickBlockSize; // unused
};
#pragma pack(pop)

class IplFile {
public:
	std::vector<IplAuzoEntry> m_vAuzoEntries;
	std::vector<IplCarsEntry> m_vCarsEntries;
	std::vector<IplCullEntry> m_vCullEntries;
	std::vector<IplEnexEntry> m_vEnexEntries;
	std::vector<IplGrgeEntry> m_vGrgeEntries;
	std::vector<IplInstEntry> m_vInstEntries;
	std::vector<IplJumpEntry> m_vJumpEntries;
	std::vector<IplOcclEntry> m_vOcclEntries;
	std::vector<IplPickEntry> m_vPickEntries;
	std::vector<IplTcycEntry> m_vTcycEntries;
	std::vector<IplZoneEntry> m_vZoneEntries;

	void ClearEntries();

	bool Read(const char* szFileName, bool bClearExisting = true);
	bool ReadBinary(const char* szFileName, bool bClearExisting = true);
	bool Write(const char* szFileName);
	bool WriteBinary(const char* szFileName);

private:
	template <typename T>
	void WriteSection(FILE* pFile, const char* szSectionName, std::vector<T>& vEntries) {
		fprintf(pFile, "%s\n", szSectionName);
		for (auto it : vEntries) it.Write(pFile);
		fprintf(pFile, "end\n");
	}
};