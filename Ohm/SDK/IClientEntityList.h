#pragma once

#include "../Utility/VirtualFunc.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icliententitylist.h

class IClientEntity;

class IClientEntityList {
public:
	IClientEntity* GetClientEntity(int entityNumber) {
		typedef IClientEntity* (__thiscall* OriginalFn)(void*, int);
		return GetVFunc<OriginalFn>(this, 3)(this, entityNumber);
	}
	int GetHighestEntityIndex() {
		typedef int(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 6)(this);
	}
};