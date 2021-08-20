#pragma once

#include "./CBaseCombatWeapon.h"
#include "./CBaseEntity.h"

#include "../Interfaces/IClientEntity.h"
#include "../Interfaces/IEngineTrace.h"

#include "../Math/Vector.h"

#include "../../Netvars.h"

#include "../../Interfaces/Interfaces.h"

class CBasePlayer : public CBaseEntity {
public:
	int getArmor() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_ArmorValue);
	}
	int getHealth() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_iHealth);
	}
	int getLifeState() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_lifeState);
	}
	bool isAlive() {
		return getLifeState() == LIFE_ALIVE;
	}
	int getTeam() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_iTeamNum);
	}
	int getFlags() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_fFlags);
	}
	bool hasC4() {
		return memory->IsC4Owner(reinterpret_cast<uintptr_t>(this));
	}
	int getTickBase() {
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_nTickBase);
	}
	float getNextAttack() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flNextAttack);
	}
	bool isDefusing() {
		return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + netvars->m_bIsDefusing);
	}
	bool isScoped() {
		return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + netvars->m_bIsScoped);
	}
	float getFlashDuration() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->m_flFlashMaxAlpha);
	}
	bool isFlashed() {
		return getFlashDuration() > 75.f;
	}
	bool hasGunGameImmunity() {
		return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + netvars->m_bGunGameImmunity);
	}
	Vector getEyePosition() {
		Vector vec;

		typedef void(__thiscall* GetEyePositionFn)(void*, Vector&);
		GetVFunc<GetEyePositionFn>(this, 284)(this, vec);

		return vec;
	}
	Vector getAimPunch() {
		Vector vec;

		typedef void(__thiscall* GetAimPunchFn)(void*, Vector&);
		GetVFunc<GetAimPunchFn>(this, 345)(this, vec);

		return vec;
	}
	Vector getBonePosition(int bone) {
		matrix3x4_t boneMatrix[256];
		return SetupBones(boneMatrix, 256, 256, 0.f) ? boneMatrix[bone].GetOrigin() : Vector{};
	}
	CBaseCombatWeapon* getCurrentWeapon() {
		typedef CBaseCombatWeapon* (__thiscall* GetCurrentWeaponFn)(void*);
		return GetVFunc<GetCurrentWeaponFn>(this, 267)(this);
	}
	bool isVisible(CBasePlayer* localPlayer, const Vector& vecDst) {
		Trace trace;

		interfaces->EngineTrace->traceRay({ localPlayer->getEyePosition(), vecDst.notNull() ? vecDst : this->getBonePosition(8) }, 0x46004009, { localPlayer }, trace);

		return trace.entity == this || trace.fraction > .97f;
	}
};