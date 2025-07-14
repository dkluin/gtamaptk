#include "../../main/Shared.hpp"
#include "IplJumpEntry.h"

bool IplJumpEntry::Read(const char* szLine) {
	Box mStartArea;
	Box mTargetArea;
	Vector3d vCameraPos;
	int32_t nReward;

	if (sscanf(szLine, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d",
		&mStartArea.m_vMin.x, &mStartArea.m_vMin.y, &mStartArea.m_vMin.z,
		&mStartArea.m_vMax.x, &mStartArea.m_vMax.y, &mStartArea.m_vMax.z, 
		&mTargetArea.m_vMin.x, &mTargetArea.m_vMin.y, &mTargetArea.m_vMin.z,
		&mTargetArea.m_vMax.x, &mTargetArea.m_vMax.y, &mTargetArea.m_vMax.z,
		&vCameraPos.x, &vCameraPos.y, &vCameraPos.z,
		&nReward) == 14) {
		m_mStartArea = mStartArea;
		m_mTargetArea = mTargetArea;
		m_vCameraPos = vCameraPos;
		m_nReward = nReward;

		return true;
	}

	SetLastErrorMsg("Error: IPL:JUMP line \"%s\" is invalid\n", szLine);
	return false;
}

void IplJumpEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IplJumpEntry::Write(FILE* pFile) {
	fprintf(pFile, "%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %d\n",
		m_mStartArea.m_vMin.x, m_mStartArea.m_vMin.y, m_mStartArea.m_vMin.z,
		m_mStartArea.m_vMax.x, m_mStartArea.m_vMax.y, m_mStartArea.m_vMax.z,
		m_mTargetArea.m_vMin.x, m_mTargetArea.m_vMin.y, m_mTargetArea.m_vMin.z,
		m_mTargetArea.m_vMax.x, m_mTargetArea.m_vMax.y, m_mTargetArea.m_vMax.z,
		m_vCameraPos.x, m_vCameraPos.y, m_vCameraPos.z,
		m_nReward);
}

void IplJumpEntry::WriteBinary(FILE* pFile) {
	// not implemented
}