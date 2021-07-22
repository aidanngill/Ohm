#include <thread>
#include <chrono>

#include "./Hooks.h"
#include "./Callbacks.h"

#include "../Interfaces.h"

#include "../GUI/Font.h"
#include "../GUI/Render.h"

#include "../SDK/IPanel.h"
#include "../SDK/ISurface.h"

#include "../Utility/Utilities.h"

// In the future, I'd like to move this to `Callbacks.cc`.
typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/vgui/IPanel.h#L99
void __fastcall PaintTraverse(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce = true) {
	if (hooks == nullptr || hooks->VGUI == nullptr)
		return;

	hooks->VGUI->GetOriginal<PaintTraverseFn>(41)(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int focus_overlay_panel = 0;
	static bool found_panel = false;

	if (!found_panel) {
		const char* panel_name = interfaces->Panel->GetName(vguiPanel);

		if (!strstr(panel_name, "MatSystemTopPanel"))
			return;

		focus_overlay_panel = vguiPanel;
		found_panel = true;
	}

	if (focus_overlay_panel != vguiPanel)
		return;

	// Do draw functions on ISurface here.
	render->Watermark();
}

VmtHook::VmtHook(void* class_ptr) {
	this->class_pointer = reinterpret_cast<uintptr_t**>(class_ptr);

	int table_size = 0;

	while (reinterpret_cast<uintptr_t*>(*this->class_pointer)[table_size])
		table_size++;

	original_pointer = *this->class_pointer;

	new_table_pointer = new uintptr_t[sizeof(uintptr_t) * table_size];
	memcpy(new_table_pointer, original_pointer, sizeof(uintptr_t) * table_size);
}

VmtHook::~VmtHook() {
	RestoreOldTable();
	delete original_pointer;
	delete new_table_pointer;
}

void VmtHook::SwapPointer(size_t index, void* new_function) {
	new_table_pointer[index] = reinterpret_cast<uintptr_t>(new_function);
}

void VmtHook::ApplyNewTable() {
	*class_pointer = new_table_pointer;
}

void VmtHook::RestoreOldTable() {
	*class_pointer = original_pointer;
}

template<typename T>
T VmtHook::GetOriginal(size_t index) {
	return reinterpret_cast<T>(original_pointer[index]);
}

Hooks::Hooks(HMODULE module) {
	AttachGameConsole();
	printf("[+] Started the cheat on module %#08p.\n", module);

	this->module = module;

	interfaces = std::make_unique<Interfaces>();
	printf("[+] Initialized all interfaces.\n");

	render = std::make_unique<Render>();
	printf("[+] Initialized the renderer.\n");

	this->window = FindWindow(L"Valve001", nullptr);
	this->original_wnd_proc = WNDPROC(SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(WndProc)));

	this->Install();
	printf("[+] Initialized all hooks.\n");
}

void Hooks::Install() {
	VGUI = new VmtHook(interfaces->Panel);
	VGUI->SwapPointer(41, reinterpret_cast<void*>(PaintTraverse));
	VGUI->ApplyNewTable();
}

void Hooks::Restore() {
	hooks->VGUI->RestoreOldTable();

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