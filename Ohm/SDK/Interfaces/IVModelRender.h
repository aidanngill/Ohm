#pragma once

#include "./IMaterial.h"

#include "../Math/QAngle.h"

#include "../../Utility/VirtualFunc.h"

class IVModelRender {
public:
	void ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0) {
		typedef void(__thiscall* oForceMaterialOverride)(void*, IMaterial*, OverrideType_t, int);
		return GetVFunc<oForceMaterialOverride>(this, 1)(this, newMaterial, nOverrideType, nOverrides);
	}
	bool IsForcedMaterialOverride(void) {
		typedef bool(__thiscall* oIsForcedMaterialOverride)(void*);
		return GetVFunc<oIsForcedMaterialOverride>(this, 2)(this);
	}
};
