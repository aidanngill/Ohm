#pragma once

#include <algorithm>

#include "./IClientNetworkable.h"
#include "./IClientRenderable.h"
#include "./IClientThinkable.h"
#include "./IClientUnknown.h"

#include "./Color.h"
#include "./ICollideable.h"
#include "./WeaponData.h"

#include "../Memory.h"
#include "../Netvars.h"

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

	Vector getOrigin() {
		return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(this) + netvars->m_vecOrigin);
	}
	bool isC4() {
		return this->GetClientClass()->m_pNetworkName == "CC4";
	}
	bool isFogController() {
		return this->GetClientClass()->m_pNetworkName == "FogController";
	}
	bool isWeapon() {
		IClientClass* thisClass = this->GetClientClass();

		if (thisClass->m_ClassID == netvars->classIdentifiers["CAK47"] ||
			thisClass->m_ClassID == netvars->classIdentifiers["CDEagle"] ||
			thisClass->m_ClassID == netvars->classIdentifiers["CKnife"])
			return true;

		static const char* weaponStr = "CWeapon";
		static int weaponStrLen = strlen(weaponStr);

		return strncmp(thisClass->m_pNetworkName, weaponStr, weaponStrLen) == 0;
	}
	ICollideable* getCollideable() {
		return reinterpret_cast<ICollideable*>(reinterpret_cast<uintptr_t>(this) + netvars->m_Collision);
	}
	WeaponInfo* getWeaponInfo() {
		typedef WeaponInfo*(__thiscall* GetWeaponInfoFn)(void*);
		return GetVFunc<GetWeaponInfoFn>(this, 460)(this);
	}
	bool requiresRecoilControl() {
		const auto weaponData = getWeaponInfo();

		if (weaponData)
			return weaponData->recoilMagnitude < 35.f && weaponData->recoveryTimeStand > weaponData->cycletime;

		return false;
	}
	bool* shouldGlow() {
		return reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + netvars->m_bShouldGlow);
	}
};