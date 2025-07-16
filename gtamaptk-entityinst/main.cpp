#include <gtamaptk-core.h>
#include "inst/EntityInstFile.h"

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

int main(int argc, char* argv[]) {
	enum {
		ARG_APP_NAME = 0,
		ARG_OPTION,
		ARG_INPUT_FILE,
		ARG_OUTPUT_FILE
	};

	if (argc < 3) {
		printf("Usage: %s [-c] [-d] [input file] [output file]\n", argv[ARG_APP_NAME]);
		printf("\t-c: Compiles a new entity.inst file\n");
		printf("\t-d: Decompiles an entity.inst file\n");
		return 1;
	}

	if (strcmp(argv[ARG_OPTION], "-c") == 0) {
		if (argc < 3) {
			printf("Error: Not enough arguments for compile mode.\n");
			return 1;
		}

		EntityInstFile mInputFile;
		TRY_LOG_IF_FAILED(mInputFile.Read(argv[ARG_INPUT_FILE]));
		TRY_LOG_IF_FAILED(mInputFile.WriteBinary(argv[ARG_OUTPUT_FILE]));
	} else if (strcmp(argv[ARG_OPTION], "-d") == 0) {
		if (argc < 4) {
			printf("Error: Not enough arguments for decompile mode.\n");
			return 1;
		}

		EntityInstFile mInputFile;
		TRY_LOG_IF_FAILED(mInputFile.ReadBinary(argv[ARG_INPUT_FILE]));
		TRY_LOG_IF_FAILED(mInputFile.Write(argv[ARG_OUTPUT_FILE]));
	} else {
		printf("Error: Unknown option '%s'. Use -c to compile or -d to decompile.\n", argv[ARG_OPTION]);
		return 1;
	}

	return 0;
}