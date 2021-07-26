#pragma once

#include <iostream>
#include <vector>

#include <Windows.h>

#include "./Tab.h"

class Menu {
public:
	Menu();

	void Render();
	void Controls(UINT msg, WPARAM wParam, LPARAM lParam);
	
	bool IsClosing();
	bool HoveringTab(int &result);

	bool is_open = true;
	bool is_clicking = false;
	bool is_dragging = false;
private:
	int offset_x = 50, offset_y = 50;
	int old_offset_x, old_offset_y;

	int mouse_x, mouse_y;
	int previous_mouse_x, previous_mouse_y;

	int current_tab = 0;
	std::vector<Tab> tabs;
};

inline std::unique_ptr<Menu> menu;