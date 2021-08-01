#pragma once

#include "../Utility/VirtualFunc.h"

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

class IAppSystem {
public:
	bool Connect(CreateInterfaceFn factory) {
		typedef bool(__thiscall* OriginalFn)(void*, CreateInterfaceFn);
		return GetVFunc<OriginalFn>(this, 0)(this, factory);
	}
	void Disconnect() {
		typedef void(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 1)(this);
	}
	void* QueryInterface(const char* interfaceName) {
		typedef void* (__thiscall* OriginalFn)(void*, const char*);
		return GetVFunc<OriginalFn>(this, 2)(this, interfaceName);
	}
	int Init() {
		typedef int(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 3)(this);
	}
	void Shutdown() {
		typedef void(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 4)(this);
	}
	const void* GetDependencies() {
		typedef const void* (__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 5)(this);
	}
	int GetTier() {
		typedef int(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 6)(this);
	}
	void Reconnect(CreateInterfaceFn factory, const char* interfaceName) {
		typedef void(__thiscall* OriginalFn)(void*, CreateInterfaceFn, const char*);
		return GetVFunc<OriginalFn>(this, 7)(this, factory, interfaceName);
	}
};