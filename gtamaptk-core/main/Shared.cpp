#include "Shared.hpp"
#include <stdlib.h>
#include <cstdarg>
#include <stdint.h>

char gErrorMsg[512];

char* ReadLine(FILE* pFile) {
	static char gLineBuffer[1024];

	if (fgets(gLineBuffer, sizeof(gLineBuffer), pFile) == nullptr)
		return nullptr;

	for (int32_t i = 0; gLineBuffer[i] != '\0'; i++) {
		if (gLineBuffer[i] < ' ' || gLineBuffer[i] == ',')
			gLineBuffer[i] = ' ';
	}

	char* szLine;
	for (szLine = gLineBuffer; *szLine <= ' ' && *szLine != '\0'; szLine++);

	return szLine;
}

void SetLastErrorMsg(const char* szMessage, ...) {
	va_list vl;

	va_start(vl, szMessage);
	_vsnprintf_s(gErrorMsg, _countof(gErrorMsg), _countof(gErrorMsg) - 1, szMessage, vl);
	va_end(vl);
}

const char* GetLastErrorMsg() {
	return gErrorMsg;
}