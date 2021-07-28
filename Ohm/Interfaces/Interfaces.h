#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>

class CClientMode;
class InterfaceReg;
class IBaseClientDLL;
class IClientEntityList;
class IInputSystem;
class IPanel;
class ISurface;
class IVDebugOverlay;
class IVEngineClient;

class Module;

uintptr_t FollowJump(uintptr_t address);
bool GetInterfaceList(HMODULE module, std::vector<InterfaceReg*>& interfaceRegList);
std::vector<Module> GetModuleList();

class Interface {
public:
	std::string moduleName;
	const char* interfaceName;
	uintptr_t createFn;
};

class Interfaces {
public:
	Interfaces();
	~Interfaces();

	CClientMode* ClientMode;
	IBaseClientDLL* BaseClient;
	IClientEntityList* ClientEntityList;
	IInputSystem* InputSystem;
	IPanel* Panel;
	ISurface* Surface;
	IVDebugOverlay* DebugOverlay;
	IVEngineClient* Engine;

	Interface FindInterface(const char* interfaceName);
private:
	std::vector<Module> modules;
	std::vector<Interface> interfaces;
};

inline std::unique_ptr<const Interfaces> interfaces;