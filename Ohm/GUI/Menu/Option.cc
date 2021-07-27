#include "./Option.h"

Option::Option(const wchar_t* title, bool* parent) {
	this->title = title;
	this->type = TYPE_BOOL;
	this->boolean_value = parent;
}