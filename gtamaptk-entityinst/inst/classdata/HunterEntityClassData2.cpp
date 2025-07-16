#include "HunterEntityClassData2.h"
#include "../EntityInstance.h"

bool HunterEntityClassData2::Read(const char* szLine) {
	char szEntityType[MAX_PADDED_STRING];
	char szInternalEntityName[MAX_PADDED_STRING];
	char szEntityClassName[MAX_PADDED_STRING];
	Vector3d vPosition;
	Quaternion qRotation;
	int32_t nHitPoints;
	int32_t nSlots[3];
	char szMeleeWeaponType[MAX_PADDED_STRING];
	char szWeaponType[MAX_PADDED_STRING];
	int32_t nBehaviorType;
	int32_t nDropAmmo;
	int32_t nFlags;

	if (sscanf(szLine, "%s %s %s %f %f %f %f %f %f %f %d %d %d %d %s %s %d %d %d",
		szEntityType, szInternalEntityName, szEntityClassName,
		&vPosition.x, &vPosition.y, &vPosition.z,
		&qRotation.x, &qRotation.y, &qRotation.z, &qRotation.w,
		&nHitPoints,
		&nSlots[0], &nSlots[1], &nSlots[2],
		szMeleeWeaponType, szWeaponType,
		&nBehaviorType,
		&nDropAmmo,
		&nFlags) == 19) {
		int32_t nMeleeWeaponType, nWeaponType;

		nMeleeWeaponType = FindMeleeWeaponType(szMeleeWeaponType);
		nWeaponType = FindWeaponType(szWeaponType);

		if (nMeleeWeaponType == -1) {
			SetLastErrorMsg("Error: Unknown melee weapon type %s used in hunter entity class type 2 line: \"%s\"", szMeleeWeaponType, szLine);
			return false;
		}

		if (nWeaponType == -1) {
			SetLastErrorMsg("Error: Unknown weapon type %s used in hunter entity class type 2 line: \"%s\"", szWeaponType, szLine);
			return false;
		}

		m_nHitPoints = nHitPoints;
		m_nSlots[0] = nSlots[0];
		m_nSlots[1] = nSlots[1];
		m_nSlots[2] = nSlots[2];
		m_nMeleeWeaponType = nMeleeWeaponType;
		m_nWeaponType = nWeaponType;
		m_nBehaviorType = nBehaviorType;
		m_nDropAmmo = nDropAmmo;
		m_nFlags = nFlags;

		return true;
	}

	SetLastErrorMsg("Error: Invalid hunter entity class type 2 data on line \"%s\"", szLine);
	return false;
}

void HunterEntityClassData2::ReadBinary(FILE* pFile) {
	BinaryHunterEntityClassData2 mData;

	fread(&mData, sizeof(BinaryHunterEntityClassData2), 1, pFile);

	m_nHitPoints = mData.m_nHitPoints;
	m_nSlots[0] = mData.m_nSlots[0];
	m_nSlots[1] = mData.m_nSlots[1];
	m_nSlots[2] = mData.m_nSlots[2];
	m_nMeleeWeaponType = mData.m_nMeleeWeaponType;
	m_nWeaponType = mData.m_nWeaponType;
	m_nBehaviorType = mData.m_nBehaviorType;
	m_nDropAmmo = mData.m_nDropAmmo;
	m_nFlags = mData.m_nFlags;
}

void HunterEntityClassData2::Write(FILE* pFile) {
	fprintf(pFile, "%d, %d, %d, %d, %s, %s, %d, %d, %d",
		m_nHitPoints,
		m_nSlots[0], m_nSlots[1], m_nSlots[2],
		gszMeleeWeaponTypeNames[m_nMeleeWeaponType],
		gszWeaponTypeNames[m_nWeaponType],
		m_nBehaviorType,
		m_nDropAmmo,
		m_nFlags);
}

void HunterEntityClassData2::WriteBinary(FILE* pFile) {
	BinaryHunterEntityClassData2 mData;

	mData.m_nHitPoints = m_nHitPoints;
	mData.m_nSlots[0] = m_nSlots[0];
	mData.m_nSlots[1] = m_nSlots[1];
	mData.m_nSlots[2] = m_nSlots[2];
	mData.m_nMeleeWeaponType = m_nMeleeWeaponType;
	mData.m_nWeaponType = m_nWeaponType;
	mData.m_nBehaviorType = m_nBehaviorType;
	mData.m_nDropAmmo = m_nDropAmmo;
	mData.m_nFlags = m_nFlags;

	fwrite(&mData, sizeof(BinaryHunterEntityClassData2), 1, pFile);
}