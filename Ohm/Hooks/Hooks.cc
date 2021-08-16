#include <thread>
#include <chrono>

#include "./Hooks.h"
#include "./Callbacks.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../Config.h"
#include "../Memory.h"
#include "../Netvars.h"

#include "../Features/Aimbot.h"
#include "../Features/Chams.h"
#include "../Features/Misc.h"
#include "../Features/Visuals.h"

#include "../GUI/Font.h"
#include "../GUI/Render.h"
#include "../GUI/Menu/Menu.h"

#include "../SDK/Class/CUserCmd.h"
#include "../SDK/Class/CViewSetup.h"

#include "../Utility/Utilities.h"

typedef bool(__thiscall* CreateMoveFn)(void*, float, CUserCmd*);
typedef void(__thiscall* PostScreenEffectsFn)(void*, CViewSetup*);
typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
typedef void(__thiscall* OnResolutionChangeFn)(ISurface*, int, int);
typedef void(__thiscall* LockCursorFn)(ISurface*);
typedef void(__thiscall* DrawModelExecuteFn)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

// In the future, I'd like to move this to `Callbacks.cc`.
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/vgui/IPanel.h#L99
void __fastcall PaintTraverse(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce = true) {
	if (hooks == nullptr || hooks->VGUI == nullptr)
		return;

	hooks->VGUI->GetOriginal<PaintTraverseFn>(41)(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int focusOverlayPanel;
	static bool foundPanel = false;

	if (!foundPanel) {
		const char* panelName = interfaces->Panel->GetName(vguiPanel);

		if (!strstr(panelName, "MatSystemTopPanel"))
			return;

		focusOverlayPanel = vguiPanel;
		foundPanel = true;
	}

	if (focusOverlayPanel != vguiPanel)
		return;

	Visuals::Render();

	render->Watermark();
	menu->Render();
}

void __fastcall PostScreenEffects(void* ecx, void* edx, CViewSetup* setup) {
	static auto originalFn = hooks->ClientInput->GetOriginal<PostScreenEffectsFn>(44);

	if (!interfaces->Engine->IsConnected())
		originalFn(ecx, setup);

	Glow::Render();
	originalFn(ecx, setup);
}

static bool __stdcall CreateMove(float inputSampleFrametime, CUserCmd* cmd) {
	if (hooks == nullptr || hooks->ClientInput == nullptr)
		return false;

	// We can only get the class IDs when we are in game.
	if (interfaces->Engine->IsInGame() && !netvars->classIdentifiersInitialized)
		netvars->classIdentifiersInitialized = netvars->InitializeClassIdentifiers();

	auto result = hooks->ClientInput->GetOriginal<CreateMoveFn>(24)(interfaces->ClientMode, inputSampleFrametime, cmd);

	if (!cmd || !cmd->commandNumber)
		return result;

	Vector oldAngles = cmd->viewAngles;
	float oldForward = cmd->forwardMove;
	float oldSide = cmd->sideMove;

	Aimbot::Run(cmd);
	Misc::BunnyHop(cmd);

	if (config->aim.silentAim)
		Aimbot::FixMovement(cmd, oldAngles, oldForward, oldSide);

	return false;
}

static void __stdcall OnResolutionChanged(int oldWidth, int oldHeight) {
	hooks->Surface->GetOriginal<OnResolutionChangeFn>(116);
	render->SetupFonts();
}

void __fastcall LockCursor(void* ecx) {
	if (hooks->Surface == nullptr)
		return;

	if (!menu->isOpen)
		return hooks->Surface->GetOriginal<LockCursorFn>(67)(interfaces->Surface);

	interfaces->Surface->UnlockCursor();
	interfaces->InputSystem->ResetInputState();
}

void __fastcall DrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix) {
	static bool chamsInitialized = false;

	// Initialize within DME thread to avoid issues with continually making new threads.
	if (!chamsInitialized) {
		chams = std::make_unique<Chams>();
		chamsInitialized = true;
	}

	// Make sure that skins are skipped when we're trying to put chams on.
	if (interfaces->ModelRender->IsForcedMaterialOverride() && info.skin != 0)
		return hooks->ModelRender->GetOriginal<DrawModelExecuteFn>(21)(_this, ctx, state, info, matrix);

	chams->OnDrawModelExecute(ctx, state, info, matrix);
	hooks->ModelRender->GetOriginal<DrawModelExecuteFn>(21)(_this, ctx, state, info, matrix);
	interfaces->ModelRender->ForcedMaterialOverride(nullptr);
}

