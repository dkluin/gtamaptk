#include <gtamaptk-core.h>
#include <iostream>
#include <Windows.h>

#define MAX_COLMODELNAME_LEN 24
#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

// Very primitive reading/writing code... I don't need to do anything specific just dump out the contents as they are so no need for fancy reading/parsing logic
// It's really heavy allocation-wise, in GTA code Obbe uses a nice trick to read the file in one operation and resolve the refs afterwards (refs being ptrs to the actual col data like triangles etc)
// but we're skipping that for now!
struct ColFileHeader {
	uint32_t m_nVersionCC;
	uint32_t m_nModelSize;
	char m_szName[MAX_COLMODELNAME_LEN];
};

struct ColHeap {
	uint32_t m_nVersionCC;

	char m_szName[MAX_COLMODELNAME_LEN];
	uint8_t* m_pHeap;
	uint32_t m_nSize;
};

class CollisionFile {
public:
	std::vector<ColHeap> m_vModels;

public:
	bool Read(const char* szFileName) {
		FILE* pFile = fopen(szFileName, "rb");

		if (pFile == nullptr) {
			SetLastErrorMsg("Error: Unable to open %s", szFileName);
			return false;
		}

		int nTotalSize;
		fseek(pFile, 0, SEEK_END);
		nTotalSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		uint8_t* pRawBuffer = new uint8_t[nTotalSize];
		memset(pRawBuffer, 0, nTotalSize);

		fread(pRawBuffer, 1, nTotalSize, pFile);
		fclose(pFile);

		uint32_t nOffset = 0;
		while (nOffset < nTotalSize) {
			ColFileHeader* pCurrFileHeader;
			ColHeap mColHeap;
			
			pCurrFileHeader = (ColFileHeader*)(pRawBuffer + nOffset);

			// If we reached the end of the file
			if (pCurrFileHeader->m_nVersionCC != 0x4C4C4F43 && pCurrFileHeader->m_nVersionCC != 0x324C4F43 && pCurrFileHeader->m_nVersionCC != 0x334C4F43 && pCurrFileHeader->m_nVersionCC != 0x344C4F43) {
				return true;
			}

			mColHeap.m_nVersionCC = pCurrFileHeader->m_nVersionCC;
			strcpy(mColHeap.m_szName, pCurrFileHeader->m_szName);
			mColHeap.m_szName[MAX_COLMODELNAME_LEN - 1] = '\0';
			
			mColHeap.m_pHeap = new uint8_t[pCurrFileHeader->m_nModelSize];
			memcpy(mColHeap.m_pHeap, (uint8_t*)(pRawBuffer + nOffset + 8), pCurrFileHeader->m_nModelSize);
			mColHeap.m_nSize = pCurrFileHeader->m_nModelSize;

			m_vModels.push_back(mColHeap);	
			nOffset += pCurrFileHeader->m_nModelSize + 8;
		}

		delete[] pRawBuffer;
		fclose(pFile);

		return true;
	}

	bool Write(const char* szFileName) {
		FILE* pFile = fopen(szFileName, "wb");

		if (pFile == nullptr) {
			SetLastErrorMsg("Error: Unable to open %s", szFileName);
			return false;
		}

		for (auto& it : m_vModels) {
			fwrite(&it.m_nVersionCC, 1, sizeof(it.m_nVersionCC), pFile);
			fwrite(&it.m_nSize, 1, sizeof(it.m_nSize), pFile);
			fwrite(it.m_pHeap, 1, it.m_nSize, pFile);
		}

		fclose(pFile);
		return true;
	}

	const ColHeap* FindColHeapByName(const char* szModelName) {
		for (auto& it : m_vModels) {
			if (!strcmp(it.m_szName, szModelName)) {
				return &it;
			}
		}

		return nullptr;
	}

	// Adds a heap from another col heap 
	// Deliberately ugly to ensure this collision file instance retains ownership of its heap data
	void AddColHeap(const ColHeap* pOtherHeap) {
		ColHeap mNewHeap;

		mNewHeap.m_nVersionCC = pOtherHeap->m_nVersionCC;

		strcpy(mNewHeap.m_szName, pOtherHeap->m_szName);
		mNewHeap.m_szName[MAX_COLMODELNAME_LEN - 1] = '\0';

		mNewHeap.m_pHeap = new uint8_t[pOtherHeap->m_nSize];
		memcpy(mNewHeap.m_pHeap, pOtherHeap->m_pHeap, pOtherHeap->m_nSize);
		mNewHeap.m_nSize = pOtherHeap->m_nSize;

		m_vModels.push_back(mNewHeap);
	}
};

