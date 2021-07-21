#include "./Callbacks.h"
#include "./Hooks.h"
#include "../Interfaces.h"
#include "../Utility/Utilities.h"

#include "../SDK/IPanel.h"

LRESULT __stdcall WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!hooks->IsWindowHooked())
        return NULL;

    if (msg == WM_KEYDOWN && LOWORD(wParam) == VK_HOME) {
        hooks->Restore();
        return 0;
    }

    return hooks->ReturnWindowCallback(window, msg, wParam, lParam);
}

DWORD __stdcall Unload(HMODULE hModule) {
    DetachGameConsole();
    FreeLibraryAndExitThread(hModule, 0);

    return NULL;
}