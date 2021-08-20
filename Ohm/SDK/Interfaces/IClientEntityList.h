#pragma once

#include "../../Utility/VirtualFunc.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icliententitylist.h

class CBaseEntity;

class IClientEntityList {
public:
	CBaseEntity* GetClientEntity(int entityNumber) {
		typedef CBaseEntity* (__thiscall* OriginalFn)(void*, int);
		return GetVFunc<OriginalFn>(this, 3)(this, entityNumber);
	}
	int GetHighestEntityIndex() {
		typedef int(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 6)(this);
	}
};