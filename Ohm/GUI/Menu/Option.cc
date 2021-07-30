#include "./Option.h"

Option::Option(const wchar_t* title, bool* parent) {
	this->title = title;
	this->type = this->TYPE_BOOL;
	this->boolValue = parent;
}

Option::Option(const wchar_t* title, int* parent) {
	this->title = title;
	this->type = this->TYPE_INTEGER;
	this->intValue = parent;
}

Option::Option(const wchar_t* title, int* parent, std::vector<std::wstring> options) {
	this->title = title;
	this->type = this->TYPE_COMBO;
	this->intValue = parent;
	this->stringValues = options;
}

const wchar_t* Option::CurrentString() {
	if (this->type != Option::TYPE_COMBO)
		return L"";

	return this->stringValues[*this->intValue].c_str();
}