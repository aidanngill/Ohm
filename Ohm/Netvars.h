#pragma once

#include <iostream>

class IClientClass;
class RecvTable;

intptr_t GetOffset(RecvTable* table, const char* table_name, const char* netvar_name);
intptr_t GetNetvarOffset(const char* table_name, const char* netvar_name, IClientClass* client_class);

class Netvars {
public:
	Netvars();

	std::ptrdiff_t m_iHealth;
	std::ptrdiff_t m_vecOrigin;
	std::ptrdiff_t m_Collision;
	std::ptrdiff_t m_lifeState;
};

inline std::unique_ptr<Netvars> netvars;