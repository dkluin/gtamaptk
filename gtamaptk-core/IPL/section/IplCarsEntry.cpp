#include "../../main/Shared.hpp"
#include "IplCarsEntry.h"

bool IplCarsEntry::Read(const char* szLine) {
	Vector3d vPosition;
	RadianAngle fRotation;
	int32_t nModelIndex;
	int32_t nPrimaryColour, nSecondaryColour;
	BOOL bForceSpawn;
	int32_t nAlarmChance, nLockedChance;
	int32_t nMinDelay, nMaxDelay;

	if (sscanf(szLine, "%f %f %f %f %d %d %d %d %d %d %d %d",
		&vPosition.x, &vPosition.y, &vPosition.z,
		&fRotation,
		&nModelIndex,
		&nPrimaryColour, &nSecondaryColour,
		&bForceSpawn,
		&nAlarmChance, &nLockedChance,
		&nMinDelay, &nMaxDelay) == 12) {
		m_vPosition = vPosition;
		m_fRotation = fRotation;
		m_nModelIndex = nModelIndex;
		m_nPrimaryColour = nPrimaryColour;
		m_nSecondaryColour = nSecondaryColour;
		m_bForceSpawn = bForceSpawn;
		m_nAlarmChance = nAlarmChance;
		m_nLockedChance = nLockedChance;
		m_nMinDelay = nMinDelay;
		m_nMaxDelay = nMaxDelay;

		return true;
	}

	SetLastErrorMsg("Error: IPL:CARS line \"%s\" is invalid\n", szLine);
	return false;
}

void IplCarsEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	BinaryIplCarsData mData;

	memcpy(&mData, reinterpret_cast<BinaryIplCarsData*>(pFileContent + nOffset), sizeof(BinaryIplCarsData));

	m_vPosition = mData.m_vPosition;
	m_fRotation = mData.m_fRotation;
	m_nModelIndex = mData.m_nModelIndex;
	m_nPrimaryColour = mData.m_nPrimaryColour;
	m_nSecondaryColour = mData.m_nSecondaryColour;
	m_bForceSpawn = mData.m_bForceSpawn;
	m_nAlarmChance = mData.m_nAlarmChance;
	m_nLockedChance = mData.m_nLockedChance;
	m_nMinDelay = mData.m_nMinDelay;
	m_nMaxDelay = mData.m_nMaxDelay;
}

void IplCarsEntry::Write(FILE* pFile) {
	fprintf(pFile, "%g, %g, %g, %g, %d, %d, %d, %d, %d, %d, %d, %d\n",
		m_vPosition.x, m_vPosition.y, m_vPosition.z,
		m_fRotation,
		m_nModelIndex,
		m_nPrimaryColour, m_nSecondaryColour,
		m_bForceSpawn,
		m_nAlarmChance, m_nLockedChance,
		m_nMinDelay, m_nMaxDelay);
}

void IplCarsEntry::WriteBinary(FILE* pFile) {
	BinaryIplCarsData mEntry;

	mEntry.m_vPosition = m_vPosition;
	mEntry.m_fRotation = m_fRotation;
	mEntry.m_nModelIndex = m_nModelIndex;
	mEntry.m_nPrimaryColour = m_nPrimaryColour;
	mEntry.m_nSecondaryColour = m_nSecondaryColour;
	mEntry.m_bForceSpawn = m_bForceSpawn;
	mEntry.m_nAlarmChance = m_nAlarmChance;
	mEntry.m_nLockedChance = m_nLockedChance;
	mEntry.m_nMinDelay = m_nMinDelay;
	mEntry.m_nMaxDelay = m_nMaxDelay;

	fwrite(&mEntry, 1, sizeof(BinaryIplCarsData), pFile);
}