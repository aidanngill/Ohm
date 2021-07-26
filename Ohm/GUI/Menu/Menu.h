#pragma once

#include <iostream>
#include <vector>

#include <Windows.h>

#include "./Tab.h"

#define MENU_WIDTH 300
#define MENU_HEIGHT 200

#define TITLEBAR_WIDTH MENU_WIDTH
#define TITLEBAR_HEIGHT 20

#define BUTTON_HEIGHT TITLEBAR_HEIGHT
#define BUTTON_WIDTH BUTTON_HEIGHT

#define TAB_WIDTH 60
#define TAB_HEIGHT TITLEBAR_HEIGHT
#define TAB_WIDTH_EXTRA 10

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

	int mouse_x, mouse_y;
	int offset_x = 50, offset_y = 50;
private:
	int old_offset_x, old_offset_y;
	int previous_mouse_x, previous_mouse_y;

	int current_tab = 0;
	std::vector<Tab> tabs;
};

inline std::unique_ptr<Menu> menu;