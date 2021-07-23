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
	bool is_dragging = false;
private:
	int offset_x = 50, offset_y = 50;
	int mouse_x, mouse_y;
	int previous_mouse_x, previous_mouse_y;
	int old_offset_x, old_offset_y;
};

inline std::unique_ptr<Menu> menu;