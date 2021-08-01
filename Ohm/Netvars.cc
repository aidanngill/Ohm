#include <Windows.h>

#include "./Netvars.h"

#include "./Interfaces/Interfaces.h"

#include "./SDK/Interfaces/IBaseClientDLL.h"

intptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName) {
	for (int i = 0; i < table->m_nProps; i++) {
		RecvProp prop = table->m_pProps[i];

		if (!_stricmp(prop.m_pVarName, netvarName))
			return prop.m_Offset;

		if (prop.m_pDataTable) {
			intptr_t offset = GetOffset(prop.m_pDataTable, tableName, netvarName);
			if (offset) return offset + prop.m_Offset;
		}
	}

	return 0;
}

intptr_t GetNetvarOffset(const char* tableName, const char* netvarName, IClientClass* clientClass) {
	IClientClass* currentNode = clientClass;

	for (auto currentNode = clientClass; currentNode; currentNode = currentNode->m_pNext)
		if (!_stricmp(tableName, currentNode->m_pRecvTable->m_pNetTableName))
			return GetOffset(currentNode->m_pRecvTable, tableName, netvarName);

	return 0;
}

bool Netvars::InitializeClassIdentifiers() {
	IClientClass* currentNode = clientClass;
	int probableValidCount = 0;

	for (auto currentNode = clientClass; currentNode; currentNode = currentNode->m_pNext) {
		if (currentNode->m_ClassID > 0)
			++probableValidCount;

		classIdentifiers[currentNode->m_pNetworkName] = currentNode->m_ClassID;
	}

	if (probableValidCount < 1)
		return false;

	printf("[+] Initialized %d class identifiers.\n", classIdentifiers.size());

	return true;
}

Netvars::Netvars() {
	clientClass = interfaces->BaseClient->GetAllClasses();

	// DT_BaseCombatCharacter
	m_flNextAttack = GetNetvarOffset("DT_BaseCombatCharacter", "m_flNextAttack", clientClass);

	// DT_BaseCombatWeapon
	m_flNextPrimaryAttack = GetNetvarOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", clientClass);
	m_iClip = GetNetvarOffset("DT_BaseCombatWeapon", "m_iClip1", clientClass);

	// DT_BasePlayer
	m_Collision = GetNetvarOffset("DT_BasePlayer", "m_Collision", clientClass);
	m_fFlags = GetNetvarOffset("DT_BasePlayer", "m_fFlags", clientClass);
	m_iHealth = GetNetvarOffset("DT_BasePlayer", "m_iHealth", clientClass);
	m_iTeamNum = GetNetvarOffset("DT_BasePlayer", "m_iTeamNum", clientClass);
	m_nTickBase = GetNetvarOffset("DT_BasePlayer", "m_nTickBase", clientClass);
	m_vecOrigin = GetNetvarOffset("DT_BasePlayer", "m_vecOrigin", clientClass);
	
	// DT_CSPlayer
	m_ArmorValue = GetNetvarOffset("DT_CSPlayer", "m_ArmorValue", clientClass);
	m_bGunGameImmunity = GetNetvarOffset("DT_CSPlayer", "m_bGunGameImmunity", clientClass);
	m_bIsDefusing = GetNetvarOffset("DT_CSPlayer", "m_bIsDefusing", clientClass);
	m_bIsScoped = GetNetvarOffset("DT_CSPlayer", "m_bIsScoped", clientClass);
	m_flFlashMaxAlpha = GetNetvarOffset("DT_CSPlayer", "m_flFlashMaxAlpha", clientClass) - 8;
	m_lifeState = GetNetvarOffset("DT_CSPlayer", "m_lifeState", clientClass);

	// DT_DynamicProp
	m_bShouldGlow = GetNetvarOffset("DT_DynamicProp", "m_bShouldGlow", clientClass);

	// DT_PlantedC4
	m_flC4Blow = GetNetvarOffset("DT_PlantedC4", "m_flC4Blow", clientClass);
	m_flTimerLength = GetNetvarOffset("DT_PlantedC4", "m_flTimerLength", clientClass);
}
