#pragma once

#include "./IClientClass.h"

#include "../Utility/VirtualFunc.h"

enum DataUpdateType_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

class IClientNetworkable {
public:
	virtual ~IClientNetworkable() {};
	void Release() {
		typedef void(__thiscall* oRelease)(void*);
		return GetVFunc<oRelease>(this, 1)(this);
	}
	IClientClass* GetClientClass() {
		typedef IClientClass* (__thiscall* oGetClientClass)(void*);
		return GetVFunc<oGetClientClass>(this, 2)(this);
	}
	void PreDataUpdate(DataUpdateType_t updateType) {
		typedef void(__thiscall* oPreDataUpdate)(void*, DataUpdateType_t);
		return GetVFunc<oPreDataUpdate>(this, 6)(this, updateType);
	}
	bool GetDormant() {
		typedef bool(__thiscall* oGetDormant)(void*);
		return GetVFunc<oGetDormant>(this, 9)(this);
	}
	int GetIndex() {
		typedef int(__thiscall* oGetIndex)(void*);
		return GetVFunc<oGetIndex>(this, 10)(this);
	}
	void SetDestroyedOnRecreateEntities() {
		typedef void(__thiscall* oSetDestroyedOnRecreateEntities)(void*);
		return GetVFunc<oSetDestroyedOnRecreateEntities>(this, 13)(this);
	}
};