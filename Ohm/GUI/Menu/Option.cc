#include "./Option.h"

Option::Option(const wchar_t* title, bool* parent) {
	this->title = title;
	this->type = this->TYPE_BOOL;
	this->boolValue = parent;
}