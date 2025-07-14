#include "../../main/Shared.hpp"
#include "IplTcycEntry.h"

bool IplTcycEntry::Read(const char* szLine) {
	Box mBox;
	int32_t nFarClipOverride;
	int32_t nExtraCol;
	float fExtraColIntensity;
	float fFallOffDist;
	float unused;
	float fLodDistMult;

	if (sscanf(szLine, "%f %f %f %f %f %f %d %d %f %f %f %f",
		&mBox.m_vMin.x, &mBox.m_vMin.y, &mBox.m_vMin.z,
		&mBox.m_vMax.x, &mBox.m_vMax.y, &mBox.m_vMax.z,
		&nFarClipOverride,
		&nExtraCol,
		&fExtraColIntensity,
		&fFallOffDist,
		&unused,
		&fLodDistMult) == 12) {
		m_mBox = mBox;
		m_nFarClipOverride = nFarClipOverride;
		m_nExtraCol = nExtraCol;
		m_fExtraColIntensity = fExtraColIntensity;
		m_fFallOffDist = fFallOffDist;
		m_fLodDistMult_OLD = unused;
		m_fLodDistMult = fLodDistMult;

		return true;
	}

	SetLastErrorMsg("Error: IPL:TCYC line \"%s\" is invalid\n", szLine);
	return false;
}

void IplTcycEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IplTcycEntry::Write(FILE* pFile) {
	fprintf(pFile, "%g, %g, %g, %g, %g, %g, %d, %d, %g, %g, %g, %g\n",
		m_mBox.m_vMin.x, m_mBox.m_vMin.y, m_mBox.m_vMin.z,
		m_mBox.m_vMax.x, m_mBox.m_vMax.y, m_mBox.m_vMax.z,
		m_nFarClipOverride,
		m_nExtraCol,
		m_fExtraColIntensity,
		m_fFallOffDist,
		m_fLodDistMult_OLD,
		m_fLodDistMult);
}

void IplTcycEntry::WriteBinary(FILE* pFile) {
	// not implemented
}