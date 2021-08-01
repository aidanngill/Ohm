#pragma once

#include "./IClientClass.h"

#include "../../Utility/VirtualFunc.h"

class IBaseClientDLL {
public:
	IClientClass* GetAllClasses() {
		typedef IClientClass* (__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 8)(this);
	}
};