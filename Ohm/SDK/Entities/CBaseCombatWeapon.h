#pragma once

#include "../Interfaces/IClientEntity.h"

#include "../../Utility/VirtualFunc.h"

class CBaseCombatWeapon : public IClientEntity {
public:
	int getClip() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_iClip);
	}
	float nextPrimaryAttack() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flNextPrimaryAttack);
	}
	WeaponType getWeaponType() {
		typedef WeaponType(__thiscall* GetWeaponTypeFn)(void*);
		return GetVFunc<GetWeaponTypeFn>(this, 454)(this);
	}
	WeaponInfo* getWeaponInfo() {
		typedef WeaponInfo* (__thiscall* GetWeaponInfoFn)(void*);
		return GetVFunc<GetWeaponInfoFn>(this, 460)(this);
	}
	bool isPistol() {
		return getWeaponType() == WeaponType::Pistol;
	}
	bool isSniper() {
		return getWeaponType() == WeaponType::SniperRifle;
	}
	bool isGrenade() {
		return getWeaponType() == WeaponType::Grenade;
	}
	bool requiresRecoilControl() {
		const auto weaponData = getWeaponInfo();

		if (weaponData)
			return weaponData->recoilMagnitude < 35.f && weaponData->recoveryTimeStand > weaponData->cycletime;

		return false;
	}
};