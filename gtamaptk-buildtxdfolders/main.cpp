#include <gtamaptk-core.h>
#include <iostream>
#include <rw.h>
#include <Windows.h>
#include "librwgta/rwgta.h"

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

const char* GetFileNameWithoutExtension(const char* szFileName) {
	static char gszTemp[MAX_PATH];

	size_t nLastIndex = std::string(szFileName).find_last_of(".");
	strcpy(gszTemp, std::string(szFileName).substr(0, nLastIndex).c_str());

	return gszTemp;
}

const char* GetBaseName(const char* szFilePath) {
	static char gszTemp[MAX_PATH];

	std::string sStr = std::string(szFilePath);
	strcpy(gszTemp, sStr.substr(sStr.find_last_of("/\\") + 1).c_str());

	return gszTemp;
}

bool FileExists(const char* szFile) {
	FILE* pFile = fopen(szFile, "rb");

	if (pFile != nullptr) {
		fclose(pFile);
		return true;
	}

	return false;
}

const char* FindTextureFileName(const char* szSourceFolder, const char* szMaterialTexName) {
	static char gszTempBuffer[MAX_PATH];

	sprintf(gszTempBuffer, "%s\\%s.jpg", szSourceFolder, szMaterialTexName);
	if (FileExists(gszTempBuffer)) {
		memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
		sprintf(gszTempBuffer, "%s.jpg", szMaterialTexName);

		return gszTempBuffer;
	}

	memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
	sprintf(gszTempBuffer, "%s\\%s.jpeg", szSourceFolder, szMaterialTexName);
	if (FileExists(gszTempBuffer)) {
		memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
		sprintf(gszTempBuffer, "%s.jpeg", szMaterialTexName);

		return gszTempBuffer;
	}

	memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
	sprintf(gszTempBuffer, "%s\\%s.bmp", szSourceFolder, szMaterialTexName);
	if (FileExists(gszTempBuffer)) {
		memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
		sprintf(gszTempBuffer, "%s.bmp", szMaterialTexName);

		return gszTempBuffer;
	}

	memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
	sprintf(gszTempBuffer, "%s\\%s.png", szSourceFolder, szMaterialTexName);
	if (FileExists(gszTempBuffer)) {
		memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
		sprintf(gszTempBuffer, "%s.png", szMaterialTexName);

		return gszTempBuffer;
	}

	memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
	sprintf(gszTempBuffer, "%s\\%s.rwtex", szSourceFolder, szMaterialTexName);
	if (FileExists(gszTempBuffer)) {
		memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
		sprintf(gszTempBuffer, "%s.rwtex", szMaterialTexName);

		return gszTempBuffer;
	}

	memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
	sprintf(gszTempBuffer, "%s\\%s.dds", szSourceFolder, szMaterialTexName);
	if (FileExists(gszTempBuffer)) {
		memset(gszTempBuffer, NULL, sizeof(gszTempBuffer));
		sprintf(gszTempBuffer, "%s.dds", szMaterialTexName);

		return gszTempBuffer;
	}

	return nullptr;
}

const char* FindTxdName(IdeFile& mIdeFile, const char* szModelName) {
	for (auto& it : mIdeFile.m_vObjsEntries) {
		if (!strcmp(it.m_szModelName, szModelName)) {
			return it.m_szTxdName;
		}
	}

	for (auto& it : mIdeFile.m_vTobjEntries) {
		if (!strcmp(it.m_szModelName, szModelName)) {
			return it.m_szTxdName;
		}
	}

	for (auto& it : mIdeFile.m_vAnimEntries) {
		if (!strcmp(it.m_szModelName, szModelName)) {
			return it.m_szTxdName;
		}
	}

	return nullptr;
}

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_DFF_NAME,
		ARG_IDE_NAME,
		ARG_TEXTURE_SOURCE_FOLDER,
		ARG_DEST_TEXTURE_FOLDER_ROOT
	};

	rw::version = 0;
	rw::platform = rw::PLATFORM_D3D9;

	rw::Clump* pClump;
	rw::ChunkHeaderInfo mHeader;
	rw::StreamFile mStream;

	rw::Engine::init();
	gta::attachPlugins();
	rw::Engine::open(NULL);
	rw::Engine::start();

	rw::Texture::setCreateDummies(1);

	if (argc < 4) {
		printf("Usage: %s <ide file> <dff file> <texture source folder> <folder in which to populate texture folders>\n", argv[ARG_APP_NAME]);
		return 1;
	}

	IdeFile mIdeFile;
	TRY_LOG_IF_FAILED(mIdeFile.Read(argv[ARG_IDE_NAME]));

	char szFileNameWithoutExt[MAX_PATH];
	strcpy(szFileNameWithoutExt, GetFileNameWithoutExtension(GetBaseName(argv[ARG_DFF_NAME])));

	const char* szTxdName = FindTxdName(mIdeFile, szFileNameWithoutExt);
	if (szTxdName == nullptr) {
		printf("Error: Model %s not found in IDE\n", szFileNameWithoutExt);
		return 1;
	}

	if (!mStream.open(argv[ARG_DFF_NAME], "rb")) {
		printf("Error: couldn't open %s\n", argv[ARG_DFF_NAME]);
		return 1;
	}

	readChunkHeaderInfo(&mStream, &mHeader);
	rw::version = mHeader.version;
	rw::build = mHeader.build;

	if (mHeader.type != rw::ID_CLUMP) {
		printf("Error: First section in %s must be ID_CLUMP\n", argv[ARG_DFF_NAME]);

		mStream.close();
		return 1;
	}

	pClump = rw::Clump::streamRead(&mStream);
	mStream.close();

	if (pClump == nullptr) {
		printf("Error: Couldn't read clump\n");
		return 1;
	}

	FORLIST(pLink, pClump->atomics) {
		rw::Atomic* pAtomic = rw::Atomic::fromClump(pLink);
		rw::Geometry* pGeometry = pAtomic->geometry;

		for (int i = 0; i < pGeometry->matList.numMaterials; i++) {
			rw::Material* pMaterial = pGeometry->matList.materials[i];

			if (pMaterial->texture != nullptr) {
				const char* szFoundTextureFile = FindTextureFileName(argv[ARG_TEXTURE_SOURCE_FOLDER], pMaterial->texture->name);

				if (szFoundTextureFile == nullptr) {
					printf("Error: Unable to find texture %s in texture dir\n", pMaterial->texture->name);
					continue;
				}

				char szSourcePath[MAX_PATH];
				sprintf(szSourcePath, "%s\\%s", argv[ARG_TEXTURE_SOURCE_FOLDER], szFoundTextureFile);

				CreateDirectoryA(std::string(std::string(argv[ARG_DEST_TEXTURE_FOLDER_ROOT]) + "\\" + std::string(szTxdName)).c_str(), nullptr);

				char szDestPath[MAX_PATH];
				sprintf(szDestPath, "%s\\%s\\%s", argv[ARG_DEST_TEXTURE_FOLDER_ROOT], szTxdName, szFoundTextureFile);

				CopyFileA(szSourcePath, szDestPath, FALSE);
			}
			else {
				printf("Error: Model %s has an improper material - this model will have missing texture in-game\n", argv[ARG_DFF_NAME]);
			}
		}
	}
	return 0;
}