#include <algorithm>

#include "Module.h"

bool Module::GetFileHeaders(uintptr_t base, PIMAGE_DOS_HEADER& out_dos, PIMAGE_NT_HEADERS& out_nt) {
	IMAGE_DOS_HEADER* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);

	if (!dos || dos->e_magic != IMAGE_DOS_SIGNATURE)
		return false;

	IMAGE_NT_HEADERS* nt = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);

	if (!nt || nt->Signature != IMAGE_NT_SIGNATURE)
		return false;

	out_dos = dos;
	out_nt = nt;

	return true;
}

bool Module::InitializeModule(LDR_DATA_TABLE_ENTRY* ldr_entry) {
	IMAGE_DOS_HEADER* dos;
	IMAGE_NT_HEADERS* nt;

	if (!ldr_entry)
		return false;

	uintptr_t base = ldr_entry->DllBase;

	if (!GetFileHeaders(base, dos, nt))
		return false;

	m_ldr_entry = ldr_entry;
	m_dos = dos;
	m_nt = nt;
	m_base = base;
	m_pathW = ldr_entry->FullDllName.Buffer;

	std::transform(m_pathW.begin(), m_pathW.end(), m_pathW.begin(), tolower);

	return true;
}

std::wstring Module::GetModuleNameW() {
	if (m_pathW.empty())
		return {};

	std::wstring out = m_pathW;

	size_t delim = out.find_last_of('\\');

	if (delim == std::wstring::npos)
		return {};

	return out.substr(delim + 1);
}

std::string Module::GetModuleNameA() {
	std::wstring name = GetModuleNameW();

	int str_len = WideCharToMultiByte(CP_UTF8, 0, name.data(), static_cast<int>(name.size()), nullptr, 0, nullptr, nullptr);

	std::string out;
	out.resize(str_len);

	WideCharToMultiByte(CP_UTF8, 0, name.data(), static_cast<int>(name.size()), &out[0], str_len, nullptr, nullptr);

	return out;
}