#pragma once

#include "./IClientNetworkable.h"
#include "./IClientRenderable.h"
#include "./IClientThinkable.h"
#include "./IClientUnknown.h"

#include "../Netvars.h"

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable {
public:
	virtual ~IClientEntity() {};
	int Health() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_iHealth);
	}
};