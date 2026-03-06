#include "Ide2dfxEntry.h"
#include <vcruntime_string.h>
#include <string.h>

bool Ide2dfxEntry::Read(const char* szLine) {
    int32_t nModelIndex;
	Vector3d vPosition;
    RGBA mColour;
    int32_t nType;

    int32_t nNumConsumedChars = 0;
    if (sscanf(szLine, "%d %f %f %f %hhu %hhu %hhu %hhu %d%n", &nModelIndex, &vPosition.x, &vPosition.y, &vPosition.z, &mColour.r, &mColour.g, &mColour.b, &mColour.a, &nType, &nNumConsumedChars) < 9) {
        SetLastErrorMsg("Error: Incorrect IDE:2DFX line (too few fields): \"%s\"\n", szLine);
        return false;
    }

    m_nModelIndex = nModelIndex;
    m_vPosition = vPosition;
    m_mColour = mColour;
    m_eType = (eIde2dfxType)nType;

    // Skip to after the 9th field (type)
	const char* pExtraFields = szLine + nNumConsumedChars;
    if (pExtraFields == nullptr) {
        // For types with no extra fields (e.g. sun glare), we're done
        if (m_eType == IDE2DFX_SUNGLARE) {
            return true;
        }

        SetLastErrorMsg("Error: IDE:2DFX line missing extra fields after type: \"%s\"\n", szLine);
        return false;
	}

    switch (m_eType) {
        case IDE2DFX_LIGHT: {
			char szCoronaTex[MAX_2DFX_SPRITENAME];
			char szShadowTex[MAX_2DFX_SPRITENAME];
            float fFarClip, fPointlightSize, fCoronaSize, fShadowSize;
			int32_t nShadowIntensity, nFlashType, nFlareType, nFlags;
			int32_t bWet;

            if (sscanf(pExtraFields, " \"%31[^\"]\" \"%31[^\"]\" %f %f %f %f %d %d %d %d %d",
					   szCoronaTex, szShadowTex, &fFarClip, &fPointlightSize, &fCoronaSize, &fShadowSize,
                       &nShadowIntensity, &nFlashType, &bWet, &nFlareType, &nFlags) != 11) {
                SetLastErrorMsg("Error: IDE:2DFX LIGHT line has wrong number of fields: \"%s\"\n", szLine);
                return false;
            }

            strncpy(m_mLightData.m_szCoronaSprite, szCoronaTex, MAX_2DFX_SPRITENAME - 1);
			m_mLightData.m_szCoronaSprite[MAX_2DFX_SPRITENAME - 1] = '\0';

            strncpy(m_mLightData.m_szShadowSprite, szShadowTex, MAX_2DFX_SPRITENAME - 1);
			m_mLightData.m_szShadowSprite[MAX_2DFX_SPRITENAME - 1] = '\0';

            m_mLightData.m_fCoronaFarClip = fFarClip;
            m_mLightData.m_fPointlightSize = fPointlightSize;
            m_mLightData.m_fCoronaSize = fCoronaSize;
            m_mLightData.m_fShadowSize = fShadowSize;
			m_mLightData.m_nShadowIntensity = nShadowIntensity;
            m_mLightData.m_nFlashType = nFlashType;
            m_mLightData.m_bWetReflection = (bWet != 0);
            m_mLightData.m_nFlareType = nFlareType;
            m_mLightData.m_nFlags = nFlags;
            break;
        }

        case IDE2DFX_PARTICLE: {
            int32_t nParticleType;
			Vector3d vDir;
            float fScale;

            if (sscanf(pExtraFields, " %d %f %f %f %f", &nParticleType, &vDir.x, &vDir.y, &vDir.z, &fScale) != 5) {
                SetLastErrorMsg("Error: IDE:2DFX PARTICLE line has wrong number of fields: \"%s\"\n", szLine);
                return false;
            }

            m_mParticleData.m_nParticleType = nParticleType;
            m_mParticleData.m_vDir = vDir;
            m_mParticleData.m_fScale = fScale;
            break;
        }

        case IDE2DFX_ATTRACTOR: {
            int32_t nAttractorType;
			Vector3d vDir;
            int32_t nProbability;

            if (sscanf(pExtraFields, " %d %f %f %f %d", &nAttractorType, &vDir.x, &vDir.y, &vDir.z, &nProbability) != 5) {
                SetLastErrorMsg("Error: IDE:2DFX ATTRACTOR line has wrong number of fields: \"%s\"\n", szLine);
                return false;
            }

            m_mAttractorData.m_nType = nAttractorType;
            m_mAttractorData.m_vDir = vDir;
			m_mAttractorData.m_nProbability = nProbability;
            break;
        }

        case IDE2DFX_PED_ATTRACTOR: {
            int32_t nPedType;
			Vector3d vQueueDir, vUseDir;

            if (sscanf(pExtraFields, " %d %f %f %f %f %f %f", &nPedType, &vQueueDir.x, &vQueueDir.y, &vQueueDir.z, &vUseDir.x, &vUseDir.y, &vUseDir.z) != 7) {
                SetLastErrorMsg("Error: IDE:2DFX PED_ATTRACTOR line has wrong number of fields (expected 7 after type): \"%s\"\n", szLine);
                return false;
            }

            m_mPedAttractorData.m_nType = nPedType;
			m_mPedAttractorData.m_vQueueDir = vQueueDir;
            m_mPedAttractorData.m_vUseDir = vUseDir;
            break;
        }

        case IDE2DFX_SUNGLARE:
            // No extra fields
            break;

        default:
            SetLastErrorMsg("Error: Unknown 2DFX type %d in line: \"%s\"\n", nType, szLine);
            return false;
    }

    return true;
}

void Ide2dfxEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {

}

void Ide2dfxEntry::Write(FILE* pFile) {
    // not implemented yet
}

void Ide2dfxEntry::WriteBinary(FILE* pFile) {

}