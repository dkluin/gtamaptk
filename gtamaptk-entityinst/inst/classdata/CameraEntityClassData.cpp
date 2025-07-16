#include "CameraEntityClassData.h"
#include "../EntityInstance.h"

bool CameraEntityClassData::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t unknown1, unknown2;
	float unknown3, unknown4;
	int32_t unknown5;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %d %f %f %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&unknown1, &unknown2, 
		&unknown3, &unknown4, 
		&unknown5) == 15) {
		this->unknown1 = unknown1;
		this->unknown2 = unknown2;
		this->unknown3 = unknown3;
		this->unknown4 = unknown4;
		this->unknown5 = unknown5;

		return true;
	}

	SetLastErrorMsg("Error: Invalid camera entity class data on line \"%s\"", szLine);
	return false;
}

void CameraEntityClassData::ReadBinary(FILE* pFile) {
	BinaryCameraEntityClassData mData;

	fread(&mData, sizeof(BinaryCameraEntityClassData), 1, pFile);

	unknown1 = mData.unknown1;
	unknown2 = mData.unknown2;
	unknown3 = mData.unknown3;
	unknown4 = mData.unknown4;
	unknown5 = mData.unknown5;
}

void CameraEntityClassData::Write(FILE* pFile) {
	fprintf(pFile, "%d, %d, %g, %g, %d",
		unknown1, unknown2,
		unknown3, unknown4,
		unknown5);
}

void CameraEntityClassData::WriteBinary(FILE* pFile) {
	BinaryCameraEntityClassData mData;

	mData.unknown1 = unknown1;
	mData.unknown2 = unknown2;
	mData.unknown3 = unknown3;
	mData.unknown4 = unknown4;
	mData.unknown5 = unknown5;

	fwrite(&mData, sizeof(BinaryCameraEntityClassData), 1, pFile);
}