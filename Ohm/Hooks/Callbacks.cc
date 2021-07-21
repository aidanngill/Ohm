#include "./Callbacks.hh"
#include "./Hooks.hh"
#include "../Interfaces.hh"
#include "../Utility/Utilities.hh"

#include "../SDK/IPanel.hh"

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