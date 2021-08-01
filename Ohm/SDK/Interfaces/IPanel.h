#pragma once

#include "../../Utility/VirtualFunc.h"

class IPanel {
public:
	const char* GetName(unsigned int vguiPanel) {
		typedef const char* (__thiscall* OriginalFn)(void*, int);
		return GetVFunc<OriginalFn>(this, 36)(this, vguiPanel);
	}
};