#pragma once

#include <iostream>
#include <chrono>
#include <vector>

class Option {
public:
	enum : unsigned char {
		TYPE_BOOL,
		TYPE_COMBO,
		TYPE_INTEGER,
		TYPE_FLOAT,
		TYPE_COLOR,
		TYPE_BUTTON
	};

	Option(const wchar_t* title, bool* parent);
	Option(const wchar_t* title, void* parent);
	Option(const wchar_t* title, int* parent);
	Option(const wchar_t* title, int* parent, std::vector<std::wstring> options);

	const wchar_t* CurrentString();
	void ExecuteFunction();

	const wchar_t* title;
	unsigned char type;

	bool* boolValue;
	int* intValue;

	std::vector<std::wstring> stringValues;

	void* funcValue;
};