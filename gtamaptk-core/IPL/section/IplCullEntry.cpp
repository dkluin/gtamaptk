#include "../../main/Shared.hpp"
#include "IplCullEntry.h"

bool IplCullEntry::Read(const char* szLine) {
	Vector3d vCenter;
	float fS1x, fS1y;
	float fS2x, fS2y;
	float fZmin, fZmax;
	int32_t nFlags;
	int32_t nWantedLevelDrop;
	Vector3d vMirrorNormal;
	float fMirrorDist;
	
	if (sscanf(szLine, "%f %f %f %f %f %f %f %f %f %d %f %f %f %f",
		&vCenter.x, &vCenter.y, &vCenter.z,
		&fS1x, &fS1y, &fZmin,
		&fS2x, &fS2y, &fZmax,
		&nFlags,
		&vMirrorNormal.x, &vMirrorNormal.y, &vMirrorNormal.z, &fMirrorDist) == 14) {
		m_eType = IPLCULL_MIRROR;
		m_vCenter = vCenter;
		m_fS1x = fS1x;
		m_fS1y = fS1y;
		m_fZmin = fZmin;
		m_fS2x = fS2x;
		m_fS2y = fS2y;
		m_fZmax = fZmax;
		m_nFlags = nFlags;
		m_mMirror.m_vNormal = vMirrorNormal;
		m_mMirror.m_fDistance = fMirrorDist;

		return true;
	} else if (sscanf(szLine, "%f %f %f %f %f %f %f %f %f %d %d",
		&vCenter.x, &vCenter.y, &vCenter.z,
		&fS1x, &fS1y, &fZmin,
		&fS2x, &fS2y, &fZmax,
		&nFlags, &nWantedLevelDrop) == 11) {
		m_eType = IPLCULL_ATTR;
		m_vCenter = vCenter;
		m_fS1x = fS1x;
		m_fS1y = fS1y;
		m_fZmin = fZmin;
		m_fS2x = fS2x;
		m_fS2y = fS2y;
		m_fZmax = fZmax;
		m_nFlags = nFlags;
		m_nWantedLevelDrop = nWantedLevelDrop;

		return true;
	}

	SetLastErrorMsg("Error: IPL:CULL line \"%s\" is invalid\n", szLine);
	return false;
}

void IplCullEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IplCullEntry::Write(FILE* pFile) {
	switch (m_eType) {
	case IPLCULL_ATTR:
		fprintf(pFile, "%g, %g, %g, %g, %g, %g, %g, %g, %f, %d, %d\n",
			m_vCenter.x, m_vCenter.y, m_vCenter.z,
			m_fS1x, m_fS1y, m_fZmin,
			m_fS2x, m_fS2y, m_fZmax,
			m_nFlags, m_nWantedLevelDrop);
		break;

	case IPLCULL_MIRROR:
		fprintf(pFile, "%g, %g, %g, %g, %g, %g, %g, %g, %g, %d, %g, %g, %g, %g\n",
			m_vCenter.x, m_vCenter.y, m_vCenter.z,
			m_fS1x, m_fS1y, m_fZmin,
			m_fS2x, m_fS2y, m_fZmax,
			m_nFlags,
			m_mMirror.m_vNormal.x, m_mMirror.m_vNormal.y, m_mMirror.m_vNormal.z, m_mMirror.m_fDistance);
		break;
	}
}

void IplCullEntry::WriteBinary(FILE* pFile) {
	// not implemented
}