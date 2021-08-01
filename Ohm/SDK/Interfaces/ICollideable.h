#pragma once

#include "../Math/Vector.h"

#include "../../Utility/VirtualFunc.h"

class ICollideable {
public:
	Vector& OBBMins() {
		typedef Vector&(__thiscall* OriginalFn)(ICollideable*);
		return GetVFunc<OriginalFn>(this, 1)(this);
	}
	Vector& OBBMaxs() {
		typedef Vector&(__thiscall* OriginalFn)(ICollideable*);
		return GetVFunc<OriginalFn>(this, 2)(this);
	}
};