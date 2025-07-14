#include "../../main/Shared.hpp"
#include "IplOcclEntry.h"

bool IplOcclEntry::Read(const char* szLine) {
    Vector2d vCenter;
    float fBottom;
    float fWidth, fLength, fHeight;
    EulerAngle fRotX, fRotY, fRotZ;
    int32_t nListIndex;

    if (sscanf(szLine, "%f %f %f %f %f %f %f %f %f %d",
        &vCenter.x, &vCenter.y,
        &fBottom,
        &fWidth, &fLength, &fHeight,
        &fRotX, &fRotY, &fRotZ,
        &nListIndex) == 11) {
        m_vCenter = vCenter;
        m_fBottom = fBottom;
        m_fWidth = fWidth;
        m_fLength = fLength;
        m_fHeight = fHeight;
        m_fRotX = fRotX;
        m_fRotY = fRotY;
        m_fRotZ = fRotZ;
        m_nListIndex = nListIndex;

        return true;
    }

    SetLastErrorMsg("Error: IPL:OCCL line \"%s\" is invalid\n", szLine);
    return false;
}

void IplOcclEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
    // not implemented
}

void IplOcclEntry::Write(FILE* pFile) {
    fprintf(pFile, "%g, %g, %g, %g, %g, %g, %g, %g, %g, %d\n",
        m_vCenter.x, m_vCenter.y,
        m_fBottom,
        m_fWidth, m_fLength, m_fHeight,
        m_fRotX, m_fRotY, m_fRotZ,
        m_nListIndex);
}

void IplOcclEntry::WriteBinary(FILE* pFile) {
    // not implemented
}