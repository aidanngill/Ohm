#pragma once

#include <iostream>
#include <vector>

class IPanel;
class ISurface;

class Module;

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

	bool initialized = false;

	IPanel* Panel;
	ISurface* Surface;

	Interface FindInterface(std::string interface_name);
private:
	std::vector<Module> modules;
	std::vector<Interface> interfaces;
};

inline std::unique_ptr<const Interfaces> interfaces;