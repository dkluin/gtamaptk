#include "IplAuzoEntry.h"
#include <cstring>

bool IplAuzoEntry::Read(const char* szLine) {
	char szZoneName[MAX_AUZO_NAME];
	int32_t nTrackID;
	int32_t bOnByDefault;
	Box mBox;
	Sphere mSphere;

	if (sscanf(szLine, "%s %d %d %f %f %f %f %f %f", szZoneName, &nTrackID, &bOnByDefault, &mBox.m_vMin.x, &mBox.m_vMin.y, &mBox.m_vMin.z, &mBox.m_vMax.x, &mBox.m_vMax.y, &mBox.m_vMax.z) == 9) {
		m_eType = IPLAUZO_BOX;
		strcpy(m_szName, szZoneName);
		m_nTrackID = nTrackID;
		m_bOnByDefault = bOnByDefault;
		m_mBox = mBox;

		return true;
	} else if (sscanf(szLine, "%s %d %d %f %f %f %f", szZoneName, &nTrackID, &bOnByDefault, &mSphere.m_vCenter.x, &mSphere.m_vCenter.y, &mSphere.m_vCenter.z, &mSphere.m_fRadius) == 7) {
		m_eType = IPLAUZO_SPHERE;
		strcpy(m_szName, szZoneName);
		m_nTrackID = nTrackID;
		m_bOnByDefault = bOnByDefault;
		m_mSphere = mSphere;

		return true;
	}

	SetLastErrorMsg("Error: IPL:AUZO line \"%s\" is invalid\n", szLine);
	return false;
}

void IplAuzoEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IplAuzoEntry::Write(FILE* pFile) {
	switch (m_eType) {
	case IPLAUZO_BOX:
		fprintf(pFile, "%s, %d, %d, %g, %g, %g, %g, %g, %g\n",
			m_szName, m_nTrackID, m_bOnByDefault,
			m_mBox.m_vMin.x, m_mBox.m_vMin.y, m_mBox.m_vMin.z,
			m_mBox.m_vMax.x, m_mBox.m_vMax.y, m_mBox.m_vMax.z);
		break;

	case IPLAUZO_SPHERE:
		fprintf(pFile, "%s, %d, %d, %g, %g, %g, %g\n",
			m_szName, m_nTrackID, m_bOnByDefault,
			m_mSphere.m_vCenter.x, m_mSphere.m_vCenter.y, m_mSphere.m_vCenter.z,
			m_mSphere.m_fRadius);
		break;
	}
}

void IplAuzoEntry::WriteBinary(FILE* pFile) {
	// not implemented
}