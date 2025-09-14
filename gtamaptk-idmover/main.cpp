#include <gtamaptk-core.h>

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

bool ValidateNumber(const char* szValue) {
	if (!*szValue || *szValue == '-' || *szValue == '\0') return false; // empty strings / negative numbers are invalid

	// check if the string contains only digits
	while (*szValue) {
		if (!isdigit(*szValue)) return false;
		else ++szValue;
	}

	return true;
}

int main(int argc, char* argv[]) {
	WIN32_FIND_DATAA mFindFileData;
	std::vector<IdeFile> vIdeFiles;
	std::vector<IplFile> vIplFiles;
	int32_t nNewModelIndex;
	HANDLE hResult;

	enum {
		ARG_APP_NAME = 0,
		ARG_START_ID,
		ARG_INPUT_FOLDER
	};

	if (argc < 2) {
		printf("Usage: %s [start ID]\n", argv[ARG_APP_NAME]);
		return 1;
	}

	if (!ValidateNumber(argv[ARG_START_ID])) {
		printf("Error: Invalid numeric argument(s). All numeric arguments must be positive integers.\n");
		return 1;
	}

	CreateDirectoryA("input", nullptr);
	CreateDirectoryA("output", nullptr);

	hResult = FindFirstFileA("input\\*.ide", &mFindFileData);
	if (hResult == INVALID_HANDLE_VALUE) {
		printf("Error: No .ide files found in input folder.\n");
		return 1;
	}

	// Remap IDE IDs
	nNewModelIndex = atoi(argv[ARG_START_ID]);

	do {
		char szInputPath[MAX_PATH];
		char szOutputPath[MAX_PATH];
		IdeFile mIdeFile;

		printf("Found IDE file: %s\n", mFindFileData.cFileName);

		sprintf(szInputPath, "input/%s", mFindFileData.cFileName);
		TRY_LOG_IF_FAILED(mIdeFile.Read(szInputPath));

		for (auto& it : mIdeFile.m_vObjsEntries) {
			printf("Changing %s model index from %d to %d\n", it.m_szModelName, it.m_nModelIndex, nNewModelIndex);
			it.m_nModelIndex = nNewModelIndex++;
		}

		for (auto& it : mIdeFile.m_vTobjEntries) {
			printf("Changing %s model index from %d to %d\n", it.m_szModelName, it.m_nModelIndex, nNewModelIndex);
			it.m_nModelIndex = nNewModelIndex++;
		}

		for (auto& it : mIdeFile.m_vAnimEntries) {
			printf("Changing %s model index from %d to %d\n", it.m_szModelName, it.m_nModelIndex, nNewModelIndex);
			it.m_nModelIndex = nNewModelIndex++;
		}

		sprintf(szOutputPath, "output/%s", mFindFileData.cFileName);
		TRY_LOG_IF_FAILED(mIdeFile.Write(szOutputPath));

		vIdeFiles.push_back(mIdeFile);

	} while (FindNextFileA(hResult, &mFindFileData) != 0);

	FindClose(hResult);

	// Remap IPL IDs
	hResult = FindFirstFileA("input\\*.ipl", &mFindFileData);
	if (hResult == INVALID_HANDLE_VALUE) return 0; // No IPL files found, just exit

	do {
		char szInputPath[MAX_PATH];
		char szOutputPath[MAX_PATH];
		IplFile mIplFile;

		printf("Found IPL file: %s\n", mFindFileData.cFileName);

		sprintf(szInputPath, "input/%s", mFindFileData.cFileName);
		TRY_LOG_IF_FAILED(mIplFile.Read(szInputPath));

		for (auto& it : mIplFile.m_vInstEntries) {
			bool bFound = false;
			for (auto& jt : vIdeFiles) {
				for (auto& kt : jt.m_vObjsEntries) {
					if (!strcmp(kt.m_szModelName, it.m_szModelName)) {
						printf("Changing %s model index from %d to %d\n", kt.m_szModelName, it.m_nModelIndex, kt.m_nModelIndex);
						it.m_nModelIndex = kt.m_nModelIndex;

						bFound = true;
						break;
					}
				}

				if (bFound) break;
				for (auto& kt : jt.m_vTobjEntries) {
					if (!strcmp(kt.m_szModelName, it.m_szModelName)) {
						printf("Changing %s model index from %d to %d\n", kt.m_szModelName, it.m_nModelIndex, kt.m_nModelIndex);
						it.m_nModelIndex = kt.m_nModelIndex;

						bFound = true;
						break;
					}
				}

				if (bFound) break;
				for (auto& kt : jt.m_vAnimEntries) {
					if (!strcmp(kt.m_szModelName, it.m_szModelName)) {
						printf("Changing %s model index from %d to %d\n", kt.m_szModelName, it.m_nModelIndex, kt.m_nModelIndex);
						it.m_nModelIndex = kt.m_nModelIndex;

						bFound = true;
						break;
					}
				}

				if (bFound) break;
			}

			if (!bFound) {
				printf("Warning: Could not find IDE entry for model index %d. Leaving unchanged.\n", it.m_nModelIndex);
			}
		}

		sprintf(szOutputPath, "output/%s", mFindFileData.cFileName);
		TRY_LOG_IF_FAILED(mIplFile.Write(szOutputPath));
	} while (FindNextFileA(hResult, &mFindFileData) != 0);

	FindClose(hResult);

	return 0;
}