VmtHook::VmtHook(void* targetClassPointer) {
	this->classPointer = reinterpret_cast<uintptr_t**>(targetClassPointer);

	int tableSize = 0;

	while (reinterpret_cast<uintptr_t*>(*this->classPointer)[tableSize])
		tableSize++;

	originalPointer = *this->classPointer;

	newTablePointer = new uintptr_t[sizeof(uintptr_t) * tableSize];
	memcpy(newTablePointer, originalPointer, sizeof(uintptr_t) * tableSize);
}

VmtHook::~VmtHook() {
	RestoreOldTable();

	delete originalPointer;
	delete newTablePointer;
}

void VmtHook::SwapPointer(size_t index, void* new_function) {
	newTablePointer[index] = reinterpret_cast<uintptr_t>(new_function);
}

void VmtHook::ApplyNewTable() {
	*classPointer = newTablePointer;
}

void VmtHook::RestoreOldTable() {
	*classPointer = originalPointer;
}

template<typename T>
T VmtHook::GetOriginal(size_t index) {
	return reinterpret_cast<T>(originalPointer[index]);
}

Hooks::Hooks(HMODULE module) {
	Utilities::attachConsole();
	printf("[#] 0x%08X -> Module\n", reinterpret_cast<int>(module));

	this->module = module;

	interfaces = std::make_unique<Interfaces>();
	netvars = std::make_unique<Netvars>();
	config = std::make_unique<Config>();
	render = std::make_unique<Render>();
	memory = std::make_unique<Memory>();
	menu = std::make_unique<Menu>();

	this->window = FindWindow(L"Valve001", nullptr);
	this->originalWndProc = WNDPROC(SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(WndProc)));

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	this->Install();
}

void Hooks::Install() {
	ClientInput = new VmtHook(interfaces->ClientMode);
	ClientInput->SwapPointer(24, reinterpret_cast<void*>(CreateMove));
	ClientInput->SwapPointer(44, reinterpret_cast<void*>(PostScreenEffects));
	ClientInput->ApplyNewTable();

	ModelRender = new VmtHook(interfaces->ModelRender);
	ModelRender->SwapPointer(21, reinterpret_cast<void*>(DrawModelExecute));
	ModelRender->ApplyNewTable();

	Surface = new VmtHook(interfaces->Surface);
	Surface->SwapPointer(67, reinterpret_cast<void*>(LockCursor));
	Surface->SwapPointer(116, reinterpret_cast<void*>(OnResolutionChanged));
	Surface->ApplyNewTable();

	VGUI = new VmtHook(interfaces->Panel);
	VGUI->SwapPointer(41, reinterpret_cast<void*>(PaintTraverse));
	VGUI->ApplyNewTable();
}

void Hooks::Restore() {
	ClientInput->RestoreOldTable();
	ModelRender->RestoreOldTable();
	Surface->RestoreOldTable();
	VGUI->RestoreOldTable();

	Glow::Shutdown();

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWndProc));
	HANDLE thread = CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Unload), module, NULL, NULL);

	if (thread) CloseHandle(thread);
}

bool Hooks::IsWindowHooked() {
	return this != nullptr && originalWndProc != nullptr;
}

LRESULT Hooks::ReturnWindowCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	return CallWindowProcW(originalWndProc, window, msg, wParam, lParam);
}