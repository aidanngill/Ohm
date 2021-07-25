#include <thread>
#include <chrono>

#include "./Hooks.h"
#include "./Callbacks.h"

#include "../Interfaces.h"
#include "../InterfaceDep.h"

#include "../Memory.h"
#include "../Netvars.h"

#include "../Features/Visuals.h"

#include "../GUI/Font.h"
#include "../GUI/Menu.h"
#include "../GUI/Render.h"

#include "../SDK/CUserCmd.h"

#include "../Utility/Utilities.h"

typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
typedef bool(__thiscall* CreateMoveFn)(void*, float, CUserCmd*);

// In the future, I'd like to move this to `Callbacks.cc`.
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

	render->Watermark();
	render->Visuals();

	menu->Render();
}

static bool __stdcall CreateMove(float input_sample_frametime, CUserCmd* cmd) {
	if (hooks == nullptr || hooks->ClientInput == nullptr)
		return false;

	// We can only get the class IDs when we are in game.
	if (interfaces->Engine->IsInGame() && !netvars->ci_initialized)
		netvars->ci_initialized = netvars->InitializeClassIdentifiers();

	auto result = hooks->ClientInput->GetOriginal<CreateMoveFn>(24)(interfaces->ClientMode, input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return result;

	return false;
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
	printf("[#] 0x%08X -> Module\n", reinterpret_cast<int>(module));

	this->module = module;

	interfaces = std::make_unique<Interfaces>();
	netvars = std::make_unique<Netvars>();
	render = std::make_unique<Render>();
	memory = std::make_unique<Memory>();
	menu = std::make_unique<Menu>();

	this->window = FindWindow(L"Valve001", nullptr);
	this->original_wnd_proc = WNDPROC(SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(WndProc)));

	std::this_thread::sleep_for(std::chrono::milliseconds(25));

	this->Install();
}

void Hooks::Install() {
	VGUI = new VmtHook(interfaces->Panel);
	VGUI->SwapPointer(41, reinterpret_cast<void*>(PaintTraverse));
	VGUI->ApplyNewTable();

	ClientInput = new VmtHook(interfaces->ClientMode);
	ClientInput->SwapPointer(24, reinterpret_cast<void*>(CreateMove));
	ClientInput->ApplyNewTable();
}

void Hooks::Restore() {
	VGUI->RestoreOldTable();
	ClientInput->RestoreOldTable();

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