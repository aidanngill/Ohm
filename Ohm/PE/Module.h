#pragma once

#include "Types.h"

class Module {
public:
	bool GetFileHeaders(uintptr_t base, PIMAGE_DOS_HEADER& out_dos, PIMAGE_NT_HEADERS& out_nt);
	bool InitializeModule(LDR_DATA_TABLE_ENTRY* ldr_entry);
	std::wstring GetModuleNameW();
	std::string GetModuleNameA();
private:
	LDR_DATA_TABLE_ENTRY* m_ldr_entry;
	IMAGE_DOS_HEADER* m_dos;
	IMAGE_NT_HEADERS* m_nt;
	uintptr_t m_base;
	std::wstring m_pathW;
};