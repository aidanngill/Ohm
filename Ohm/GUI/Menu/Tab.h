#pragma once

#include <vector>
#include <string>

#include "./Option.h"

#define OPTION_SEPARATION 5
#define BUTTON_INDENT 2

#define SUBTAB_HEIGHT TITLEBAR_HEIGHT
#define SUBTAB_PAD (TITLEBAR_HEIGHT / 2)

class GenericTab {
public:
	void Draw() {};
	void DrawOptions();
	void DrawComboContent();

	const wchar_t* title;
	bool isSubTab = false;

	std::vector<Option> options;
	int currentOpened = -1;
	int hoveredComboItem = -1;
};

class SubTab : public GenericTab {
public:
	SubTab(const wchar_t* subTabTitle);
	void Draw();
};

class Tab : public GenericTab {
public:
	Tab(const wchar_t* tabTitle);
	void Draw();

	std::vector<SubTab> subTabs;
private:
	int currentSubTab = 0;
};