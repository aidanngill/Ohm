#pragma once

#include <iostream>
#include <unordered_map>

class IClientClass;
class RecvTable;

intptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName);
intptr_t GetNetvarOffset(const char* tableName, const char* netvarName, IClientClass* clientClass);

class Netvars {
public:
	Netvars();

	std::ptrdiff_t m_flNextAttack;

	std::ptrdiff_t m_bIsDefusing;
	std::ptrdiff_t m_flFlashMaxAlpha;

	std::ptrdiff_t m_Collision;
	std::ptrdiff_t m_fFlags;
	std::ptrdiff_t m_iHealth;
	std::ptrdiff_t m_iTeamNum;
	std::ptrdiff_t m_nTickBase;
	std::ptrdiff_t m_vecOrigin;

	std::ptrdiff_t m_ArmorValue;
	std::ptrdiff_t m_lifeState;

	std::ptrdiff_t m_bShouldGlow;

	std::ptrdiff_t m_flC4Blow;
	std::ptrdiff_t m_flTimerLength;

	bool InitializeClassIdentifiers();

	bool classIdentifiersInitialized = false;
	std::unordered_map<std::string, int> classIdentifiers;
private:
	IClientClass* clientClass;
};

inline std::unique_ptr<Netvars> netvars;