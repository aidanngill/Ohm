#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>

class InterfaceReg;
class IBaseClientDLL;
class IPanel;
class ISurface;

class Module;

uintptr_t FollowJump(uintptr_t address);
bool GetInterfaceList(HMODULE module, std::vector<InterfaceReg*>& interface_reg_list);
std::vector<Module> GetModuleList();

class Interface {
public:
	std::string module_name;
	const char* interface_name;
	uintptr_t create_fn;
};

class Interfaces {
public:
	Interfaces();
	~Interfaces();

	IBaseClientDLL* BaseClient;
	IPanel* Panel;
	ISurface* Surface;

	Interface FindInterface(const char* interface_name);
private:
	std::vector<Module> modules;
	std::vector<Interface> interfaces;
};

inline std::unique_ptr<const Interfaces> interfaces;