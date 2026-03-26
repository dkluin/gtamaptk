#include <gtamaptk-core.h>
#include <iostream>
#include <rw.h>
#include <Windows.h>
#include "librwgta/rwgta.h"

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

// Used to convert III/VC particle IDs to their SA counterparts
const char* gsz2dfxParticleNameMap[7] = {
	"vent",
	"vent2",
	"prt_sand",
	"fire",
	"prt_smokeII_3_expand",
	"water_fountain",
	"water_splash"
};

bool FileExists(const char* szFile) {
	FILE* pFile = fopen(szFile, "rb");

	if (pFile != nullptr) {
		fclose(pFile);
		return true;
	}

	return false;
}

std::vector<Ide2dfxEntry> Get2dfxEntriesForModel(IdeFile mIdeFile, int32_t nModelIndex) {
	std::vector<Ide2dfxEntry> vEntries;

	for (auto& it : mIdeFile.m_v2dfxEntries) {
		if (it.m_nModelIndex != nModelIndex) continue;

		vEntries.push_back(it);
	}

	return vEntries;
}

rw::Clump* ReadDFF(const char* szFilePath) {
	rw::Clump* pClump;
	rw::ChunkHeaderInfo mHeader;
	rw::StreamFile mStream;

	if (mStream.open(szFilePath, "rb") == nullptr) {
		printf("Error: Unable to open DFF: %s\n", szFilePath);
		return nullptr;
	}

	readChunkHeaderInfo(&mStream, &mHeader);
	rw::version = mHeader.version;
	rw::build= mHeader.build;

	if (mHeader.type != rw::ID_CLUMP) {
		printf("Error: First section in %s must be ID_CLUMP\n", szFilePath);

		mStream.close();
		return nullptr;
	}

	pClump = rw::Clump::streamRead(&mStream);
	mStream.close();

	if (pClump == nullptr) {
		printf("Error: Couldn't read clump in %s\n", szFilePath);
		return nullptr;
	}

	return pClump;
}

bool WriteDFF(const char* szFilePath, rw::Clump* pClump) {
	rw::StreamFile mStream;

	if (!mStream.open(szFilePath, "wb")) {
		printf("Error: Unable to open DFF output file: %s\n", szFilePath);
		return false;
	}

	pClump->streamWrite(&mStream);
	mStream.close();
	return true;
}

