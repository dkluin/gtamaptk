#include <gtamaptk-core.h>
#include <iostream>
#include <rw.h>
#include <Windows.h>
#include "librwgta/rwgta.h"

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

std::map<std::string, rw::Clump*> gmDffInputFiles_1;
std::map<std::string, rw::Clump*> gmDffInputFiles_2;
std::map<std::string, rw::Clump*> gmDffOutputFiles;

bool PointOverlapsWithinThreshold(const rw::V3d& a, const rw::V3d& b, float fThreshold) {
	bool bOverlaps = true;

	if (fabs(a.x - b.x) > fThreshold) bOverlaps = false;
	if (fabs(a.y - b.y) > fThreshold) bOverlaps = false;
	if (fabs(a.z - b.z) > fThreshold) bOverlaps = false;
	return bOverlaps;
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

	rw::version = 0x36003;
	pClump->streamWrite(&mStream);
	mStream.close();
	return true;
}

bool ReadDFFs(const char* szInputFolder, std::map<std::string, rw::Clump*>& mInputMap) {
	char szDffInputPath[MAX_PATH];
	sprintf(szDffInputPath, "%s/*.dff", szInputFolder);

	WIN32_FIND_DATA mFile;
	HANDLE hFind = NULL;
	if ((hFind = FindFirstFile(szDffInputPath, &mFile)) == INVALID_HANDLE_VALUE) {
		printf("dffin folder does not exist\n");
		return false;
	}
	
	do {
		char szDffPath[MAX_PATH];
		sprintf(szDffPath, "%s/%s", szInputFolder, mFile.cFileName);

		rw::Clump* pClump = ReadDFF(szDffPath);
		if (pClump == nullptr || pClump->countAtomics() == 0) continue;

		mInputMap.emplace(std::string(mFile.cFileName), pClump);
	} while (FindNextFile(hFind, &mFile));

	FindClose(hFind);
	return true;
}

rw::Geometry* FindFirstGeometry(rw::Clump* pClump) {
	if (pClump->countAtomics() == 0) return nullptr;

	FORLIST(pLink, pClump->atomics) {
		rw::Atomic* pAtomic	= rw::Atomic::fromClump(pLink);
		rw::Geometry* pGeometry = pAtomic->geometry;

		if (pGeometry == nullptr) continue;
		return pGeometry;
	}
	
	return nullptr;
}

// Check if the input vector already contains a 2dfx entry at the given position and type
bool DoWeHaveThisEntryAlready(std::vector<gta::Effect2d> v2dfxEntries, gta::Effect2d* pOtherEntry, float fThreshold) {
	for (auto it : v2dfxEntries) {
		if (it.type == pOtherEntry->type && PointOverlapsWithinThreshold(it.posn, pOtherEntry->posn, fThreshold)) {
			return true;
		}
	}

	return false;
}

