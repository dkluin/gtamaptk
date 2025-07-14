#pragma once

#include "../../main/MapFileSection.h"
#include "../../math/Vector3d.h"
#include "../../math/RGBA.h"

#define MAX_2DFX_SPRITENAME 32

enum eIde2dfxType {
	IDE2DFX_LIGHT,
	IDE2DFX_PARTICLE,
	IDE2DFX_ATTRACTOR,
	IDE2DFX_PED_ATTRACTOR,
	IDE2DFX_SUNGLARE
};

enum eIde2dfxLightFlag {
	IDE2DFX_LIGHT_LOSCHECK = 1,					// disable z test
	IDE2DFX_LIGHT_FOG_NORMAL = 2,				// can have light and fog
	IDE2DFX_LIGHT_FOG_ALWAYS = 4,				// fog only
	IDE2DFX_LIGHT_HIDE_OBJECT = 8,				// Not available in GTA: III
	IDE2DFX_LIGHT_LONG_DIST = 16				// Not available in GTA: III
};

enum eIde2dfxLightFlashType {
	IDE2DFX_LIGHTFLASH_ON,
	IDE2DFX_LIGHTFLASH_ON_NIGHT,
	IDE2DFX_LIGHTFLASH_FLICKER,					// Equivalent of RANDOM_FLASHING in SA
	IDE2DFX_LIGHTFLASH_FLICKER_NIGHT,			// Equivalent of RANDOM_FLASHING in SA
	IDE2DFX_LIGHTFLASH_FLASH1,					// Equivalent of LIGHTS_ANIM_SPEED_4X in SA
	IDE2DFX_LIGHTFLASH_FLASH1_NIGHT,			// Equivalent of LIGHTS_ANIM_SPEED_4X in SA
	IDE2DFX_LIGHTFLASH_FLASH2,					// Equivalent of LIGHTS_ANIM_SPEED_2X in SA
	IDE2DFX_LIGHTFLASH_FLASH2_NIGHT,			// Equivalent of LIGHTS_ANIM_SPEED_2X in SA
	IDE2DFX_LIGHTFLASH_FLASH3,					// Equivalent of LIGHTS_ANIM_SPEED_1X in SA
	IDE2DFX_LIGHTFLASH_FLASH3_NIGHT,			// Equivalent of LIGHTS_ANIM_SPEED_1X in SA
	IDE2DFX_LIGHTFLASH_RANDOM_FLICKER,			// Equivalent of BLINKING1 flag in SA
	IDE2DFX_LIGHTFLASH_RANDOM_FLICKER_NIGHT,	// Equivalent of BLINKING1 flag in SA
	IDE2DFX_LIGHTFLASH_SPECIAL,					// Used for traffic lights etc
	IDE2DFX_LIGHTFLASH_BRIDGE_FLASH1,
	IDE2DFX_LIGHTFLASH_BRIDGE_FLASH2,
};

enum eIde2dfxLightFlareType {
	IDE2DFX_LIGHTFLARE_DISABLED,
	IDE2DFX_LIGHTFLARE_YELLOW,
	IDE2DFX_LIGHTFLARE_WHITE
};

enum eIde2dfxParticleType {
	IDE2DFX_PARTICLE_PAVEMENT_STEAM,
	IDE2DFX_PARTICLE_WALL_STEAM,
	IDE2DFX_PARTICLE_DRY_ICE,
	IDE2DFX_PARTICLE_SMALL_FIRE,
	IDE2DFX_PARTICLE_DARK_SMOKE,
	IDE2DFX_PARTICLE_WATER_FOUNTAIN_VERT,
	IDE2DFX_PARTICLE_WATER_FOUNTAIN_HORIZ
};

// This is only used in GTA: III, but is still read in VC etc.
// 
enum eIde2dfxAttractorType {
	IDE2DFX_ATTRACTOR_ICECREAM,
	IDE2DFX_ATTRACTOR_STARE
};

enum eIde2dfxPedAttractorType {
	IDE2DFX_PEDATTRACTOR_ATM,
	IDE2DFX_PEDATTRACTOR_SIT,
	IDE2DFX_PEDATTRACTOR_WAIT_BUS,
	IDE2DFX_PEDATTRACTOR_SHOPPING, // looking at store fronts
	IDE2DFX_PEDATTRACTOR_RAIN_SHELTER
};

class Ide2dfxEntry : public IMapFileSectionEntry {
public:
	int32_t m_nModelIndex;
	eIde2dfxType m_eType;
	Vector3d m_vPosition;
	RGBA m_mColour;

	union {
		struct {
			char m_szCoronaSprite[MAX_2DFX_SPRITENAME];
			char m_szShadowSprite[MAX_2DFX_SPRITENAME];
			float m_fCoronaFarClip;
			float m_fPointlightSize;
			float m_fCoronaSize;
			float m_fShadowSize;
			int8_t m_nFlashType;
			bool m_bWetReflection;
			int8_t m_nFlareType;
			int8_t m_nFlags;
		} m_mLightData;

		struct {
			int8_t m_nParticleType;
			Vector3d m_vDir;
			float m_fScale;
		} m_mParticleData;

		struct {
			int8_t m_nType;
			Vector3d m_vDir;
			uint8_t m_nProbability;
		} m_mAttractorData;

		struct {
			int8_t m_nType;
			Vector3d m_vQueueDir;
			Vector3d m_vUseDir;
		} m_mPedAttractorData;
	};

	int8_t GetType() { return IDE_SECTION_2DFX; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};