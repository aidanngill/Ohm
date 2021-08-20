#pragma once

#include "../../Netvars.h"

class CBaseEntity : public IClientEntity {
public:
	const matrix3x4_t& getCoordinateFrame() {
		return *reinterpret_cast<matrix3x4_t*>(reinterpret_cast<uintptr_t>(this) + netvars->m_rgflCoordinateFrame);
	}
};