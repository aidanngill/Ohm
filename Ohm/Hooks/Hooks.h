#pragma once

#include <iostream>
#include <Windows.h>

// Implementation of a Virtual Method Table (VMT) Hook.
// https://en.wikipedia.org/wiki/Virtual_method_table
class Hook {
public:
	Hook(void* given_class_pointer);
	~Hook();

	void SwapPointer(size_t index, void* new_function);

	void Apply();
	void Restore();

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

	// WndProc Functions
	bool IsWindowHooked();
	LRESULT ReturnWindowCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	WNDPROC original_wnd_proc = nullptr;
	HWND window = nullptr;
	HMODULE module = nullptr;
};

// Define hooks once. C++ 17 Standard is necessary
// for `inline` in this case.
inline std::unique_ptr<Hooks> hooks;