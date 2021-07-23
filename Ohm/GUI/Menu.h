#pragma once

#include <iostream>
#include <Windows.h>

class Menu {
public:
	Menu();
	void Render();
	void Controls(UINT msg, WPARAM wParam, LPARAM lParam);

	bool is_open = true;
	bool is_clicking = false;
private:
	int x = 50, y = 50;
};

inline std::unique_ptr<Menu> menu;