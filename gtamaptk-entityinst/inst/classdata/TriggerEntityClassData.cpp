#include "TriggerEntityClassData.h"
#include "../EntityInstance.h"

bool TriggerEntityClassData::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t nType;
	float fSize;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %f",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&nType,
		&fSize) == 12) {
		m_nType = nType;
		m_fSize = fSize;

		return true;
	}

	SetLastErrorMsg("Error: Invalid trigger entity class data on line \"%s\"", szLine);
	return false;
}

void TriggerEntityClassData::ReadBinary(FILE* pFile) {
	BinaryTriggerEntityClassData mData;

	fread(&mData, sizeof(BinaryTriggerEntityClassData), 1, pFile);

	m_nType = mData.m_nType;
	m_fSize = mData.m_fSize;
}

void TriggerEntityClassData::Write(FILE* pFile) {
	fprintf(pFile, "%d, %g", m_nType, m_fSize);
}

void TriggerEntityClassData::WriteBinary(FILE* pFile) {
	BinaryTriggerEntityClassData mData;

	mData.m_nType = m_nType;
	mData.m_fSize = m_fSize;

	fwrite(&mData, sizeof(BinaryTriggerEntityClassData), 1, pFile);
}