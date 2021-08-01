#pragma once

#include <iostream>

#include "./IAppSystem.h"
#include "./IMaterial.h"

#include "../../Utility/VirtualFunc.h"

class IMaterialSystem
{
public:
	IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues) {
		typedef IMaterial* (__thiscall* oCreateMaterial)(void*, const char*, KeyValues*);
		return GetVFunc<oCreateMaterial>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}
	IMaterial* FindMaterial(const char* material_name, const char* texture_group_name = nullptr, bool complain = true, const char* complain_prefix = NULL) {
		typedef IMaterial* (__thiscall* oFindMaterial)(void*, const char*, const char*, bool, const char*);
		return GetVFunc<oFindMaterial>(this, 84)(this, material_name, texture_group_name, complain, complain_prefix);
	}
};