bool Convert2dfxDataFor(const char* szDffInputFolder, const char* szDffOutputFolder, IdeFile mIdeFile, int32_t nModelIndex, const char* szModelName) {
	char szDffInputPath[MAX_PATH];
	sprintf(szDffInputPath, "%s/%s.dff", szDffInputFolder, szModelName);

	if (!FileExists(szDffInputPath)) {
		printf("Unable to find %s.dff, skipping\n", szModelName);
		return false;
	}

	std::vector<Ide2dfxEntry> v2dfxEntries = Get2dfxEntriesForModel(mIdeFile, nModelIndex);
	if (v2dfxEntries.size() == 0) return false;
	
	rw::Clump* pClump = ReadDFF(szDffInputPath);
	if (pClump == nullptr || pClump->countAtomics() == 0) return false;

	rw::Geometry* pGeometry = nullptr; // main geometry (first encountered is expected to receive the 2dfx data)
	FORLIST(pLink, pClump->atomics) {
		rw::Atomic* pAtomic = rw::Atomic::fromClump(pLink);
		rw::Geometry* _pGeometry = pAtomic->geometry;

		if (_pGeometry == nullptr) continue;
		pGeometry = _pGeometry;
		break;
	}

	if (pGeometry == nullptr) {
		printf("%s.dff has no usable geometry\n", szModelName);
		return false;
	}

	rw::uint8* p2dfxBuffer = new rw::uint8[v2dfxEntries.size() * sizeof(gta::Effect2d) + 4];
	memset(p2dfxBuffer, 0, v2dfxEntries.size() * sizeof(gta::Effect2d) + 4);

	*PLUGINOFFSET(rw::uint8*, pGeometry, gta::twodEffectOffset) = p2dfxBuffer;

	*(rw::int32*)p2dfxBuffer = v2dfxEntries.size();
	gta::Effect2d* pEffects = (gta::Effect2d*)(p2dfxBuffer + 4);

	for (int32_t i = 0; i < v2dfxEntries.size(); i++) {
		gta::Effect2d* pEffect = &pEffects[i];
		Ide2dfxEntry& mIde2dfxEntry = v2dfxEntries.at(i);

		pEffect->posn.x = mIde2dfxEntry.m_vPosition.x;
		pEffect->posn.y = mIde2dfxEntry.m_vPosition.y;
		pEffect->posn.z = mIde2dfxEntry.m_vPosition.z;

		switch (mIde2dfxEntry.m_eType) {
			case IDE2DFX_LIGHT:
				pEffect->type = gta::ET_LIGHT;

				pEffect->attr.l.col.red = mIde2dfxEntry.m_mColour.r;
				pEffect->attr.l.col.green = mIde2dfxEntry.m_mColour.g;
				pEffect->attr.l.col.blue = mIde2dfxEntry.m_mColour.b;
				pEffect->attr.l.col.alpha = mIde2dfxEntry.m_mColour.a;

				pEffect->attr.l.lodDist = mIde2dfxEntry.m_mLightData.m_fCoronaFarClip;
				pEffect->attr.l.size = mIde2dfxEntry.m_mLightData.m_fPointlightSize;
				pEffect->attr.l.coronaSize = mIde2dfxEntry.m_mLightData.m_fCoronaSize;
				pEffect->attr.l.shadowSize = mIde2dfxEntry.m_mLightData.m_fShadowSize;
						
				if (mIde2dfxEntry.m_mLightData.m_nFlags & IDE2DFX_LIGHT_LOSCHECK)
					pEffect->attr.l.flags |= gta::LF_LOSCHECK;
				if (mIde2dfxEntry.m_mLightData.m_nFlags & IDE2DFX_LIGHT_FOG_NORMAL)
					pEffect->attr.l.flags |= gta::LF_FOG_NORMAL;
				if (mIde2dfxEntry.m_mLightData.m_nFlags & IDE2DFX_LIGHT_FOG_ALWAYS)
					pEffect->attr.l.flags |= gta::LF_FOG_ALWAYS;
				if (mIde2dfxEntry.m_mLightData.m_nFlags & IDE2DFX_LIGHT_HIDE_OBJECT)
					pEffect->attr.l.flags |= gta::LF_HIDE_OBJECT;
				if (mIde2dfxEntry.m_mLightData.m_nFlags & IDE2DFX_LIGHT_LONG_DIST)
					pEffect->attr.l.flags |= gta::LF_LONG_DIST;

				switch (mIde2dfxEntry.m_mLightData.m_nFlashType) {
					case IDE2DFX_LIGHTFLASH_ON:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_DAY;
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						break;

					case IDE2DFX_LIGHTFLASH_ON_NIGHT:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						break;

					case IDE2DFX_LIGHTFLASH_FLICKER:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_DAY;
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_RANDOM_FLASHINESS;
						break;

					case IDE2DFX_LIGHTFLASH_FLICKER_NIGHT:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_RANDOM_FLASHINESS;
						break;

					case IDE2DFX_LIGHTFLASH_FLASH1:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_DAY;
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_ONCE_SECOND;
						break;

					case IDE2DFX_LIGHTFLASH_FLASH1_NIGHT:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_ONCE_SECOND;
						break;

					case IDE2DFX_LIGHTFLASH_FLASH2:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_DAY;
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_TWICE_SECOND;
						break;

					case IDE2DFX_LIGHTFLASH_FLASH2_NIGHT:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_TWICE_SECOND;
						break;

					case IDE2DFX_LIGHTFLASH_FLASH3:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_DAY;
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_FIVE_SECOND;
						break;

					case IDE2DFX_LIGHTFLASH_FLASH3_NIGHT:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_FIVE_SECOND;
						break;

					case IDE2DFX_LIGHTFLASH_RANDOM_FLICKER:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_DAY;
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flags |= gta::LF_BLINKING;
						break;

					case IDE2DFX_LIGHTFLASH_RANDOM_FLICKER_NIGHT:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flags |= gta::LF_BLINKING;
						break;

					case IDE2DFX_LIGHTFLASH_SPECIAL:
						pEffect->attr.l.flags |= gta::LF_ACTIVE_DAY;
						pEffect->attr.l.flags |= gta::LF_ACTIVE_NIGHT;
						pEffect->attr.l.flashiness = gta::FL_OFF;
						break;
				}

				pEffect->attr.l.reflectionType = mIde2dfxEntry.m_mLightData.m_bWetReflection ? 1 : 0;
				pEffect->attr.l.lensFlareType = mIde2dfxEntry.m_mLightData.m_nFlareType;

				pEffect->attr.l.shadowAlpha = mIde2dfxEntry.m_mLightData.m_nShadowIntensity;
				pEffect->attr.l.shadowDepth = 0;
				pEffect->attr.l.lightDirX = 0;
				pEffect->attr.l.lightDirY = 0;
				pEffect->attr.l.lightDirZ = 100;

				strcpy(pEffect->attr.l.coronaTex, mIde2dfxEntry.m_mLightData.m_szCoronaSprite);
				pEffect->attr.l.coronaTex[23] = '\0';

				strcpy(pEffect->attr.l.shadowTex, mIde2dfxEntry.m_mLightData.m_szShadowSprite);
				pEffect->attr.l.shadowTex[23] = '\0';
				break;

			case IDE2DFX_PARTICLE:
				pEffect->type = gta::ET_PARTICLE;

				assert(mIde2dfxEntry.m_mParticleData.m_nParticleType >= 0 && mIde2dfxEntry.m_mParticleData.m_nParticleType <= 7);
				strcpy(pEffect->attr.p.name, gsz2dfxParticleNameMap[mIde2dfxEntry.m_mParticleData.m_nParticleType]);
				pEffect->attr.p.name[23] = '\0';
				break;

			case IDE2DFX_LOOKATPOINT:
				pEffect->type = gta::ET_PEDQUEUE;

				pEffect->attr.q.useDir.x = mIde2dfxEntry.m_mLookAtPointData.m_vDir.x;
				pEffect->attr.q.useDir.y = mIde2dfxEntry.m_mLookAtPointData.m_vDir.y;
				pEffect->attr.q.useDir.z = mIde2dfxEntry.m_mLookAtPointData.m_vDir.z;

				pEffect->attr.q.queueDir = pEffect->attr.q.useDir;
				pEffect->attr.q.forwardDir = pEffect->attr.q.useDir;

				switch (mIde2dfxEntry.m_mLookAtPointData.m_nType) {
					case IDE2DFX_LOOKATPOINT_ICECREAM: pEffect->attr.q.type = gta::QT_BUS_STOP; break;
					case IDE2DFX_LOOKATPOINT_STARE: pEffect->attr.q.type = gta::QT_LOOK_AT; break;
				}

				pEffect->attr.q.interest = (rw::uint8)((float)mIde2dfxEntry.m_mLookAtPointData.m_nProbability / 255.0f * 100.0f);
				pEffect->attr.q.lookAt = 0;
				pEffect->attr.q.flags = 0;

				strcpy(pEffect->attr.q.scriptName, "none");
				break;

			case IDE2DFX_PEDQUEUE:
				pEffect->type = gta::ET_PEDQUEUE;

				pEffect->attr.q.queueDir.x = mIde2dfxEntry.m_mPedQueueData.m_vQueueDir.x;
				pEffect->attr.q.queueDir.y = mIde2dfxEntry.m_mPedQueueData.m_vQueueDir.y;
				pEffect->attr.q.queueDir.z = mIde2dfxEntry.m_mPedQueueData.m_vQueueDir.z;
				pEffect->attr.q.useDir.x = mIde2dfxEntry.m_mPedQueueData.m_vUseDir.x;
				pEffect->attr.q.useDir.y = mIde2dfxEntry.m_mPedQueueData.m_vUseDir.y;
				pEffect->attr.q.useDir.z = mIde2dfxEntry.m_mPedQueueData.m_vUseDir.z;
				pEffect->attr.q.forwardDir = pEffect->attr.q.useDir; // same in most cases, only relevant when you have angled attractors 

				switch (mIde2dfxEntry.m_mPedQueueData.m_nType) {
					case IDE2DFX_PEDQUEUE_ATM: pEffect->attr.q.type = gta::QT_ATM; break;
					case IDE2DFX_PEDQUEUE_SIT: pEffect->attr.q.type = gta::QT_SEAT; break;
					case IDE2DFX_PEDQUEUE_WAIT_BUS: pEffect->attr.q.type = gta::QT_BUS_STOP; break;
					case IDE2DFX_PEDQUEUE_SHOPPING: pEffect->attr.q.type = gta::QT_LOOK_AT; break;
					case IDE2DFX_PEDQUEUE_RAIN_SHELTER: pEffect->attr.q.type = gta::QT_SHELTER; break;
				}

				pEffect->attr.q.interest = 75;
				pEffect->attr.q.lookAt = 0;
				pEffect->attr.q.flags = 0;

				strcpy(pEffect->attr.q.scriptName, "none");
				break;

			case IDE2DFX_SUNGLARE:
				pEffect->type = gta::ET_SUNGLARE;
				break;
		}
	}

	char szDffOutputPath[MAX_PATH];
	sprintf(szDffOutputPath, "%s/%s.dff", szDffOutputFolder, szModelName);

	if (!WriteDFF(szDffOutputPath, pClump)) {
		pClump->destroy();
		return false;
	}

	printf("Added DFF 2DFX data for %s\n", szModelName);

	pClump->destroy();
	return true;
}

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_IDE_NAME,
		ARG_DFF_INPUT_FOLDER,
		ARG_DFF_OUTPUT_FOLDER
	};

	rw::version	 = 0;
	rw::platform = rw::PLATFORM_D3D9;

	rw::Engine::init();
	gta::attachPlugins();
	rw::Engine::open(NULL);
	rw::Engine::start();

	rw::Texture::setCreateDummies(1);

	if (argc < 3) {
		printf("Usage: %s <ide file> <input dff folder> <output dff folder>\n", argv[ARG_APP_NAME]);
		return 1;
	}

	IdeFile mIdeFile;
	TRY_LOG_IF_FAILED(mIdeFile.Read(argv[ARG_IDE_NAME]));

	for (auto& it : mIdeFile.m_vObjsEntries) {
		Convert2dfxDataFor(argv[ARG_DFF_INPUT_FOLDER], argv[ARG_DFF_OUTPUT_FOLDER], mIdeFile, it.m_nModelIndex, it.m_szModelName);
	}

	for (auto& it : mIdeFile.m_vTobjEntries) {
		Convert2dfxDataFor(argv[ARG_DFF_INPUT_FOLDER], argv[ARG_DFF_OUTPUT_FOLDER], mIdeFile, it.m_nModelIndex, it.m_szModelName);
	}

	for (auto& it : mIdeFile.m_vAnimEntries) {
		Convert2dfxDataFor(argv[ARG_DFF_INPUT_FOLDER], argv[ARG_DFF_OUTPUT_FOLDER], mIdeFile, it.m_nModelIndex, it.m_szModelName);
	}

	return 0;
}