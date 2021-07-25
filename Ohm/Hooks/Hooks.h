#pragma once

#include <iostream>
#include <Windows.h>

// Implementation of a Virtual Method Table (VMT) Hook.
// https://en.wikipedia.org/wiki/Virtual_method_table
class VmtHook {
public:
	VmtHook(void* class_ptr);
	~VmtHook();
	void SwapPointer(size_t index, void* new_function);
	void ApplyNewTable();
	void RestoreOldTable();
	template<typename T>
	T GetOriginal(size_t index);
private:
	uintptr_t** class_pointer = nullptr;
	uintptr_t* original_pointer = nullptr;
	uintptr_t* new_table_pointer = nullptr;
};

class Hooks {
public:
	Hooks(HMODULE hModule);
	void Install();
	void Restore();

	bool IsWindowHooked();
	LRESULT ReturnWindowCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

	VmtHook* VGUI;
	VmtHook* ClientInput;
	VmtHook* Surface;
private:
	WNDPROC original_wnd_proc = nullptr;
	HWND window = nullptr;
	HMODULE module = nullptr;
};

// Define `hooks` once. C++ 17 Standard is necessary
// for `inline` in this case.
inline std::unique_ptr<Hooks> hooks;