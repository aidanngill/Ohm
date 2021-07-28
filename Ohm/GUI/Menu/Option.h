#pragma once

class Option {
public:
	enum : unsigned char {
		TYPE_BOOL,
		TYPE_COMBO,
		TYPE_INTEGER,
		TYPE_FLOAT,
		TYPE_COLOR
	};

	Option(const wchar_t* title, bool* parent);

	const wchar_t* title;
	unsigned char type;

	bool* boolValue;
};