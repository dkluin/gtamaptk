#include "PlayerEntityClassData.h"
#include "../EntityInstance.h"

bool PlayerEntityClassData::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t unknown;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&unknown) == 11) {
		this->unknown = unknown;

		return true;
	}

	SetLastErrorMsg("Error: Invalid player entity class data on line \"%s\"", szLine);
	return false;
}

void PlayerEntityClassData::ReadBinary(FILE* pFile) {
	BinaryPlayerEntityClassData mData;

	fread(&mData, sizeof(BinaryPlayerEntityClassData), 1, pFile);

	unknown = mData.unknown;
}

void PlayerEntityClassData::Write(FILE* pFile) {
	fprintf(pFile, "%d", unknown);
}

void PlayerEntityClassData::WriteBinary(FILE* pFile) {
	BinaryPlayerEntityClassData mData;

	mData.unknown = unknown;

	fwrite(&mData, sizeof(BinaryPlayerEntityClassData), 1, pFile);
}