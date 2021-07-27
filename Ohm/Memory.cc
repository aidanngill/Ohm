#include <Windows.h>

#include "./Memory.h"

#include "./Interfaces/Interfaces.h"

#include "./Utility/Utilities.h"

Memory::Memory() {
	HANDLE engine_handle = GetModuleHandle(L"engine.dll");
	HANDLE client_handle = GetModuleHandle(L"client.dll");

	ClientState = **reinterpret_cast<CClientState***>(PatternScan(engine_handle, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	GlobalVars = **reinterpret_cast<CGlobalVars***>((*reinterpret_cast<uintptr_t**>(interfaces->BaseClient))[11] + 10);
	LocalPlayer = *reinterpret_cast<IClientEntity**>(PatternScan(client_handle, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
}