#include "IplEnexEntry.h"
#include <cstring>

bool IplEnexEntry::Read(const char* szLine) {
    Vector3d vEntrancePos, vExitPos;
    EulerAngle fEntranceRot, fExitRot;
    Vector3d vSize;
    int32_t nTargetAreaID, nFlags;
    char szTempName[32];
    int32_t nExtraCol, nNumPedsToSpawn;
    int32_t nTimeOn, nTimeOff;

    if (sscanf(szLine, "%f %f %f %f %f %f %f %f %f %f %f %d %d %s %d %d %d %d",
        &vEntrancePos.x, &vEntrancePos.y, &vEntrancePos.z,
        &fEntranceRot,
        &vSize.x, &vSize.y, &vSize.z,
        &vExitPos.x, &vExitPos.y, &vExitPos.z,
        &fExitRot,
        &nTargetAreaID, &nFlags,
        szTempName,
        &nExtraCol,
        &nNumPedsToSpawn,
        &nTimeOn, &nTimeOff) == 18) {
        if (strlen(szTempName) < 3) return false;

        m_vEntrancePos = vEntrancePos;
        m_fEntranceRot = fEntranceRot;
        m_vSize = vSize;
        m_vExitPos = vExitPos;
        m_fExitRot = fExitRot;
        m_nTargetAreaID = nTargetAreaID;
        m_nFlags = nFlags;

        char* szFinalQuote = strrchr(szTempName, '"');
        if (szFinalQuote != nullptr) *szFinalQuote = '\0';

        strncpy(m_szName, &szTempName[1], MAX_GXT_ENTRY_NAME);
        m_szName[MAX_GXT_ENTRY_NAME - 1] = '\0'; // cap it just in case.

        m_nExtraCol = nExtraCol;
        m_nNumPedsToSpawn = nNumPedsToSpawn;
        m_nTimeOn = nTimeOn;
        m_nTimeOff = nTimeOff;

        return true;
    }

    SetLastErrorMsg("Error: IPL:ENEX line \"%s\" is invalid\n", szLine);
    return false;
}

void IplEnexEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
    // not implemented
}

void IplEnexEntry::Write(FILE* pFile) {
    fprintf(pFile, "%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %d, %d, \"%s\", %d, %d, %d, %d\n",
        m_vEntrancePos.x, m_vEntrancePos.y, m_vEntrancePos.z,
        m_fEntranceRot,
        m_vSize.x, m_vSize.y, m_vSize.z,
        m_vExitPos.x, m_vExitPos.y, m_vExitPos.z,
        m_fExitRot,
        m_nTargetAreaID, m_nFlags,
        m_szName,
        m_nExtraCol,
        m_nNumPedsToSpawn,
        m_nTimeOn, m_nTimeOff);
}

void IplEnexEntry::WriteBinary(FILE* pFile) {
    // not implemented
}