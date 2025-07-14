#pragma once

#include "Vector3d.h"

#pragma pack(push, 1)
class Box {
public:
	Vector3d m_vMin;
	Vector3d m_vMax;
};
#pragma pack(pop)