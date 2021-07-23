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

Netvars::Netvars() {
	IClientClass* client_class = interfaces->BaseClient->GetAllClasses();

	// DT_BasePlayer
	m_iHealth = GetNetvarOffset("DT_BasePlayer", "m_iHealth", client_class);
	m_vecOrigin = GetNetvarOffset("DT_BasePlayer", "m_vecOrigin", client_class);
	m_Collision = GetNetvarOffset("DT_BasePlayer", "m_Collision", client_class);
	
	// DT_CSPlayer
	m_lifeState = GetNetvarOffset("DT_CSPlayer", "m_lifeState", client_class);
}