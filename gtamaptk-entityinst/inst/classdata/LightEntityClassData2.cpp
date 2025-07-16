#include "LightEntityClassData2.h"
#include "../EntityInstance.h"

bool LightEntityClassData2::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t nType;
	float fConeAngle, fRadius;
	int32_t nColourR, nColourG, nColourB;
	int32_t unknown1, unknown2, unknown3;
	int32_t unknown4, unknown5, unknown6;
	int32_t unknown7;
	float fSize;
	int32_t nIntensity;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %f %f %d %d %d %d %d %d %d %d %d %d %f %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&nType,
		&fConeAngle, &fRadius,
		&nColourR, &nColourG, &nColourB,
		&unknown1, &unknown2, &unknown3,
		&unknown4, &unknown5, &unknown6,
		&unknown7,
		&fSize,
		&nIntensity) == 25) {
		m_nType = nType;
		m_fConeAngle = fConeAngle;
		m_fRadius = fRadius;
		m_fColour[0] = static_cast<float>(nColourR) / 255.0f;
		m_fColour[1] = static_cast<float>(nColourG) / 255.0f;
		m_fColour[2] = static_cast<float>(nColourB) / 255.0f;
		this->unknown1 = unknown1;
		this->unknown2 = unknown2;
		this->unknown3 = unknown3;
		this->unknown4 = unknown4;
		this->unknown5 = unknown5;
		this->unknown6 = unknown6;
		this->unknown7 = unknown7;
		m_fSize = fSize;
		m_nIntensity = nIntensity;

		return true;
	}

	SetLastErrorMsg("Error: Invalid light entity class type 2 data on line \"%s\"", szLine);
	return false;
}

void LightEntityClassData2::ReadBinary(FILE* pFile) {
	BinaryLightEntityClassData2 mData;

	fread(&mData, sizeof(BinaryLightEntityClassData2), 1, pFile);

	m_nType = mData.m_nType;
	m_fConeAngle = mData.m_fConeAngle;
	m_fRadius = mData.m_fRadius;
	m_fColour[0] = mData.m_fColour[0];
	m_fColour[1] = mData.m_fColour[1];
	m_fColour[2] = mData.m_fColour[2];
	unknown1 = mData.unknown1;
	unknown2 = mData.unknown2;
	unknown3 = mData.unknown3;
	unknown4 = mData.unknown4;
	unknown5 = mData.unknown5;
	unknown6 = mData.unknown6;
	unknown7 = mData.unknown7;
	m_fSize	= mData.m_fSize;
	m_nIntensity = mData.m_nIntensity;
}

void LightEntityClassData2::Write(FILE* pFile) {
	fprintf(pFile, "%d, %g, %g, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %g, %d",
		m_nType, m_fConeAngle, m_fRadius,
		static_cast<int32_t>(m_fColour[0] * 255.0f), static_cast<int32_t>(m_fColour[1] * 255.0f), static_cast<int32_t>(m_fColour[2] * 255.0f),
		unknown1, unknown2, unknown3, unknown4, unknown5, unknown6, unknown7,
		m_fSize, m_nIntensity);
}

void LightEntityClassData2::WriteBinary(FILE* pFile) {
	BinaryLightEntityClassData2 mData;

	mData.m_nType = m_nType;
	mData.m_fConeAngle = m_fConeAngle;
	mData.m_fRadius = m_fRadius;
	mData.m_fColour[0] = m_fColour[0];
	mData.m_fColour[1] = m_fColour[1];
	mData.m_fColour[2] = m_fColour[2];
	mData.unknown1 = unknown1;
	mData.unknown2 = unknown2;
	mData.unknown3 = unknown3;
	mData.unknown4 = unknown4;
	mData.unknown5 = unknown5;
	mData.unknown6 = unknown6;
	mData.unknown7 = unknown7;
	mData.m_fSize = m_fSize;
	mData.m_nIntensity = m_nIntensity;

	fwrite(&mData, sizeof(BinaryLightEntityClassData2), 1, pFile);
}