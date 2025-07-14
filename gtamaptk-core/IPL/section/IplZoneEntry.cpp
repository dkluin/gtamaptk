#include "IplZoneEntry.h"

bool IplZoneEntry::Read(const char* szLine) {
	return true;
}

void IplZoneEntry::ReadBinary(uint8_t* pFileContent, int32_t nOffset) {
	// not implemented
}

void IplZoneEntry::Write(FILE* pFile) {

}

void IplZoneEntry::WriteBinary(FILE* pFile) {
	// not implemented
}