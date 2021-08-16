#pragma once

#include <iostream>
#include <unordered_map>

class IClientClass;
class RecvTable;

intptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName);
intptr_t GetNetvarOffset(const char* tableName, const char* netvarName, IClientClass* clientClass);

class ClassIdentifiers {
public:
	ClassIdentifiers() {};
	bool Initialize();

	bool isInitialized = false;

	int CBaseAnimating = -1;
	int CChicken = -1;
	int CCSPlayer = -1;
	int CPlantedC4 = -1;
private:
	IClientClass* clientClass = nullptr;
	std::unordered_map<std::string, int> classIdentifiers;
};

class Netvars {
public:
	Netvars();

	// DT_BaseCombatCharacter
	std::ptrdiff_t m_flNextAttack;

	// DT_BaseCombatWeapon
	std::ptrdiff_t m_flNextPrimaryAttack;
	std::ptrdiff_t m_iClip;

	// DT_BaseEntity
	std::ptrdiff_t m_hOwnerEntity;

	// DT_BasePlayer
	std::ptrdiff_t m_Collision;
	std::ptrdiff_t m_fFlags;
	std::ptrdiff_t m_iHealth;
	std::ptrdiff_t m_iTeamNum;
	std::ptrdiff_t m_nTickBase;
	std::ptrdiff_t m_vecOrigin;

	// DT_CSPlayer
	std::ptrdiff_t m_ArmorValue;
	std::ptrdiff_t m_bGunGameImmunity;
	std::ptrdiff_t m_bIsDefusing;
	std::ptrdiff_t m_bIsScoped;
	std::ptrdiff_t m_flFlashMaxAlpha;
	std::ptrdiff_t m_lifeState;

	// DT_DynamicProp
	std::ptrdiff_t m_bShouldGlow;

	// DT_PlantedC4
	std::ptrdiff_t m_flC4Blow;
	std::ptrdiff_t m_flTimerLength;

	ClassIdentifiers classIdentifiers;
private:
	IClientClass* clientClass;
};

inline std::unique_ptr<Netvars> netvars;