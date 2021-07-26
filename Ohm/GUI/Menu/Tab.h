#pragma once

#include <vector>
#include <string>

class Tab {
public:
	Tab(std::wstring tab_title);
	void Draw();

	std::wstring title;
};