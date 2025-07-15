#include <gtamaptk-core.h>

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

void ResolveModelNames(IplFile& mIplFile, GameMap& mGameMap) {
	for (auto& it : mIplFile.m_vInstEntries) {
		if (mGameMap.IsObjsEntry(it.m_nModelIndex)) {
			IdeObjsEntry& mEntry = mGameMap.GetObjsEntry(it.m_nModelIndex);

			printf("Resolved ID %d to %s\n", it.m_nModelIndex, mEntry.m_szModelName);
			strcpy(it.m_szModelName, mEntry.m_szModelName);
		} else if (mGameMap.IsTobjEntry(it.m_nModelIndex)) {
			IdeTobjEntry& mEntry = mGameMap.GetTobjEntry(it.m_nModelIndex);

			printf("Resolved ID %d to %s\n", it.m_nModelIndex, mEntry.m_szModelName);
			strcpy(it.m_szModelName, mEntry.m_szModelName);
		} else if (mGameMap.IsAnimEntry(it.m_nModelIndex)) {
			IdeAnimEntry& mEntry = mGameMap.GetAnimEntry(it.m_nModelIndex);

			printf("Resolved ID %d to %s\n", it.m_nModelIndex, mEntry.m_szModelName);
			strcpy(it.m_szModelName, mEntry.m_szModelName);
		} else {
			printf("Error: Unable to find model name for ID %d\n", it.m_nModelIndex);
			continue;
		}
	}
}

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_OPTION,
		ARG_INPUT_FILE,
		ARG_OUTPUT_FILE,
		ARG_GAME_ROOT // only used for binary IPLs
	};

	if (argc < 2) {
		printf("Usage: %s [-c] [-d] input.ipl output.ipl [gta dir]\n", argv[ARG_APP_NAME]);
		printf("\t-c: Compiles a text IPL into a binary IPL\n");
		printf("\t-d: Decompiles a binary IPL into a text IPL, using the IDE files in gta.dat to fill out the model names\n");
		return 1;
	}

	if (strcmp(argv[ARG_OPTION], "-c") == 0) {
		if (argc < 3) {
			printf("Error: Not enough arguments for compile mode.\n");
			return 1;
		}

		IplFile mInputFile;
		TRY_LOG_IF_FAILED(mInputFile.Read(argv[ARG_INPUT_FILE]));
		TRY_LOG_IF_FAILED(mInputFile.WriteBinary(argv[ARG_OUTPUT_FILE]));
	} else if (strcmp(argv[ARG_OPTION], "-d") == 0) {
		if (argc < 4) {
			printf("Error: Not enough arguments for decompile mode.\n");
			return 1;
		}

		IplFile mInputFile;
		TRY_LOG_IF_FAILED(mInputFile.ReadBinary(argv[ARG_INPUT_FILE]));

		GameMap mGameMap;
		mGameMap.Load(argv[ARG_GAME_ROOT], true, false);

		ResolveModelNames(mInputFile, mGameMap);
		
		TRY_LOG_IF_FAILED(mInputFile.Write(argv[ARG_OUTPUT_FILE]));
	} else {
		printf("Error: Unknown option '%s'. Use -c to compile or -d to decompile.\n", argv[ARG_OPTION]);
		return 1;
	}
}