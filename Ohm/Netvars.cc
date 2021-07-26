#include <Windows.h>

#include "./Netvars.h"
#include "./Interfaces.h"

#include "./SDK/IBaseClientDLL.h"

intptr_t GetOffset(RecvTable* table, const char* table_name, const char* netvar_name) {
	for (int i = 0; i < table->m_nProps; i++) {
		RecvProp prop = table->m_pProps[i];

		if (!_stricmp(prop.m_pVarName, netvar_name))
			return prop.m_Offset;

		if (prop.m_pDataTable) {
			intptr_t offset = GetOffset(prop.m_pDataTable, table_name, netvar_name);
			if (offset) return offset + prop.m_Offset;
		}
	}

	return 0;
}

intptr_t GetNetvarOffset(const char* table_name, const char* netvar_name, IClientClass* client_class) {
	IClientClass* current_node = client_class;

	for (auto current_node = client_class; current_node; current_node = current_node->m_pNext)
		if (!_stricmp(table_name, current_node->m_pRecvTable->m_pNetTableName))
			return GetOffset(current_node->m_pRecvTable, table_name, netvar_name);

	return 0;
}

bool Netvars::InitializeClassIdentifiers() {
	IClientClass* current_node = client_class;
	int probable_valid_count = 0;

	for (auto current_node = client_class; current_node; current_node = current_node->m_pNext) {
		if (current_node->m_ClassID > 0)
			++probable_valid_count;

		class_identifiers[current_node->m_pNetworkName] = current_node->m_ClassID;
	}

	if (probable_valid_count < 1)
		return false;

	printf("[+] Initialized %d class identifiers.\n", class_identifiers.size());

	return true;
}

Netvars::Netvars() {
	client_class = interfaces->BaseClient->GetAllClasses();

	// DT_BasePlayer
	m_Collision = GetNetvarOffset("DT_BasePlayer", "m_Collision", client_class);
	m_fFlags = GetNetvarOffset("DT_BasePlayer", "m_fFlags", client_class);
	m_iHealth = GetNetvarOffset("DT_BasePlayer", "m_iHealth", client_class);
	m_iTeamNum = GetNetvarOffset("DT_BasePlayer", "m_iTeamNum", client_class);
	m_nTickBase = GetNetvarOffset("DT_BasePlayer", "m_nTickBase", client_class);
	m_vecOrigin = GetNetvarOffset("DT_BasePlayer", "m_vecOrigin", client_class);
	
	// DT_CSPlayer
	m_ArmorValue = GetNetvarOffset("DT_CSPlayer", "m_ArmorValue", client_class);
	m_lifeState = GetNetvarOffset("DT_CSPlayer", "m_lifeState", client_class);

	// DT_PlantedC4
	m_flC4Blow = GetNetvarOffset("DT_PlantedC4", "m_flC4Blow", client_class);
	m_flTimerLength = GetNetvarOffset("DT_PlantedC4", "m_flTimerLength", client_class);
}
