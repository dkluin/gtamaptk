#pragma once

#include "../../main/Shared.hpp"
#include "../../main/MapFileSection.h"

enum eObjsEntryType {
	IDEOBJS_TYPE_1, // Id, ModelName, TxdName, MeshCount, DrawDistance, Flags
	IDEOBJS_TYPE_2, // Id, ModelName, TxdName, MeshCount, DrawDistance1, DrawDistance2, Flags
	IDEOBJS_TYPE_3, // Id, ModelName, TxdName, MeshCount, DrawDistance1, DrawDistance2, DrawDistance3, Flags
	IDEOBJS_TYPE_4 // Id, ModelName, TxdName, DrawDistance, Flags
};

// https://github.com/dkluin/Underground_Core/blob/master/Game/ModelInfo/ModelInfo.h#L54
enum eModelInfoFlags {
	//// Basic
	MINFO_FLAG_WETROADS = (1 << 0),				// 1 - Use wet road reflections				
	MINFO_FLAG_NOFADE = (1 << 1),				// 2 - Don't fade. Unused in SA.	
	MINFO_FLAG_DRAWLAST = (1 << 2) | (1 << 3),	// 4 | 8 - Draw after opaque geometry
	MINFO_FLAG_ADDITIVE = (1 << 3),				// 8 - Additive blending
	MINFO_FLAG_SUBWAY = (1 << 4),				// 16 - Is a subway object. Unused in SA.
	MINFO_FLAG_NOBACKFACECULLING = (1 << 21),	// 2097152 - Disable backface culling

	//// Clumps
	MINFO_FLAG_DOOR = (1 << 5),					// 32 - This is a door

	//// Atomics
	MINFO_FLAG_NOZWRITE = (1 << 6),				// 64 - Don't write to Z buffer
	MINFO_FLAG_NOSHADOWS = (1 << 7),			// 128 - Don't cast shadows on me
	MINFO_FLAG_IGNOREDRAWDIST = (1 << 8),		// 256 - Ignore draw distance. Unused in SA.
	MINFO_FLAG_CODEGLASS = (1 << 9),			// 512 - Code glass
	MINFO_FLAG_ARTISTGLASS = (1 << 10),			// 1024 - Artistic glass
	MINFO_FLAG_GARAGEDOOR = (1 << 11),			// 2048 - Garage door
	MINFO_FLAG_DAMAGEABLE = (1 << 12),			// 4096 - Damageable. Not for timed objects.
	MINFO_FLAG_TREE = (1 << 13),				// 8192 - Is a tree which moves with the wind
	MINFO_FLAG_PALMTREE = (1 << 14),			// 16384 - Is a palm tree which moves with the wind
	MINFO_FLAG_NOFLYERCOLLIDE = (1 << 15),		// 32768 - Don't collide with flyer
	MINFO_FLAG_UNUSED2 = (1 << 16),				// 65536 - Used by numerous objects but does nothing
	MINFO_FLAG_UNUSED3 = (1 << 17),				// 131072 - Used by numerous objects but does nothing
	MINFO_FLAG_UNUSED4 = (1 << 18),				// 262144 - Used by numerous objects but does nothing
	MINFO_FLAG_UNUSED5 = (1 << 19),				// 524288 - Used by numerous objects but does nothing
	MINFO_FLAG_TAG = (1 << 20),					// 1048576 - Sprayable tag
	MINFO_FLAG_NOCOVER = (1 << 22),				// 4194304 - Peds dont take cover 
	MINFO_FLAG_WETONLY = (1 << 23),				// 8388608 - Wet only
};

class IdeObjsEntry : public IMapFileSectionEntry {
public:
	eObjsEntryType m_eType;
	int32_t m_nModelIndex;
	char m_szModelName[MAX_IMG_ENTRY_NAME];
	char m_szTxdName[MAX_IMG_ENTRY_NAME];
	int8_t m_nObjectCount;
	float m_fDrawDistance[3];
	uint32_t m_dwModelFlags;

	int8_t GetType() { return IDE_SECTION_OBJS; }
	bool Read(const char* szLine);
	void ReadBinary(uint8_t* pFileContent, int32_t nOffset);
	void Write(FILE* pFile);
	void WriteBinary(FILE* pFile);
};