#include <iostream>
#include <vector>
#include <Windows.h>

#include "./Utilities.h"

#include "../Memory.h"

FILE* f;

void Utilities::attachConsole() {
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
}

void Utilities::detachConsole() {
	if (f) fclose(f);
	FreeConsole();
}

uint8_t* Utilities::patternScan(void* module, const char* signature) {
	static auto pattern_to_bytes = [](const char* pattern) {
		auto bytes = std::vector<int>{};

		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			} else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}

		return bytes;
	};

	auto dos_header = (PIMAGE_DOS_HEADER)module;
	auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dos_header->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_bytes(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;

		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}

		if (found) return &scan_bytes[i];
	}

	return nullptr;
}

// Local player must be de-referenced once more to get the *current* local player.
CBasePlayer* Utilities::getLocalPlayer() {
	return *reinterpret_cast<CBasePlayer**>(memory->LocalPlayer);
}