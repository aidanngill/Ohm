#pragma once

#include <iostream>
#include <unordered_map>

class IClientClass;
class RecvTable;

intptr_t GetOffset(RecvTable* table, const char* table_name, const char* netvar_name);
intptr_t GetNetvarOffset(const char* table_name, const char* netvar_name, IClientClass* client_class);

class Netvars {
public:
	Netvars();

	std::ptrdiff_t m_Collision;
	std::ptrdiff_t m_fFlags;
	std::ptrdiff_t m_iHealth;
	std::ptrdiff_t m_iTeamNum;
	std::ptrdiff_t m_nTickBase;
	std::ptrdiff_t m_vecOrigin;

	std::ptrdiff_t m_ArmorValue;
	std::ptrdiff_t m_lifeState;

	std::ptrdiff_t m_flC4Blow;
	std::ptrdiff_t m_flTimerLength;

	bool InitializeClassIdentifiers();

	bool ci_initialized = false;
	std::unordered_map<std::string, int> class_identifiers;
private:
	IClientClass* client_class;
};

inline std::unique_ptr<Netvars> netvars;