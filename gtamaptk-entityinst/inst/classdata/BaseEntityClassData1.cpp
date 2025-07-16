#include "BaseEntityClassData1.h"
#include "../EntityInstance.h"

bool BaseEntityClassData1::Read(const char* szLine) {
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

	SetLastErrorMsg("Error: Invalid base entity class type 1 data on line \"%s\"", szLine);
	return false;
}

void BaseEntityClassData1::ReadBinary(FILE* pFile) {
	BinaryBaseEntityClassData1 mData;

	fread(&mData, sizeof(BinaryBaseEntityClassData1), 1, pFile);

	m_nHitPoints = mData.m_nHitPoints;
}

void BaseEntityClassData1::Write(FILE* pFile) {
	fprintf(pFile, "%d", m_nHitPoints);
}

void BaseEntityClassData1::WriteBinary(FILE* pFile) {
	BinaryBaseEntityClassData1 mData;

	mData.m_nHitPoints = m_nHitPoints;

	fwrite(&mData, sizeof(BinaryBaseEntityClassData1), 1, pFile);
}