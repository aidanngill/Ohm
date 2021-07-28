#pragma once

#include <iostream>
#include <Windows.h>

// Implementation of a Virtual Method Table (VMT) Hook.
// https://en.wikipedia.org/wiki/Virtual_method_table
class VmtHook {
public:
	VmtHook(void* targetClassPointer);
	~VmtHook();
	void SwapPointer(size_t index, void* newFunction);
	void ApplyNewTable();
	void RestoreOldTable();
	template<typename T>
	T GetOriginal(size_t index);
private:
	uintptr_t** classPointer = nullptr;
	uintptr_t* originalPointer = nullptr;
	uintptr_t* newTablePointer = nullptr;
};

class Hooks {
public:
	Hooks(HMODULE hModule);
	void Install();
	void Restore();

	bool IsWindowHooked();
	LRESULT ReturnWindowCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

	VmtHook* ClientInput;
	VmtHook* ModelRender;
	VmtHook* Surface;
	VmtHook* VGUI;
private:
	WNDPROC originalWndProc = nullptr;
	HWND window = nullptr;
	HMODULE module = nullptr;
};

// Define `hooks` once. C++ 17 Standard is necessary
// for `inline` in this case.
inline std::unique_ptr<Hooks> hooks;