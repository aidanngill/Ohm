#include "./Interfaces.h"
#include "./Dependencies.h"

#include "../PE/Types.h"
#include "../PE/Module.h"

#include "../SDK/Class/InterfaceReg.h"

uintptr_t FollowJump(uintptr_t address) {
	if (!address)
		return 0;

	uintptr_t displacement = *reinterpret_cast<uintptr_t*>(address);

	if (!displacement)
		return 0;

	return (address + sizeof(uintptr_t)) + displacement;
}

bool GetInterfaceList(HMODULE module, std::vector<InterfaceReg*> &interface_reg_list) {
	uintptr_t create_interface = (uintptr_t)GetProcAddress(module, "CreateInterface");

	if (!create_interface)
		return false;

	if (*reinterpret_cast<uint8_t*>(create_interface + 4) != 0xE9)
		return false;

	create_interface = FollowJump(create_interface + 5);

	if (!create_interface)
		return false;

	InterfaceReg* interface_list = **reinterpret_cast<InterfaceReg***>(create_interface + 6);

	for (InterfaceReg* current = interface_list; current; current = current->m_pNext)
		interface_reg_list.push_back(current);

	return true;
}

std::vector<Module> GetModuleList() {
#if _WIN64
	PEB* peb = reinterpret_cast<TEB*>(__readgsdword(0x18))->ProcessEnvironmentBlock;
#else
	PEB* peb = reinterpret_cast<TEB*>(__readfsdword(0x18))->ProcessEnvironmentBlock;
#endif

	uintptr_t base_address = peb->ImageBaseAddress;

	Module module;
	std::vector<Module> modules;

	if (!peb->Ldr->InMemoryOrderModuleList.Flink)
		return modules;

	LIST_ENTRY* list = &peb->Ldr->InMemoryOrderModuleList;

	for (auto i = list->Flink; i != list; i = i->Flink) {
		LDR_DATA_TABLE_ENTRY* ldr_entry = CONTAINING_RECORD(i, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (!ldr_entry)
			continue;

		if (!module.InitializeModule(ldr_entry))
			continue;

		modules.push_back(module);
	}

	return modules;
}

Interfaces::Interfaces() {
	std::vector<InterfaceReg*> interface_reg_list;

	HMODULE handle;
	InterfaceReg* current = nullptr;

	modules = GetModuleList();

	for (Module module : modules) {
		handle = GetModuleHandleW(module.GetModuleNameW().c_str());

		if (!handle)
			continue;

		if (!GetInterfaceList(handle, interface_reg_list))
			continue;

		for (InterfaceReg* current : interface_reg_list)
			interfaces.push_back(Interface{ module.GetModuleNameA(), current->m_pName, reinterpret_cast<uintptr_t>(current->m_CreateFn()) });
	}

	BaseClient			= reinterpret_cast<IBaseClientDLL*>(FindInterface("VClient0").createFn);
	ClientMode			= **reinterpret_cast<CClientMode***>((*reinterpret_cast<uintptr_t**>(BaseClient))[10] + 5);
	ClientEntityList	= reinterpret_cast<IClientEntityList*>(FindInterface("VClientEntityList0").createFn);
	DebugOverlay		= reinterpret_cast<IVDebugOverlay*>(FindInterface("VDebugOverlay0").createFn);
	Engine				= reinterpret_cast<IVEngineClient*>(FindInterface("VEngineClient0").createFn);
	EngineTrace			= reinterpret_cast<IEngineTrace*>(FindInterface("EngineTraceClient0").createFn);
	InputSystem			= reinterpret_cast<IInputSystem*>(FindInterface("InputSystemVersion0").createFn);
	MaterialSystem		= reinterpret_cast<IMaterialSystem*>(FindInterface("VMaterialSystem0").createFn);
	ModelRender			= reinterpret_cast<IVModelRender*>(FindInterface("VEngineModel0").createFn);
	Panel				= reinterpret_cast<IPanel*>(FindInterface("VGUI_Panel0").createFn);
	Surface				= reinterpret_cast<ISurface*>(FindInterface("VGUI_Surface0").createFn);
}

Interfaces::~Interfaces() {
	this->InputSystem->EnableInput(true);
}

Interface Interfaces::FindInterface(const char* targetName) {
	int len = strlen(targetName);

	for (size_t idx = 0; idx < interfaces.size(); idx++)
		if (strncmp(interfaces[idx].interfaceName, targetName, len) == 0)
			return interfaces[idx];

	return Interface{};
}