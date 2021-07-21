#include <iostream>
#include <Windows.h>

#include "./Hooks/Hooks.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH)
		hooks = std::make_unique<Hooks>(hModule);

	return TRUE;
}