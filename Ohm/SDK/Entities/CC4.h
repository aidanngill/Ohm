#pragma once

#include "../Interfaces/IClientEntity.h"

class CC4 : public IClientEntity {
public:
	float getTimer() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flC4Blow);
	}
	float getLength() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flTimerLength);
	}
};