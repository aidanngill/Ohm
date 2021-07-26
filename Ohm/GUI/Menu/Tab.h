#pragma once

#include <vector>
#include <string>

#include "./Option.h"

class Tab {
public:
	Tab(const wchar_t* tab_title);
	void Draw();

	const wchar_t* title;
	std::vector<Option> options;
};