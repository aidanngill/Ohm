#include <thread>
#include <chrono>

#include "./Hooks.h"
#include "./Callbacks.h"
#include "../Utilities.h"

Hook::Hook(void* given_class_pointer) {
	this->class_pointer = reinterpret_cast<uintptr_t**>(given_class_pointer);

	int table_size = 0;
	while (reinterpret_cast<uintptr_t*>(*this->class_pointer)[table_size])
		table_size++;

	original_pointer = *this->class_pointer;
	new_table_pointer = new uintptr_t[sizeof(uintptr_t) * table_size];

	memcpy(new_table_pointer, original_pointer, sizeof(uintptr_t) * table_size);
}

Hook::~Hook() {
	Restore();

	delete original_pointer;
	delete new_table_pointer;
}

void Hook::SwapPointer(size_t index, void* new_function) {
	new_table_pointer[index] = reinterpret_cast<uintptr_t>(new_function);
}

void Hook::Apply() {
	*class_pointer = new_table_pointer;
}

void Hook::Restore() {
	*class_pointer = original_pointer;
}

template<typename T>
T Hook::GetOriginal(size_t index) {
	return reinterpret_cast<T>(original_pointer[index]);
}

Hooks::Hooks(HMODULE hModule) {
	module = hModule;
	window = FindWindow(L"Valve001", nullptr);

	AttachGameConsole();
	original_wnd_proc = WNDPROC(SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(WndProc)));

	Install();
	std::cout << "[+] Installed the hooks." << std::endl;
}

void Hooks::Install() {}

void Hooks::Restore() {
	SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(original_wnd_proc));
	HANDLE thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Unload, module, NULL, NULL);

	if (thread) CloseHandle(thread);
}

bool Hooks::IsWindowHooked() {
	return original_wnd_proc != nullptr;
}

LRESULT Hooks::ReturnWindowCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	return CallWindowProcW(original_wnd_proc, window, msg, wParam, lParam);
}