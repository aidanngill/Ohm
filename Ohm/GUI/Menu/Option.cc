#include "./Option.h"

Option::Option(const wchar_t* title, void* parent) {
	this->title = title;
	this->type = this->TYPE_BUTTON;
	this->funcValue = parent;
}

Option::Option(const wchar_t* title, bool* parent) {
	this->title = title;
	this->type = this->TYPE_BOOL;
	this->boolValue = parent;
}

Option::Option(const wchar_t* title, int* parent, int min, int max) {
	this->title = title;
	this->type = this->TYPE_INTEGER;

	this->intValue = parent;
	this->intMin = min;
	this->intMax = max;
}

Option::Option(const wchar_t* title, float* parent, float min, float max) {
	this->title = title;
	this->type = this->TYPE_FLOAT;

	this->fltValue = parent;
	this->fltMin = min;
	this->fltMax = max;
}

Option::Option(const wchar_t* title, int* parent, std::vector<std::wstring> options) {
	this->title = title;
	this->type = this->TYPE_COMBO;

	this->intValue = parent;
	this->intMin = 0;
	this->intMax = options.size();

	this->stringValues = options;
}

const wchar_t* Option::CurrentString() {
	if (this->type != Option::TYPE_COMBO)
		return L"";

	return this->stringValues[*this->intValue].c_str();
}

void Option::ExecuteFunction() {
	if (this->type != Option::TYPE_BUTTON)
		return;

	reinterpret_cast<void(*)()>(funcValue)();
}
