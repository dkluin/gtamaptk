#pragma once

#include "EntityInstance.h"

class EntityInstFile {
public:
	std::vector<EntityInstance> m_vEntityInstances;

	void ClearEntries();

	bool Read(const char* szFileName, bool bClearExisting = true);
	bool ReadBinary(const char* szFileName, bool bClearExisting = true);
	bool Write(const char* szFileName);
	bool WriteBinary(const char* szFileName);
};