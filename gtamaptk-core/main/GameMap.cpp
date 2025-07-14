#include "Shared.hpp"
#include "GameMap.h"

bool GameMap::Load(const char* szGameDirectory, bool bReadIDEs, bool bReadIPLs) {
	char szLevelFile[MAX_PATH];
	sprintf(szLevelFile, "%s/data/gta.dat", szGameDirectory);

	FILE* pFile = fopen(szLevelFile, "r");
	
	if (pFile == nullptr) {
		SetLastErrorMsg("Error: Unable to open level file: %s", szLevelFile);
		return false;
	}

	for (char* szLine = ReadLine(pFile); szLine != nullptr; szLine = ReadLine(pFile)) {
		if (*szLine == '#') continue;
		if (strncmp(szLine, "EXIT", 4) == 0) break;

		if (bReadIDEs && strncmp(szLine, "IDE", 3) == 0) {
			char szIdeFile[MAX_PATH];
			sprintf(szIdeFile, "%s/%s", szGameDirectory, szLine + 4);

			IdeFile mIdeFile;
			if (!mIdeFile.Read(szIdeFile)) {
				SetLastErrorMsg("Error: Unable to read IDE file %s, skipping", szIdeFile);
				continue;
			}

			for (auto& it : mIdeFile.m_vObjsEntries)
				m_mObjsDefMap[it.m_nModelIndex] = it;
			for (auto& it : mIdeFile.m_vTobjEntries)
				m_mTobjDefMap[it.m_nModelIndex] = it;
			for (auto& it : mIdeFile.m_vAnimEntries)
				m_mAnimDefMap[it.m_nModelIndex] = it;

			m_vIdeFiles.push_back(mIdeFile);
		} else if (bReadIPLs && strncmp(szLine, "IPL", 3) == 0) {
			char szIplFile[MAX_PATH];
			sprintf(szIplFile, "%s/%s", szGameDirectory, szLine + 4);

			IplFile mIplFile;
			if (!mIplFile.Read(szIplFile)) {
				SetLastErrorMsg("Error: Unable to read IPL file %s, skipping", szIplFile);
				continue;
			}

			m_vIplFiles.push_back(mIplFile);
		}
	}

	fclose(pFile);
	return true;
}