bool IsModelInIDE(IdeFile& mIdeFile, int32_t nModelIndex) {
	for (auto& it : mIdeFile.m_vObjsEntries) {
		if (it.m_nModelIndex == nModelIndex)
			return true;
	}

	for (auto& it : mIdeFile.m_vTobjEntries) {
		if (it.m_nModelIndex == nModelIndex)
			return true;
	}

	for (auto& it : mIdeFile.m_vAnimEntries) {
		if (it.m_nModelIndex == nModelIndex)
			return true;
	}

	return false;
}

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_OPTION,
		ARG_INPUT_COL
	};

	if (argc < 2) {
		printf("Usage: %s [-ide] [-ipl] input.col output.col input.ide input.ipl\n", argv[ARG_APP_NAME]);
		printf("\t-ide: Split off a COL archive based on an IDE file\n");
		printf("\t-ipl: Split off a COL archive based on an IPL file\n");
		return 1;
	}

	if (strcmp(argv[ARG_OPTION], "-ide") == 0) {
		enum {
			ARG_OUTPUT_COL = ARG_INPUT_COL + 1,
			ARG_INPUT_IDE
		};

		if (argc < 3) {
			printf("Error: Not enough arguments for IDE mode.\n");
			return 1;
		}

		CollisionFile mInputCol;
		TRY_LOG_IF_FAILED(mInputCol.Read(argv[ARG_INPUT_COL]));

		IdeFile mIdeFile;
		TRY_LOG_IF_FAILED(mIdeFile.Read(argv[ARG_INPUT_IDE]));

		CollisionFile mOutputCol;
		for (auto& it : mIdeFile.m_vObjsEntries) {
			const ColHeap* pFoundColHeap = mInputCol.FindColHeapByName(it.m_szModelName);

			if (pFoundColHeap != nullptr) {
				printf("Adding %s to output COL\n", pFoundColHeap->m_szName);

				mOutputCol.AddColHeap(pFoundColHeap);
			}
		}

		for (auto& it : mIdeFile.m_vTobjEntries) {
			const ColHeap* pFoundColHeap = mInputCol.FindColHeapByName(it.m_szModelName);

			if (pFoundColHeap != nullptr) {
				printf("Adding %s to output COL\n", pFoundColHeap->m_szName);

				mOutputCol.AddColHeap(pFoundColHeap);
			}
		}

		for (auto& it : mIdeFile.m_vAnimEntries) {
			const ColHeap* pFoundColHeap = mInputCol.FindColHeapByName(it.m_szModelName);

			if (pFoundColHeap != nullptr) {
				printf("Adding %s to output COL\n", pFoundColHeap->m_szName);

				mOutputCol.AddColHeap(pFoundColHeap);
			}
		}

		mOutputCol.Write(argv[ARG_OUTPUT_COL]);
	} else if (strcmp(argv[ARG_OPTION], "-ipl") == 0) {
		enum {
			ARG_INPUT_IDE = ARG_INPUT_COL + 1,
			ARG_INPUT_IPL_START
		};

		if (argc < 3) {
			printf("Error: Not enough arguments for IPL mode.\n");
			return 1;
		}

		CollisionFile mInputCol;
		TRY_LOG_IF_FAILED(mInputCol.Read(argv[ARG_INPUT_COL]));

		IdeFile mIdeFile;
		TRY_LOG_IF_FAILED(mIdeFile.Read(argv[ARG_INPUT_IDE]));

		std::vector<int32_t> vModelsAlreadyWritten;
		for (int i = ARG_INPUT_IPL_START; i < argc; i++) {
			IplFile mIplFile;
			TRY_LOG_IF_FAILED(mIplFile.Read(argv[i]));

			char szTmp[MAX_PATH];
			strcpy(szTmp, argv[ARG_INPUT_COL]);
			szTmp[strlen(argv[ARG_INPUT_COL]) - 4] = '\0';

			char szOutputColName[MAX_PATH];
			sprintf(szOutputColName, "%s_%d.col", szTmp, i - ARG_INPUT_IPL_START + 1);

			CollisionFile mOutputCol;
			for (auto& it: mIplFile.m_vInstEntries) {
				if (!IsModelInIDE(mIdeFile, it.m_nModelIndex)) continue;// only move models actually present in this IDE file; everything else can be skipped thanks to the xref workflow

				const ColHeap* pFoundColHeap = mInputCol.FindColHeapByName(it.m_szModelName);

				bool bModelAlreadyAdded = std::find(vModelsAlreadyWritten.begin(), vModelsAlreadyWritten.end(), it.m_nModelIndex) != vModelsAlreadyWritten.end();
				if (pFoundColHeap != nullptr && !bModelAlreadyAdded) {
					printf("Adding %s to output COL\n", pFoundColHeap->m_szName);

					mOutputCol.AddColHeap(pFoundColHeap);
					vModelsAlreadyWritten.push_back(it.m_nModelIndex);
				}
			}

			mOutputCol.Write(szOutputColName);
		}
	} else {
		printf("Error: Unknown option '%s'. Use -ide or -ipl.\n", argv[ARG_OPTION]);
		return 1;
	}

	return 0;
}