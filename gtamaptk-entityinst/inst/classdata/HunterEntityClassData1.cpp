#include "HunterEntityClassData1.h"
#include "../EntityInstance.h"

bool HunterEntityClassData1::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t unknown1, unknown2, unknown3;
	int32_t unknown4, unknown5, unknown6;
	int32_t unknown7, unknown8;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %d %d %d %d %d %d %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&unknown1, &unknown2, &unknown3,
		&unknown4, &unknown5, &unknown6,
		&unknown7, &unknown8) == 18) {
		this->unknown1 = unknown1;
		this->unknown2 = unknown2;
		this->unknown3 = unknown3;
		this->unknown4 = unknown4;
		this->unknown5 = unknown5;
		this->unknown6 = unknown6;
		this->unknown7 = unknown7;
		this->unknown8 = unknown8;

		return true;
	}

	SetLastErrorMsg("Error: Invalid hunter entity class type 1 data on line \"%s\"", szLine);
	return false;
}

void HunterEntityClassData1::ReadBinary(FILE* pFile) {
	BinaryHunterEntityClassData1 mData;

	fread(&mData, sizeof(BinaryHunterEntityClassData1), 1, pFile);
	
	unknown1 = mData.unknown1;
	unknown2 = mData.unknown2;
	unknown3 = mData.unknown3;
	unknown4 = mData.unknown4;
	unknown5 = mData.unknown5;
	unknown6 = mData.unknown6;
	unknown7 = mData.unknown7;
	unknown8 = mData.unknown8;
}

void HunterEntityClassData1::Write(FILE* pFile) {
	fprintf(pFile, "%d, %d, %d, %d, %d, %d, %d, %d",
		unknown1, unknown2, unknown3, unknown4,
		unknown5, unknown6, unknown7, unknown8);
}

void HunterEntityClassData1::WriteBinary(FILE* pFile) {
	BinaryHunterEntityClassData1 mData;

	mData.unknown1 = unknown1;
	mData.unknown2 = unknown2;
	mData.unknown3 = unknown3;
	mData.unknown4 = unknown4;
	mData.unknown5 = unknown5;
	mData.unknown6 = unknown6;
	mData.unknown7 = unknown7;
	mData.unknown8 = unknown8;

	fwrite(&mData, sizeof(BinaryHunterEntityClassData1), 1, pFile);
}