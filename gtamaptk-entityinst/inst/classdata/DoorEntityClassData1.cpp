#include "DoorEntityClassData1.h"
#include "../EntityInstance.h"

bool DoorEntityClassData1::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t nHitPoints;
	int32_t unknown;
	int32_t nFlags;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %d %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&nHitPoints,
		&unknown,
		&nFlags) == 13)
	{
		m_nHitPoints = nHitPoints;
		this->unknown = unknown;
		m_nFlags = nFlags;

		return true;
	}

	SetLastErrorMsg("Error: Invalid door entity class type 1 data on line \"%s\"", szLine);
	return false;
}

void DoorEntityClassData1::ReadBinary(FILE* pFile) {
	BinaryDoorEntityClassData1 mData;

	fread(&mData, sizeof(BinaryDoorEntityClassData1), 1, pFile);

	m_nHitPoints = mData.m_nHitPoints;
	unknown = mData.unknown;
	m_nFlags = mData.m_nFlags;
}

void DoorEntityClassData1::Write(FILE* pFile) {
	fprintf(pFile, "%d, %d, %d", m_nHitPoints, unknown, m_nFlags);
}

void DoorEntityClassData1::WriteBinary(FILE* pFile) {
	BinaryDoorEntityClassData1 mData;

	mData.m_nHitPoints = m_nHitPoints;
	mData.unknown = unknown;
	mData.m_nFlags = m_nFlags;

	fwrite(&mData, sizeof(BinaryDoorEntityClassData1), 1, pFile);
}