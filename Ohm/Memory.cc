#include <Windows.h>

#include "./Memory.h"
#include "./Utility/Utilities.h"

Memory::Memory() {
	HANDLE engine_handle = GetModuleHandle(L"engine.dll");
	HANDLE client_handle = GetModuleHandle(L"client.dll");

	// Need to find a way to "refresh" the local player after the player joins a new session, as when the player
	// joins a new match the values go up to nonsensical numbers.
	LocalPlayer = **reinterpret_cast<IClientEntity***>(PatternScan(client_handle, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
	ClientState = **reinterpret_cast<CClientState***>(PatternScan(engine_handle, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
}