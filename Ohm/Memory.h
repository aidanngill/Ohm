#pragma once

#include <iostream>

class CClientState;
class CGlobalVars;
class IClientEntity;

class Memory {
public:
	Memory();

	CClientState* ClientState;
	CGlobalVars* GlobalVars;
	IClientEntity* LocalPlayer;
};

inline std::unique_ptr<Memory> memory;