#include "./Option.h"

Option::Option(const wchar_t* title, OptionType type) {
	this->title = title;
	this->type = type;
}