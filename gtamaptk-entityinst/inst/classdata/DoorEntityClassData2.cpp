#include "DoorEntityClassData2.h"
#include "../EntityInstance.h"

bool DoorEntityClassData2::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t unknown1, unknown2, unknown3;
	int32_t unknown4, unknown5;
	
	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %d %d %d %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&unknown1, &unknown2, &unknown3,
		&unknown4, &unknown5) == 15) {
		this->unknown1 = unknown1;
		this->unknown2 = unknown2;
		this->unknown3 = unknown3;
		this->unknown4 = unknown4;
		this->unknown5 = unknown5;

		return true;
	}

	SetLastErrorMsg("Error: Invalid door entity class type 2 data on line \"%s\"", szLine);
	return false;
}

void DoorEntityClassData2::ReadBinary(FILE* pFile) {
	BinaryDoorEntityClassData2 mData;

	fread(&mData, sizeof(BinaryDoorEntityClassData2), 1, pFile);

	unknown1 = mData.unknown1;
	unknown2 = mData.unknown2;
	unknown3 = mData.unknown3;
	unknown4 = mData.unknown4;
	unknown5 = mData.unknown5;
}

void DoorEntityClassData2::Write(FILE* pFile) {
	fprintf(pFile, "%d, %d, %d, %d, %d", unknown1, unknown2, unknown3, unknown4, unknown5);
}

void DoorEntityClassData2::WriteBinary(FILE* pFile) {
	BinaryDoorEntityClassData2 mData;

	mData.unknown1 = unknown1;
	mData.unknown2 = unknown2;
	mData.unknown3 = unknown3;
	mData.unknown4 = unknown4;
	mData.unknown5 = unknown5;

	fwrite(&mData, sizeof(BinaryDoorEntityClassData2), 1, pFile);
}