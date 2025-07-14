#pragma once

#include "section/Ide2dfxEntry.h"
#include "section/IdeAnimEntry.h"
#include "section/IdeObjsEntry.h"
#include "section/IdeTobjEntry.h"
#include "section/IdeTxdpEntry.h"
#include <vector>

class IdeFile {
public:
	std::vector<Ide2dfxEntry> m_v2dfxEntries;
	std::vector<IdeAnimEntry> m_vAnimEntries;
	std::vector<IdeObjsEntry> m_vObjsEntries;
	std::vector<IdeTobjEntry> m_vTobjEntries;
	std::vector<IdeTxdpEntry> m_vTxdpEntries;

	void ClearEntries();
	bool Read(const char* szFileName, bool bClearExisting = true);
	bool Write(const char* szFileName);

private:
	template <typename T>
	void WriteSection(FILE* pFile, const char* szSectionName, std::vector<T>& vEntries) {
		fprintf(pFile, "%s\n", szSectionName);
		for (auto it : vEntries) it.Write(pFile);
		fprintf(pFile, "end\n");
	}
};