#include "./Callbacks.h"
#include "./Hooks.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../GUI/Menu/Menu.h"

#include "../Utility/Utilities.h"

LRESULT __stdcall WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (!hooks->IsWindowHooked())
		return NULL;

	if (msg == WM_KEYDOWN && LOWORD(wParam) == VK_HOME) {
		hooks->Restore();
		return 0;
	}

	if (msg == WM_KEYDOWN && LOWORD(wParam) == VK_INSERT) {
		menu->isOpen = !menu->isOpen;
		if (!menu->isOpen) interfaces->InputSystem->ResetInputState();
	}

	menu->Controls(msg, wParam, lParam);
	interfaces->InputSystem->EnableInput(!menu->isOpen);

	return hooks->ReturnWindowCallback(window, msg, wParam, lParam);
}

DWORD __stdcall Unload(HMODULE hModule) {
	Utilities::detachConsole();
	FreeLibraryAndExitThread(hModule, 0);

	return NULL;
}