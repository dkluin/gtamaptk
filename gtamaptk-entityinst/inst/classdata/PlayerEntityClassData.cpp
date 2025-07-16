#include "PlayerEntityClassData.h"
#include "../EntityInstance.h"

bool PlayerEntityClassData::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t nHitPoints;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&nHitPoints) == 11) {
		m_nHitPoints = nHitPoints;

		return true;
	}

	SetLastErrorMsg("Error: Invalid player entity class data on line \"%s\"", szLine);
	return false;
}

void PlayerEntityClassData::ReadBinary(FILE* pFile) {
	BinaryPlayerEntityClassData mData;

	fread(&mData, sizeof(BinaryPlayerEntityClassData), 1, pFile);

	m_nHitPoints = mData.m_nHitPoints;
}

void PlayerEntityClassData::Write(FILE* pFile) {
	fprintf(pFile, "%d", m_nHitPoints);
}

void PlayerEntityClassData::WriteBinary(FILE* pFile) {
	BinaryPlayerEntityClassData mData;

	mData.m_nHitPoints = m_nHitPoints;

	fwrite(&mData, sizeof(BinaryPlayerEntityClassData), 1, pFile);
}