#include "BaseEntityClassData2.h"
#include "../EntityInstance.h"

bool BaseEntityClassData2::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t nHitPoints;
	int32_t unknown1;
	int32_t unknown2;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %d %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&nHitPoints,
		&unknown1, &unknown2) == 13) {
		m_nHitPoints = nHitPoints;
		this->unknown1 = unknown1;
		this->unknown2 = unknown2;

		return true;
	}

	SetLastErrorMsg("Error: Invalid base entity class type 2 data on line \"%s\"", szLine);
	return false;
}

void BaseEntityClassData2::ReadBinary(FILE* pFile) {
	BinaryBaseEntityClassData2 mData;

	fread(&mData, sizeof(BinaryBaseEntityClassData2), 1, pFile);

	m_nHitPoints = mData.m_nHitPoints;
	unknown1 = mData.unknown1;
	unknown2 = mData.unknown2;
}

void BaseEntityClassData2::Write(FILE* pFile) {
	fprintf(pFile, "%d, %d, %d", m_nHitPoints, unknown1, unknown2);
}

void BaseEntityClassData2::WriteBinary(FILE* pFile) {
	BinaryBaseEntityClassData2 mData;

	mData.m_nHitPoints = m_nHitPoints;
	mData.unknown1 = unknown1;
	mData.unknown2 = unknown2;

	fwrite(&mData, sizeof(BinaryBaseEntityClassData2), 1, pFile);
}