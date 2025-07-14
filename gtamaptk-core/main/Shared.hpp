#pragma once

#include <stdio.h>

#define MAX_IMG_ENTRY_NAME 40
#define MAX_GXT_ENTRY_NAME 8

#define SECTION3(l, a, b, c) ((l[0] == a) && (l[1] == b) && (l[2] == c))
#define SECTION4(l, a, b, c, d) ((l[0] == a) && (l[1] == b) && (l[2] == c) && (l[3] == d))

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "The size of " #struc " is not " #size)
#define OFFSETOF(struc, member) (reinterpret_cast<size_t>(&reinterpret_cast<const volatile char&>(((static_cast<struc*>(nullptr))->member))))
#define VALIDATE_OFFSET(struc, member, offset) static_assert(OFFSETOF(struc, member) == offset, "The offset of " #struc "::" #member " is not " #offset)
#define VALIDATE_ARRAY_INDEX(calc, expected) static_assert((calc) == expected, "The array index of " #calc " is not " #expected)

// Identical to CFileLoader::LoadLine
extern char* ReadLine(FILE* pFile);

extern char gErrorMsg[512];
extern void SetLastErrorMsg(const char* szMessage, ...);
extern const char* GetLastErrorMsg();