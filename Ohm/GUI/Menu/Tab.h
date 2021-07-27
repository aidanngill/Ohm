#pragma once

#include <vector>
#include <string>

#include "./Option.h"

#define OPTION_SEPARATION 5
#define BUTTON_INDENT 2

#define SUBTAB_HEIGHT TITLEBAR_HEIGHT
#define SUBTAB_PAD (TITLEBAR_HEIGHT / 2)

class Subtab {
public:
	Subtab(const wchar_t* subtab_title);
	void Draw();

	const wchar_t* title;

	std::vector<Option> options;
};

class Tab {
public:
	Tab(const wchar_t* tab_title);
	void Draw();

	const wchar_t* title;

	std::vector<Option> options;
	std::vector<Subtab> subtabs;
private:
	int current_subtab = 0;
};