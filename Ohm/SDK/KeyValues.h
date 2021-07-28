#pragma once

class KeyValues {
public:
	static KeyValues* FromString(const char* name, const char* value);
	KeyValues* FindKey(const char* keyName, bool create);
	void SetString(const char* keyName, const char* value);
};