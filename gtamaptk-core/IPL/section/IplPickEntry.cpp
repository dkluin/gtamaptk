#include "../../main/Shared.hpp"
#include "IplPickEntry.h"

bool IplPickEntry::Read(const char* szLine) {
	int32_t nWeaponType;
	Vector3d vPosition;

	if (sscanf(szLine, "%d %f %f %f", &nWeaponType, &vPosition.x, &vPosition.y, &vPosition.z) == 4) {
		m_nWeaponType = nWeaponType;
		m_vPosition = vPosition;

		return true;
	}

	SetLastErrorMsg("Error: IPL:PICK line \"%s\" is invalid\n", szLine);
	return false;
}

void IplPickEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IplPickEntry::Write(FILE* pFile) {
	fprintf(pFile, "%d, %f, %f, %f\n", m_nWeaponType, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void IplPickEntry::WriteBinary(FILE* pFile) {
	// not implemented
}