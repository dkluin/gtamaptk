#pragma once

#include "../IDE/IdeFile.h"
#include "../IPL/IplFile.h"
#include <map>

class GameMap {
private:
	std::map<int32_t, IdeObjsEntry> m_mObjsDefMap;
	std::map<int32_t, IdeTobjEntry> m_mTobjDefMap;
	std::map<int32_t, IdeAnimEntry> m_mAnimDefMap;

	std::vector<IdeFile> m_vIdeFiles;
	std::vector<IplFile> m_vIplFiles;

public:
	inline bool IsObjsEntry(int32_t nModelIndex) const {
		return m_mObjsDefMap.find(nModelIndex) != m_mObjsDefMap.end();
	}

	inline bool IsTobjEntry(int32_t nModelIndex) const {
		return m_mTobjDefMap.find(nModelIndex) != m_mTobjDefMap.end();
	}

	inline bool IsAnimEntry(int32_t nModelIndex) const {
		return m_mAnimDefMap.find(nModelIndex) != m_mAnimDefMap.end();
	}

	inline IdeObjsEntry& GetObjsEntry(int32_t nModelIndex) {
		return m_mObjsDefMap[nModelIndex];
	}

	inline IdeTobjEntry& GetTobjEntry(int32_t nModelIndex) {
		return m_mTobjDefMap[nModelIndex];
	}

	inline IdeAnimEntry& GetAnimEntry(int32_t nModelIndex) {
		return m_mAnimDefMap[nModelIndex];
	}

	bool Load(const char* szGameDirectory, bool bReadIDEs, bool bReadIPLs); // equivalent of CFileLoader::LoadLevel; it loads gta.dat and stores all IDE/IPL entries 
};