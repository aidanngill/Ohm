#pragma once

#include <iostream>
#include <vector>

#include <Windows.h>

#include "./Tab.h"

#define MENU_WIDTH 300
#define MENU_HEIGHT 300

#define TITLEBAR_WIDTH MENU_WIDTH
#define TITLEBAR_HEIGHT 20

#define BUTTON_HEIGHT TITLEBAR_HEIGHT
#define BUTTON_WIDTH BUTTON_HEIGHT

#define TAB_WIDTH 60
#define TAB_WIDTH_EXTRA 10
#define TAB_HEIGHT TITLEBAR_HEIGHT

class Menu {
public:
	Menu();

	void Render();
	void Controls(UINT msg, WPARAM wParam, LPARAM lParam);
	
	bool IsClosing();
	bool HoveringTab(int &tabHovered);
	bool HoveringSubtab(int tabCount, int& tabHovered);

	bool isOpen = false;
	bool isClicking = false;
	bool isDragging = false;

	int mouseX, mouseY;
	int offsetX = 50, offsetY = 50;
private:
	int oldOffsetX, oldOffsetY;
	int previousMouseX, previousMouseY;

	int currentTab = 0;
	std::vector<Tab> tabs;
};

inline std::unique_ptr<Menu> menu;