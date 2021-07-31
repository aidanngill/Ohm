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

	int Armor() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_ArmorValue);
	}
	int Health() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_iHealth);
	}
	float HealthRatio() {
		return static_cast<float>(std::clamp(this->Health(), 0, 100)) / float(100);
	}
	float ArmorRatio() {
		return static_cast<float>(std::clamp(this->Armor(), 0, 100)) / float(100);
	}
	float NextAttack() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flNextAttack);
	}
	bool IsDefusing() {
		return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + netvars->m_bIsDefusing);
	}
	float FlashDuration() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flFlashMaxAlpha);
	}
	bool IsFlashed() {
		return this->FlashDuration() > 75.f;
	}
	Color HealthColor() {
		float healthRatio = this->HealthRatio();
		return Color(
			static_cast<int>(255 - (255 * healthRatio)),
			static_cast<int>(0 + (255 * healthRatio)),
			0, 255
		);
	}
	int Team() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_iTeamNum);
	}
	int Flags() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_fFlags);
	}
	int TickBase() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_nTickBase);
	}
	Vector VecOrigin() {
		return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(this) + netvars->m_vecOrigin);
	}
	int GetLifeState() {
		return *reinterpret_cast<int*>(reinterpret_cast<int>(this) + netvars->m_lifeState);
	}
	bool IsAlive() {
		return this->Health() > 0 && this->GetLifeState() == LIFE_ALIVE;
	}
	bool IsC4() {
		return this->GetClientClass()->m_ClassID == netvars->classIdentifiers["CPlantedC4"];
	}
	bool HasC4() {
		return memory->IsC4Owner(reinterpret_cast<uintptr_t>(this));
	}
	bool* ShouldGlow() {
		return reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + netvars->m_bShouldGlow);
	}
	bool IsFogController() {
		return this->GetClientClass()->m_ClassID == netvars->classIdentifiers["FogController"];
	}
	bool IsWeapon() {
		IClientClass* thisClass = this->GetClientClass();

		if (thisClass->m_ClassID == netvars->classIdentifiers["CAK47"] ||
			thisClass->m_ClassID == netvars->classIdentifiers["CDEagle"] ||
			thisClass->m_ClassID == netvars->classIdentifiers["CKnife"])
			return true;

		static const char* weaponStr = "CWeapon";
		static int weaponStrLen = strlen(weaponStr);

		return strncmp(thisClass->m_pNetworkName, weaponStr, weaponStrLen) == 0;
	}
	float BombTimer() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flC4Blow);
	}
	float BombLength() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flTimerLength);
	}
	ICollideable* GetCollideable() {
		return reinterpret_cast<ICollideable*>(reinterpret_cast<uintptr_t>(this) + netvars->m_Collision);
	}
	WeaponInfo* GetWeaponInfo() {
		typedef WeaponInfo*(__thiscall* GetWeaponInfoFn)(void*);
		return GetVFunc<GetWeaponInfoFn>(this, 460)(this);
	}
	Vector GetEyePosition() {
		Vector eyePos;

		typedef void(__thiscall* GetEyePositionFn)(void*, Vector&);
		GetVFunc<GetEyePositionFn>(this, 284)(this, eyePos);

		return eyePos;
	}
	Vector GetAimPunch() {
		Vector aimPunch;

		typedef void(__thiscall* GetAimPunchFn)(void*, Vector&);
		GetVFunc<GetAimPunchFn>(this, 345)(this, aimPunch);

		return aimPunch;
	}
	bool RequiresRecoilControl() {
		const auto weaponData = GetWeaponInfo();

		if (weaponData)
			return weaponData->recoilMagnitude < 35.f && weaponData->recoveryTimeStand > weaponData->cycletime;

		return false;
	}
};