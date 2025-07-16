#include "LightEntityClassData1.h"
#include "../EntityInstance.h"

bool LightEntityClassData1::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t unknown1, unknown2, unknown3;
	int32_t nColourR, nColourG, nColourB;
	int32_t unknown4, unknown5, unknown6;
	int32_t unknown7;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %d %d %d %d %d %d %d %d %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&unknown1, &unknown2, &unknown3,
		&nColourR, &nColourG, &nColourB,
		&unknown4, &unknown5, &unknown6,
		&unknown7) == 20) {
		this->unknown1 = unknown1;
		this->unknown2 = unknown2;
		this->unknown3 = unknown3;
		this->m_fColour[0] = static_cast<float>(nColourR) / 255.0f;
		this->m_fColour[1] = static_cast<float>(nColourG) / 255.0f;
		this->m_fColour[2] = static_cast<float>(nColourB) / 255.0f;
		this->unknown4 = unknown4;
		this->unknown5 = unknown5;
		this->unknown6 = unknown6;
		this->unknown7 = unknown7;

		return true;
	}

	SetLastErrorMsg("Error: Invalid light entity class type 1 data on line \"%s\"", szLine);
	return false;
}

void LightEntityClassData1::ReadBinary(FILE* pFile) {
	BinaryLightEntityClassData1 mData;

	fread(&mData, sizeof(BinaryLightEntityClassData1), 1, pFile);

	unknown1 = mData.unknown1;
	unknown2 = mData.unknown2;
	unknown3 = mData.unknown3;
	m_fColour[0] = mData.m_fColour[0];
	m_fColour[1] = mData.m_fColour[1];
	m_fColour[2] = mData.m_fColour[2];
	unknown4 = mData.unknown4;
	unknown5 = mData.unknown5;
	unknown6 = mData.unknown6;
	unknown7 = mData.unknown7;
}

void LightEntityClassData1::Write(FILE* pFile) {
	fprintf(pFile, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		unknown1, unknown2, unknown3,
		static_cast<int32_t>(m_fColour[0] * 255.0f), static_cast<int32_t>(m_fColour[1] * 255.0f), static_cast<int32_t>(m_fColour[2] * 255.0f),
		unknown4, unknown5, unknown6, unknown7);
}

void LightEntityClassData1::WriteBinary(FILE* pFile) {
	LightEntityClassData1 mData;

	mData.unknown1 = unknown1;
	mData.unknown2 = unknown2;
	mData.unknown3 = unknown3;
	mData.m_fColour[0] = m_fColour[0];
	mData.m_fColour[1] = m_fColour[1];
	mData.m_fColour[2] = m_fColour[2];
	mData.unknown5 = unknown5;
	mData.unknown6 = unknown6;
	mData.unknown7 = unknown7;

	fwrite(&mData, sizeof(BinaryLightEntityClassData1), 1, pFile);
}