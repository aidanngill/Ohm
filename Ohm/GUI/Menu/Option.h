#pragma once

#include <any>

enum OptionType : unsigned char {
	TYPE_BOOL,
	TYPE_COMBO,
	TYPE_INTEGER,
	TYPE_FLOAT,
	TYPE_COLOR
};

class Option {
public:
	Option(const wchar_t* title, bool* parent);

	const wchar_t* title;
	OptionType type;

	bool* boolean_value;
};