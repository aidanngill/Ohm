#pragma once

#include "./IClientClass.h"

#include "../Utility/VirtualFunc.h"

class IBaseClientDLL {
public:
	IClientClass* GetAllClasses() {
		typedef IClientClass* (__thiscall* oGetAllClasses)(void*);
		return GetVFunc<oGetAllClasses>(this, 8)(this);
	}
};