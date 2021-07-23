#pragma once

#include "../Utility/VirtualFunc.h"

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

class IAppSystem
{
public:
	bool Connect(CreateInterfaceFn factory)
	{
		typedef bool(__thiscall* oConnect)(void*, CreateInterfaceFn);
		return GetVFunc<oConnect>(this, 0)(this, factory);
	}
	void Disconnect()
	{
		typedef void(__thiscall* oDisconnect)(void*);
		return GetVFunc<oDisconnect>(this, 1)(this);
	}
	void* QueryInterface(const char* interface_name)
	{
		typedef void* (__thiscall* oQueryInterface)(void*, const char*);
		return GetVFunc<oQueryInterface>(this, 2)(this, interface_name);
	}
	int Init()
	{
		typedef int(__thiscall* oInit)(void*);
		return GetVFunc<oInit>(this, 3)(this);
	}
	void Shutdown()
	{
		typedef void(__thiscall* oShutdown)(void*);
		return GetVFunc<oShutdown>(this, 4)(this);
	}
	const void* GetDependencies()
	{
		typedef const void* (__thiscall* oGetDependencies)(void*);
		return GetVFunc<oGetDependencies>(this, 5)(this);
	}
	int GetTier()
	{
		typedef int(__thiscall* oGetTier)(void*);
		return GetVFunc<oGetTier>(this, 6)(this);
	}
	void Reconnect(CreateInterfaceFn factory, const char* interface_name)
	{
		typedef void(__thiscall* oReconnect)(void*, CreateInterfaceFn, const char*);
		return GetVFunc<oReconnect>(this, 7)(this, factory, interface_name);
	}
};