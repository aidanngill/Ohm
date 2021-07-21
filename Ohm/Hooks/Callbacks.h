#pragma once

#include <Windows.h>

LRESULT __stdcall WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI Unload(HMODULE hModule);