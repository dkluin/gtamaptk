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
	enum {
		ARG_APP_NAME = 0,
		ARG_OPTION,
		ARG_INPUT_FILE,
		ARG_OUTPUT_FILE,
		ARG_NUM_CULL_ZONES_IN_BLOCK,
		ARG_NUM_ATTR_ZONES_IN_BLOCK,
		ARG_NUM_ZONE_INDICES,
		ARG_NUM_BUILDINGS,
		ARG_NUM_TREADABLES
	};

	if (argc < 8) {
		printf("Usage: %s [-c] [-d] [input CULLZONE.DAT] [output CULLZONE.DAT] [number of cull zones written to block] [number of attr zones written to block] [number of zone indices] [number of buildings] [number of treadables]\n", argv[ARG_APP_NAME]);
		printf("\t-c: Compiles a new CULLZONE.DAT file\n");
		printf("\t-d: Decompiles a CULLZONE.DAT file\n");
		return 1;
	}

	if (!ValidateNumber(argv[ARG_NUM_CULL_ZONES_IN_BLOCK])
		|| !ValidateNumber(argv[ARG_NUM_ATTR_ZONES_IN_BLOCK])
		|| !ValidateNumber(argv[ARG_NUM_ZONE_INDICES])
		|| !ValidateNumber(argv[ARG_NUM_BUILDINGS])
		|| !ValidateNumber(argv[ARG_NUM_TREADABLES])) {
		printf("Error: Invalid numeric argument(s). All numeric arguments must be positive integers.\n");
		return 1;
	}

	if (strcmp(argv[ARG_OPTION], "-c") == 0) {
		if (argc < 3) {
			printf("Error: Not enough arguments for compile mode.\n");
			return 1;
		}

		CullZoneFile mCullZoneFile(
			atoi(argv[ARG_NUM_CULL_ZONES_IN_BLOCK]),
			atoi(argv[ARG_NUM_ATTR_ZONES_IN_BLOCK]),
			atoi(argv[ARG_NUM_ZONE_INDICES]),
			atoi(argv[ARG_NUM_BUILDINGS]),
			atoi(argv[ARG_NUM_TREADABLES]));

		TRY_LOG_IF_FAILED(mCullZoneFile.Read(argv[ARG_INPUT_FILE]));
		TRY_LOG_IF_FAILED(mCullZoneFile.WriteBinary(argv[ARG_OUTPUT_FILE]));
	} else if (strcmp(argv[ARG_OPTION], "-d") == 0) {
		if (argc < 4) {
			printf("Error: Not enough arguments for decompile mode.\n");
			return 1;
		}

		CullZoneFile mCullZoneFile(
			atoi(argv[ARG_NUM_CULL_ZONES_IN_BLOCK]),
			atoi(argv[ARG_NUM_ATTR_ZONES_IN_BLOCK]),
			atoi(argv[ARG_NUM_ZONE_INDICES]),
			atoi(argv[ARG_NUM_BUILDINGS]),
			atoi(argv[ARG_NUM_TREADABLES]));

		TRY_LOG_IF_FAILED(mCullZoneFile.ReadBinary(argv[ARG_INPUT_FILE]));
		TRY_LOG_IF_FAILED(mCullZoneFile.Write(argv[ARG_OUTPUT_FILE]));
	} else {
		printf("Error: Unknown option '%s'. Use -c to compile or -d to decompile.\n", argv[ARG_OPTION]);
		return 1;
	}

	return 0;
}