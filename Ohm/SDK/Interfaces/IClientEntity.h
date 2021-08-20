#pragma once

#include <algorithm>

#include "./ICollideable.h"

#include "./IClientNetworkable.h"
#include "./IClientRenderable.h"
#include "./IClientThinkable.h"
#include "./IClientUnknown.h"

#include "../Color.h"
#include "../WeaponData.h"

#include "../Math/Vector.h"

#include "../../Memory.h"
#include "../../Netvars.h"

enum LifeState : unsigned char {
	LIFE_ALIVE = 0,
	LIFE_DYING = 1,
	LIFE_DEAD = 2,
	LIFE_RESPAWNABLE = 3,
	LIFE_DISCARDBODY = 4,
	LIFE_MAX
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable {
public:
	virtual ~IClientEntity() {};

	bool isC4() {
		return this->GetClientClass()->m_ClassID == netvars->classIdentifiers.CC4;
	}
	bool isFogController() {
		return this->GetClientClass()->m_ClassID == netvars->classIdentifiers.FogController;
	}
	bool isWeapon() {
		typedef bool(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 165)(this);
	}
	Vector getOrigin() {
		return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(this) + netvars->m_vecOrigin);
	}
	ICollideable* getCollideable() {
		return reinterpret_cast<ICollideable*>(reinterpret_cast<uintptr_t>(this) + netvars->m_Collision);
	}
	bool* shouldGlow() {
		return reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + netvars->m_bShouldGlow);
	}
};