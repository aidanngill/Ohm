#pragma once

#include "./IClientNetworkable.h"
#include "./IClientRenderable.h"
#include "./IClientThinkable.h"
#include "./IClientUnknown.h"

#include "./ClientClass.h"
#include "./ICollideable.h"

#include "../Netvars.h"

enum LifeState : unsigned char {
    LIFE_ALIVE			= 0,
    LIFE_DYING          = 1,
    LIFE_DEAD           = 2,
    LIFE_RESPAWNABLE	= 3,
    LIFE_DISCARDBODY    = 4,
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
    int Team() {
        return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + netvars->m_iTeamNum);
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
        return this->GetClientClass()->m_ClassID == netvars->class_identifiers["CPlantedC4"];
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
};