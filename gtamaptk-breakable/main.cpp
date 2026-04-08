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

void AttachBreakableInfo(rw::Geometry* pGeometry, bool bAutoresolveMatID) {
	gta::Breakable* pBreakableInfo;
	rw::uint32 aBreakableHeader[13];
	rw::uint8* pPtr;
	rw::uint32 nSize;

	memset((char*)aBreakableHeader, 0, 13 * 4);
	aBreakableHeader[0] = 1;
	aBreakableHeader[1] = pGeometry->numVertices;
	aBreakableHeader[5] = pGeometry->numTriangles;
	aBreakableHeader[8] = pGeometry->matList.numMaterials;

	nSize = aBreakableHeader[1] * (12 + 8 + 4) + aBreakableHeader[5] * (6 + 2) + aBreakableHeader[8] * (32 + 32 + 12);
	pPtr = new rw::uint8[sizeof(gta::Breakable) + nSize]();

	pBreakableInfo = (gta::Breakable*)pPtr;

	pBreakableInfo->position = aBreakableHeader[0];
	pBreakableInfo->numVertices	 = aBreakableHeader[1];
	pBreakableInfo->numFaces = aBreakableHeader[5];
	pBreakableInfo->numMaterials = aBreakableHeader[8];

	*PLUGINOFFSET(gta::Breakable*, pGeometry, gta::breakableOffset) = pBreakableInfo;

	pPtr += sizeof(gta::Breakable);

	pBreakableInfo->vertices = (float*)pPtr;
	pPtr += pBreakableInfo->numVertices * 12;
	pBreakableInfo->texCoords = (float*)pPtr;
	pPtr += pBreakableInfo->numVertices * 8;
	pBreakableInfo->colors = (rw::uint8*)pPtr;
	pPtr += pBreakableInfo->numVertices * 4;
	pBreakableInfo->faces = (rw::uint16*)pPtr;
	pPtr += pBreakableInfo->numFaces * 6;
	pBreakableInfo->matIDs = (rw::uint16*)pPtr;
	pPtr += pBreakableInfo->numFaces * 2;
	pBreakableInfo->texNames = (char(*)[32])pPtr;
	pPtr += pBreakableInfo->numMaterials * 32;
	pBreakableInfo->maskNames = (char(*)[32])pPtr;
	pPtr += pBreakableInfo->numMaterials * 32;
	pBreakableInfo->surfaceProps = (rw::float32(*)[3])pPtr;

	for (rw::uint32 i = 0; i < pBreakableInfo->numVertices; i++) {
		pBreakableInfo->vertices[i * 3 + 0] = pGeometry->morphTargets[0].vertices[i].x;
		pBreakableInfo->vertices[i * 3 + 1] = pGeometry->morphTargets[0].vertices[i].y;
		pBreakableInfo->vertices[i * 3 + 2] = pGeometry->morphTargets[0].vertices[i].z;

		if (pGeometry->flags & rw::Geometry::TEXTURED) {
			pBreakableInfo->texCoords[i * 2 + 0] = pGeometry->texCoords[0][i].u;
			pBreakableInfo->texCoords[i * 2 + 1] = pGeometry->texCoords[0][i].v;
		} else {
			pBreakableInfo->texCoords[i * 2 + 0] = 0.0f;
			pBreakableInfo->texCoords[i * 2 + 1] = 0.0f;
		}

		if (pGeometry->flags & rw::Geometry::PRELIT) {
			pBreakableInfo->colors[i * 4 + 0] = pGeometry->colors[i].red;
			pBreakableInfo->colors[i * 4 + 1] = pGeometry->colors[i].green;
			pBreakableInfo->colors[i * 4 + 2] = pGeometry->colors[i].blue;
			pBreakableInfo->colors[i * 4 + 3] = pGeometry->colors[i].alpha;
		} else {
			pBreakableInfo->colors[i * 4 + 0] = 0;
			pBreakableInfo->colors[i * 4 + 1] = 0;
			pBreakableInfo->colors[i * 4 + 2] = 0;
			pBreakableInfo->colors[i * 4 + 3] = 255;
		}
	}

	for (rw::uint32 i = 0; i < pBreakableInfo->numFaces; i++) {
		pBreakableInfo->faces[i * 3 + 0] = pGeometry->triangles[i].v[0];
		pBreakableInfo->faces[i * 3 + 1] = pGeometry->triangles[i].v[1];
		pBreakableInfo->faces[i * 3 + 2] = pGeometry->triangles[i].v[2];
		pBreakableInfo->matIDs[i] = pGeometry->triangles[i].matId;

		// Find mat ID from mesh data manually
		// Needed because Kams does not write out the mat id to the triangle data at all, so it has to be resolved based on the mesh data
		if (bAutoresolveMatID) {
			rw::uint16 nFoundMatID = 0;

			rw::Mesh* pMeshes = pGeometry->meshHeader->getMeshes();
			for (rw::uint32 j = 0; j < pGeometry->meshHeader->numMeshes; j++) {
				rw::uint32 nFoundMeshGroup;

				nFoundMeshGroup = -1;
				for (rw::uint32 k = 0; k < pMeshes[j].numIndices; k++) {
					if (pMeshes[j].indices[k] == i) {
						nFoundMeshGroup = k;
						break;
					}
				}


				if (nFoundMeshGroup != -1) {
					nFoundMatID = rw::findPointer(pMeshes[j].material, (void**)pGeometry->matList.materials, pGeometry->matList.numMaterials);
					break;
				}
			}

			pBreakableInfo->matIDs[i] = nFoundMatID;
		}
	}

	for (rw::uint32 i = 0; i < pBreakableInfo->numMaterials; i++) {
		if (pGeometry->matList.materials[i]->texture != nullptr) {
			strcpy(pBreakableInfo->texNames[i], pGeometry->matList.materials[i]->texture->name);
			strcpy(pBreakableInfo->maskNames[i], pGeometry->matList.materials[i]->texture->mask);
		}

		pBreakableInfo->surfaceProps[i][0] = pGeometry->matList.materials[i]->color.red / 255.0f;
		pBreakableInfo->surfaceProps[i][1] = pGeometry->matList.materials[i]->color.green / 255.0f;
		pBreakableInfo->surfaceProps[i][2] = pGeometry->matList.materials[i]->color.blue / 255.0f;
	}
}

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_MODE,
		ARG_DFF_FILENAME
	};

	rw::version	 = 0;
	rw::platform = rw::PLATFORM_D3D9;

	rw::Engine::init();
	gta::attachPlugins();
	rw::Engine::open(NULL);
	rw::Engine::start();

	rw::Texture::setCreateDummies(1);

	if (argc < 1) {
		printf("Usage: %s [-a] [-m] <input dff filename>\n", argv[ARG_APP_NAME]);
		return 1;
	}

	rw::Clump* pClump = ReadDFF(argv[ARG_DFF_FILENAME]);
	if (pClump == nullptr || pClump->countAtomics() == 0) return 1;

	rw::Geometry* pGeometry = nullptr; // main geometry (first encountered is expected to receive the 2dfx data)
	FORLIST(pLink, pClump->atomics) {
		rw::Atomic* pAtomic = rw::Atomic::fromClump(pLink);
		rw::Geometry* _pGeometry = pAtomic->geometry;

		if (_pGeometry == nullptr) continue;
		pGeometry = _pGeometry;
		break;
	}

	if (pGeometry == nullptr) {
		printf("%s has no usable geometry\n", argv[ARG_DFF_FILENAME]);
		return 1;
	}

	// Delete any existing breakable section data
	gta::Breakable* pBreakableInfo = *PLUGINOFFSET(gta::Breakable*, pGeometry, gta::breakableOffset);
	if (pBreakableInfo != nullptr) {
		*PLUGINOFFSET(gta::Breakable*, pGeometry, gta::breakableOffset) = nullptr;
		delete[] pBreakableInfo;
	}

	if (!strcmp(argv[ARG_MODE], "-a")) {
		AttachBreakableInfo(pGeometry, true);
	} else {
		AttachBreakableInfo(pGeometry, false);
	}

	WriteDFF(argv[ARG_DFF_FILENAME], pClump);

	return 0;
}