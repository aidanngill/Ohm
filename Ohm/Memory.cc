#include <Windows.h>

#include "./Memory.h"

#include "./Interfaces/Interfaces.h"

#include "./Utility/Utilities.h"

template <typename T>
static constexpr auto RelativeToAbsolute(uintptr_t address) noexcept {
	return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

Memory::Memory() {
	HANDLE engineHandle = GetModuleHandle(L"engine.dll");
	HANDLE clientHandle = GetModuleHandle(L"client.dll");

	ClientState = **reinterpret_cast<CClientState***>(PatternScan(engineHandle, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	GlobalVars = **reinterpret_cast<CGlobalVars***>((*reinterpret_cast<uintptr_t**>(interfaces->BaseClient))[11] + 10);
	GlowObjectManager = *reinterpret_cast<CGlowObjectManager**>(PatternScan(clientHandle, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
	LocalPlayer = *reinterpret_cast<IClientEntity**>(PatternScan(clientHandle, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
	IsC4Owner = *reinterpret_cast<bool(__thiscall*)(uintptr_t)>(PatternScan(clientHandle, "56 8B F1 85 F6 74 31"));

	KeyValuesFromString = RelativeToAbsolute<decltype(KeyValuesFromString)>((uintptr_t)PatternScan(clientHandle, "E8 ? ? ? ? 83 C4 04 89 45 D8") + 1);
	KeyValuesFindKey = RelativeToAbsolute<decltype(KeyValuesFindKey)>((uintptr_t)PatternScan(clientHandle, "E8 ? ? ? ? F7 45") + 1);
	KeyValuesSetString = RelativeToAbsolute<decltype(KeyValuesSetString)>((uintptr_t)PatternScan(clientHandle, "E8 ? ? ? ? 89 77 38") + 1);
}