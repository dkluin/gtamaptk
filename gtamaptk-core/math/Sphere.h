#pragma once

#include "Vector3d.h"

#pragma pack(push, 1)
class Sphere {
public:
	Vector3d m_vCenter;
	float m_fRadius;
};
#pragma pack(pop)