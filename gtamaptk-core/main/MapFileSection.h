#pragma once

#include <stdint.h>
#include <stdio.h>

enum eIdeSection {
	IDE_SECTION_NONE = 0,
	IDE_SECTION_OBJS,
	IDE_SECTION_TOBJ,
	IDE_SECTION_PATH,
	IDE_SECTION_2DFX,
	IDE_SECTION_ANIM,
	IDE_SECTION_TXDP
};

enum eIplSection {
	IPL_SECTION_NONE = 0,
	IPL_SECTION_AUZO,
	IPL_SECTION_CARS,
	IPL_SECTION_CULL,
	IPL_SECTION_ENEX,
	IPL_SECTION_GRGE,
	IPL_SECTION_INST,
	IPL_SECTION_JUMP,
	IPL_SECTION_OCCL,
	IPL_SECTION_PICK,
	IPL_SECTION_TCYC,
	IPL_SECTION_ZONE
};

class IMapFileSectionEntry {
public:
	virtual int8_t GetType() = 0;
	virtual bool Read(const char* szLine) = 0;
	virtual void ReadBinary(uint8_t* pFileContent, int32_t nOffset) = 0;
	virtual void Write(FILE* pFile) = 0;
	virtual void WriteBinary(FILE* pFile) = 0;
};