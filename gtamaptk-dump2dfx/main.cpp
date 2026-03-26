#include <gtamaptk-core.h>
#include <iostream>
#include <rw.h>
#include <Windows.h>
#include "librwgta/rwgta.h"

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

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

const char* GetBaseName(const char* szFilePath) {
	static char gszTemp[MAX_PATH];

	std::string sStr = std::string(szFilePath);
	strcpy(gszTemp, sStr.substr(sStr.find_last_of("/\\") + 1).c_str());

	return gszTemp;
}

const char* RemoveFileExtension(const char* szFilePath) {
	static char gszTemp[MAX_PATH];

	std::string sStr = std::string(szFilePath);
	size_t nLastIndex = sStr.find_last_of(".");
	strcpy(gszTemp, sStr.substr(0, nLastIndex).c_str());

	return gszTemp;
}

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_DFF_INPUT_FOLDER,
		ARG_2DFX_OUTPUT_FOLDER
	};

	rw::version	 = 0;
	rw::platform = rw::PLATFORM_D3D9;

	rw::Engine::init();
	gta::attachPlugins();
	rw::Engine::open(NULL);
	rw::Engine::start();

	rw::Texture::setCreateDummies(1);

	if (argc < 2) {
		printf("Usage: %s <input dff folder> <output 2dfx folder>\n", argv[ARG_APP_NAME]);
		return 1;
	}

	char szDffInputPath[MAX_PATH];
	sprintf(szDffInputPath, "%s/*.dff", argv[ARG_DFF_INPUT_FOLDER]);

	WIN32_FIND_DATA mFile;
	HANDLE hFind = NULL;
	if ((hFind = FindFirstFile(szDffInputPath, &mFile)) == INVALID_HANDLE_VALUE) {
		printf("dffin folder does not exist\n");
		return 1;
	}
	
	do {
		char szDffPath[MAX_PATH];
		sprintf(szDffPath, "%s/%s", argv[ARG_DFF_INPUT_FOLDER], mFile.cFileName);

		rw::Clump* pClump = ReadDFF(szDffPath);
		if (pClump == nullptr || pClump->countAtomics() == 0) continue;

		FORLIST(pLink, pClump->atomics) {
			char szOutputFile[MAX_PATH];
			const char* szBaseFileName;
			rw::Atomic* pAtomic;
			rw::Geometry* pGeometry;
			rw::uint8* p2dfxData;

			pAtomic	= rw::Atomic::fromClump(pLink);
			pGeometry = pAtomic->geometry;

			if (pGeometry == nullptr) continue;

			p2dfxData = *PLUGINOFFSET(rw::uint8*, pGeometry, gta::twodEffectOffset);

			if (p2dfxData != nullptr) {
				szBaseFileName = RemoveFileExtension(mFile.cFileName);
				sprintf(szOutputFile, "%s/%s.2dfx", argv[ARG_2DFX_OUTPUT_FOLDER], szBaseFileName);

				printf("Dumping 2dfx data for %s\n", mFile.cFileName);

				// Use plugin funcs to write out 2dfx data to output stream
				rw::StreamFile mStream;
				mStream.open(szOutputFile, "wb"); 

				FORLIST(lnk, rw::Geometry::s_plglist.plugins) {
					rw::Plugin* p = LLLinkGetData(lnk, rw::Plugin, inParentList);
					if (p->id != gta::ID_2DEFFECT) continue;

					rw::int32 nSize = p->getSize(pGeometry, gta::twodEffectOffset, 0);
					p->write(&mStream, nSize, pGeometry, gta::twodEffectOffset, 0);
					break;
				}

				mStream.close();
			}
		}
	} while (FindNextFile(hFind, &mFile));

	FindClose(hFind);

	return 0;
}