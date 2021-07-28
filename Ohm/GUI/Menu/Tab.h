#pragma once

#include <vector>
#include <string>

#include "./Option.h"

#define OPTION_SEPARATION 5
#define BUTTON_INDENT 2

#define SUBTAB_HEIGHT TITLEBAR_HEIGHT
#define SUBTAB_PAD (TITLEBAR_HEIGHT / 2)

void DrawOptions(std::vector<Option> options, bool isSubTab);

class SubTab {
public:
	SubTab(const wchar_t* subTabTitle);
	void Draw();

	const wchar_t* title;

	std::vector<Option> options;
};

class Tab {
public:
	Tab(const wchar_t* tabTitle);
	void Draw();

	const wchar_t* title;

	std::vector<Option> options;
	std::vector<SubTab> subTabs;
private:
	int currentSubTab = 0;
};