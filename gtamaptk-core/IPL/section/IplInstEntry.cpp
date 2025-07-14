#include "IplInstEntry.h"
#include <cstring>

bool IplInstEntry::Read(const char* szLine) {
	int32_t nModelIndex;
	char szModelName[MAX_IMG_ENTRY_NAME];
	uint32_t dwAreaAndFlags;
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t nLodIndex;

	if (sscanf(szLine, "%d %s %d %f %f %f %f %f %f %f %d",
		&nModelIndex, &szModelName, &dwAreaAndFlags,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&nLodIndex) == 11) {
		m_nModelIndex = nModelIndex;
		strcpy(m_szModelName, szModelName);
		m_dwAreaAndFlags = dwAreaAndFlags;
		m_vPosition = vPosition;
		m_qRotation = qRotation;
		m_nLodIndex = nLodIndex;

		return true;
	}

	SetLastErrorMsg("Error: IDE:INST line \"%s\" is invalid\n", szLine);
	return false;
}

void IplInstEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	BinaryIplInstData mData;

	memcpy(&mData, reinterpret_cast<BinaryIplInstData*>(pFileContent + nOffset), sizeof(BinaryIplInstData));

	m_nModelIndex = mData.m_nModelIndex;
	sprintf(m_szModelName, "id_%d", m_nModelIndex); // because it's unknown :^)
	m_dwAreaAndFlags = mData.m_dwAreaAndFlags;
	m_vPosition = mData.m_vPosition;
	m_qRotation = mData.m_qRotation;
	m_nLodIndex = mData.m_nLodIndex;
}

void IplInstEntry::Write(FILE* pFile) {
	fprintf(pFile, "%d, %s, %d, %.8g, %.8g, %.8g, %.10g, %.10g, %.10g, %.10g, %d\n",
		m_nModelIndex, m_szModelName, m_dwAreaAndFlags,
		m_vPosition.x, m_vPosition.y, m_vPosition.z,
		m_qRotation.x, m_qRotation.y, m_qRotation.z, m_qRotation.w,
		m_nLodIndex);
}

void IplInstEntry::WriteBinary(FILE* pFile) {
	BinaryIplInstData mData;

	mData.m_vPosition = m_vPosition;
	mData.m_qRotation = m_qRotation;
	mData.m_nModelIndex = m_nModelIndex;
	mData.m_dwAreaAndFlags = m_dwAreaAndFlags;
	mData.m_nLodIndex = m_nLodIndex;

	fwrite(&mData, sizeof(mData), 1, pFile);
}