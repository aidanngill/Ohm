#pragma once

#include "../Utility/VirtualFunc.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icliententitylist.h

class IClientEntity;

class IClientEntityList {
public:
	IClientEntity* GetClientEntity(int entity_number) {
		typedef IClientEntity* (__thiscall* oGetEntity)(void*, int);
		return GetVFunc<oGetEntity>(this, 3)(this, entity_number);
	}
	int GetHighestEntityIndex() {
		typedef int(__thiscall* oGetHighestEntityIndex)(void*);
		return GetVFunc<oGetHighestEntityIndex>(this, 6)(this);
	}
};