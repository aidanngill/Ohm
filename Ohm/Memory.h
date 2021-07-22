#pragma once

#include <iostream>

class CClientState;
class IClientEntity;

class Memory {
public:
	Memory();
	IClientEntity* LocalPlayer;
	CClientState* ClientState;
};

inline std::unique_ptr<Memory> memory;