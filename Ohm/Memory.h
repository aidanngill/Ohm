#pragma once

#include <iostream>

class CBasePlayer;
class CClientState;
class CGlobalVars;
class CGlowObjectManager;

class KeyValues;
class Vector;

class Memory {
public:
	Memory();

	CBasePlayer* LocalPlayer;
	CClientState* ClientState;
	CGlobalVars* GlobalVars;
	CGlowObjectManager* GlowObjectManager;
	bool(__thiscall* IsC4Owner)(uintptr_t);

	uintptr_t KeyValuesFromString;
	KeyValues* (__thiscall* KeyValuesFindKey)(KeyValues* key_values, const char* key_name, bool create);
	void(__thiscall* KeyValuesSetString)(KeyValues* key_values, const char* value);
	bool(__cdecl* GoesThroughSmoke)(Vector, Vector, short);
};

inline std::unique_ptr<Memory> memory;