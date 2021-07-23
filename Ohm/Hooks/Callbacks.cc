#include "./Callbacks.h"
#include "./Hooks.h"

#include "../Interfaces.h"
#include "../InterfaceDep.h"

#include "../GUI/Menu.h"

#include "../SDK/IPanel.h"

#include "../Utility/Utilities.h"

LRESULT __stdcall WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!hooks->IsWindowHooked())
        return NULL;

    if (msg == WM_KEYDOWN && LOWORD(wParam) == VK_HOME) {
        hooks->Restore();
        return 0;
    }

    if (msg == WM_KEYDOWN && LOWORD(wParam) == VK_INSERT) {
        menu->is_open = !menu->is_open;
        if (!menu->is_open) interfaces->InputSystem->ResetInputState();
    }

    menu->Controls(msg, wParam, lParam);
    interfaces->InputSystem->EnableInput(!menu->is_open);

    return hooks->ReturnWindowCallback(window, msg, wParam, lParam);
}

DWORD __stdcall Unload(HMODULE hModule) {
    DetachGameConsole();
    FreeLibraryAndExitThread(hModule, 0);

    return NULL;
}