// Overwrite existing 2dfx data on 1st input clump
rw::Clump* CreateOutputClumpWithMerged2dfx(rw::Clump* pInputClump1, rw::Clump* pInputClump2, float fThreshold) {
	rw::int32 nNum2dfxEntries1, nNum2dfxEntries2;
	std::vector<gta::Effect2d> vOutput2dfx;
	rw::uint8* p2dfxBuffer1;
	rw::uint8* p2dfxBuffer2;
	rw::Clump* pOutputClump;
	rw::Geometry* pGeometry1;
	rw::Geometry* pGeometry2;
	gta::Effect2d* p2dfxEntries1;
	gta::Effect2d* p2dfxEntries2;
	rw::uint8* pOutput2dfxData;

	pGeometry1 = FindFirstGeometry(pInputClump1);
	pGeometry2 = FindFirstGeometry(pInputClump2);

	// If either has atomics but no geometry (just frames)
	if (pGeometry1 == nullptr || pGeometry2 == nullptr)
		return pInputClump1;

	pOutputClump = pInputClump1; // cloning doesn't always work for some reason. so we'll just overwrite it this way for now

	p2dfxBuffer1 = *PLUGINOFFSET(rw::uint8*, pGeometry1, gta::twodEffectOffset);
	p2dfxBuffer2 = *PLUGINOFFSET(rw::uint8*, pGeometry2, gta::twodEffectOffset);

	nNum2dfxEntries1 = 0;
	p2dfxEntries1 = nullptr;
	if (p2dfxBuffer1 != nullptr) { 
		nNum2dfxEntries1 = *(rw::int32*)p2dfxBuffer1;
		p2dfxEntries1 = (gta::Effect2d*)(p2dfxBuffer1 + 4);
	}

	nNum2dfxEntries2 = 0;
	p2dfxEntries2 = nullptr;
	if (p2dfxBuffer2 != nullptr) {
		nNum2dfxEntries2 = *(rw::int32*)p2dfxBuffer2;
		p2dfxEntries2 = (gta::Effect2d*)(p2dfxBuffer2 + 4);
	}

	for (rw::int32 i = 0; i < nNum2dfxEntries1; i++) {
		vOutput2dfx.push_back(p2dfxEntries1[i]);
	}

	for (rw::int32 i = 0; i < nNum2dfxEntries2; i++) {
		gta::Effect2d* pEffect = &p2dfxEntries2[i];

		if (!DoWeHaveThisEntryAlready(vOutput2dfx, pEffect, fThreshold)) {
			printf("Merging 2dfx entry of type %d at %f %f %f with existing 2dfx\n", pEffect->type, pEffect->posn.x, pEffect->posn.y, pEffect->posn.z);

			vOutput2dfx.push_back(p2dfxEntries2[i]);
		} else {
			printf("2dfx entry of type %d at %f %f %f already exists\n", pEffect->type, pEffect->posn.x, pEffect->posn.y, pEffect->posn.z);
		}
	}

	// Delete old 2dfx data if needed (note: sometimes 1st geom doesn't even have 2dfx)
	if (p2dfxBuffer1 != nullptr) {
		delete[] p2dfxBuffer1;
		*PLUGINOFFSET(rw::uint8*, pGeometry1, gta::twodEffectOffset) = nullptr;
	}

	// ...and re-allocate it, according to the temporary output 2dfx vector, applying it on top 
	if (vOutput2dfx.size() > 0) {
		pOutput2dfxData = new rw::uint8[vOutput2dfx.size() * sizeof(gta::Effect2d) + 4];
		*PLUGINOFFSET(rw::uint8*, pGeometry1, gta::twodEffectOffset) = pOutput2dfxData;

		*(rw::int32*)pOutput2dfxData = vOutput2dfx.size();
		memcpy(pOutput2dfxData + 4, vOutput2dfx.data(), vOutput2dfx.size() * sizeof(gta::Effect2d));
	} else {
		*PLUGINOFFSET(rw::uint8*, pGeometry1, gta::twodEffectOffset) = nullptr;
	}

	return pOutputClump;
}

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_POS_MAX_ALLOWED_DIFFERENCE, // maximum allowed difference between 2dfx positions for it to be considered
		ARG_DFF_INPUT_FOLDER_1,
		ARG_DFF_INPUT_FOLDER_2,
		ARG_DFF_OUTPUT_FOLDER
	};

	rw::version	 = 0;
	rw::platform = rw::PLATFORM_D3D9;

	rw::Engine::init();
	gta::attachPlugins();
	rw::Engine::open(NULL);
	rw::Engine::start();

	rw::Texture::setCreateDummies(1);

	if (argc < 4) {
		printf("Usage: %s <max allowed pos difference> <1st input dff folder> <2nd input dff folder> <output 2dfx folder>\n", argv[ARG_APP_NAME]);
		return 1;
	}

	if (!ReadDFFs(argv[ARG_DFF_INPUT_FOLDER_1], gmDffInputFiles_1) || !ReadDFFs(argv[ARG_DFF_INPUT_FOLDER_2], gmDffInputFiles_2)) {
		return 1;
	}

	for (auto it : gmDffInputFiles_1) {
		if (gmDffInputFiles_2.find(it.first) == gmDffInputFiles_2.end()) {
			gmDffOutputFiles.emplace(it.first, it.second);

			printf("File %s does not exist in 2nd folder, skipping...\n", it.first.c_str());
			continue;
		}

		gmDffOutputFiles.emplace(it.first, CreateOutputClumpWithMerged2dfx(it.second, gmDffInputFiles_2[it.first], std::atof(argv[ARG_POS_MAX_ALLOWED_DIFFERENCE])));
	}

	for (auto it : gmDffOutputFiles) {
		char szOutputPath[MAX_PATH];
		sprintf(szOutputPath, "%s/%s", argv[ARG_DFF_OUTPUT_FOLDER], it.first.c_str());

		printf("Outputting DFF %s\n", it.first.c_str());

		WriteDFF(szOutputPath, it.second);
	}

	return 0;
}