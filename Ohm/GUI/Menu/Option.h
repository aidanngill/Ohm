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
	Option(const wchar_t* title, OptionType type);

	const wchar_t* title;
	OptionType type;
	std::any* value;
};