#pragma once

#include <iostream>

class CClientState;
class CGlobalVars;
class IClientEntity;

class KeyValues;

class Memory {
public:
	Memory();

	CClientState* ClientState;
	CGlobalVars* GlobalVars;
	IClientEntity* LocalPlayer;
	uintptr_t KeyValuesFromString;
	KeyValues* (__thiscall* KeyValuesFindKey)(KeyValues* key_values, const char* key_name, bool create);
	void(__thiscall* KeyValuesSetString)(KeyValues* key_values, const char* value);
};

inline std::unique_ptr<Memory> memory;