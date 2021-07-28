#include "./KeyValues.h"

#include "../Memory.h"

KeyValues* KeyValues::FromString(const char* name, const char* value) {
	const auto keyValuesFromString = memory->KeyValuesFromString;
	KeyValues* keyValues;

	__asm {
		push 0
		mov edx, value
		mov ecx, name
		call keyValuesFromString
		add esp, 4
		mov keyValues, eax
	}

	return keyValues;
}

KeyValues* KeyValues::FindKey(const char* keyName, bool create) {
	return memory->KeyValuesFindKey(this, keyName, create);
}

void KeyValues::SetString(const char* keyName, const char* value) {
	if (const auto key = FindKey(keyName, true))
		memory->KeyValuesSetString(key, value);
}