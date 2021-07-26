#include "./Menu.h"

#include "../Render.h"

#include "../../Interfaces.h"
#include "../../InterfaceDep.h"

#define MENU_WIDTH 300
#define MENU_HEIGHT 200

#define TITLEBAR_WIDTH MENU_WIDTH
#define TITLEBAR_HEIGHT 20

#define BUTTON_HEIGHT TITLEBAR_HEIGHT
#define BUTTON_WIDTH BUTTON_HEIGHT

#define TAB_WIDTH 60
#define TAB_HEIGHT TITLEBAR_HEIGHT
#define TAB_WIDTH_EXTRA 10

bool IsInRegion(int x, int y, int tx, int ty, int tw, int th) {
	return (tx <= x && x <= tx + tw) && (ty <= y && y <= ty + th);
}

Menu::Menu() {
	Tab tab_aim = Tab(L"Aim");

	Tab tab_visual = Tab(L"Visuals");

	Tab tab_misc = Tab(L"Misc");

	// Miscellaneous
	std::any misc_bhop_value = false;
	Option misc_bhop = Option(L"Bunny Hop", TYPE_BOOL);
	misc_bhop.value = &misc_bhop_value;

	tab_misc.options.push_back(misc_bhop);

	tabs.push_back(tab_aim);
	tabs.push_back(tab_visual);
	tabs.push_back(tab_misc);
}

void Menu::Render() {
	if (!is_open)
		return;

	interfaces->InputSystem->GetCursorPosition(&mouse_x, &mouse_y);

	if (IsClosing()) {
		is_open = false;
		return;
	}

	if (is_dragging) {
		if (is_clicking) {
			offset_x = (mouse_x - previous_mouse_x) + old_offset_x;
			offset_y = (mouse_y - previous_mouse_y) + old_offset_y;
		}
		else {
			previous_mouse_x = mouse_x;
			previous_mouse_y = mouse_y;

			old_offset_x = offset_x;
			old_offset_y = offset_y;

			is_dragging = false;
		}
	}
	else {
		is_dragging = IsInRegion(mouse_x, mouse_y, offset_x, offset_y, TITLEBAR_WIDTH, TITLEBAR_HEIGHT);
	}

	// Background [full]
	interfaces->Surface->DrawSetColor(Color(32, 32, 32, 255));
	interfaces->Surface->DrawFilledRect(offset_x, offset_y, offset_x + MENU_WIDTH, offset_y + MENU_HEIGHT);

	// Titlebar [text]
	render->Text(L"Ohm v0.1.0", offset_x + 6, offset_y + 6, render->font_base, Color(255, 255, 255, 255));

	// Titlebar [close button]
	interfaces->Surface->DrawSetColor(Color(255, 0, 0, 255));
	interfaces->Surface->DrawFilledRect(offset_x + TITLEBAR_WIDTH - 20, offset_y, offset_x + TITLEBAR_WIDTH, offset_y + 20);
	
	// Draw the tabs and their titles.
	int hovered_tab = -1;

	if (this->HoveringTab(hovered_tab) && is_clicking && !is_dragging)
		current_tab = hovered_tab;

	int sx = offset_x;
	int sy = offset_y + TITLEBAR_HEIGHT;
	int ex = offset_x + TAB_WIDTH;
	int ey = sy + TAB_HEIGHT;

	for (size_t i = 0; i < tabs.size(); i++) {
		Color tab_bg_color = Color(48, 48, 48, 255);
		Color tab_side_color = tab_bg_color;

		if (i == current_tab) {
			tab_bg_color = Color(86, 86, 86, 255);
			tab_side_color = Color(0, 84, 255, 255);
		}
		else if (i == hovered_tab) {
			tab_bg_color = Color(67, 67, 67, 255);
		}

		interfaces->Surface->DrawSetColor(tab_bg_color);
		interfaces->Surface->DrawFilledRect(sx, sy, ex, ey);

		interfaces->Surface->DrawSetColor(tab_side_color);
		interfaces->Surface->DrawFilledRect(ex, sy, ex + TAB_WIDTH_EXTRA, ey);

		render->Text(tabs[i].title, offset_x + 6, sy + 6, render->font_base, Color(255, 255, 255, 255));

		// (sy - 1) to fix https://cdn.discordapp.com/attachments/599271375043297282/869143609528696832/unknown.png
		interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));
		interfaces->Surface->DrawLine(ex, sy - 1, ex, ey);

		// Skip the horizontal line on the first tab.
		if (i > 0)
			interfaces->Surface->DrawLine(sx, sy, ex + TAB_WIDTH_EXTRA, sy);

		sy += TAB_HEIGHT;
		ey += TAB_HEIGHT;
	}

	interfaces->Surface->DrawLine(sx, sy, ex + TAB_WIDTH_EXTRA, sy);

	// Draw options within the tab.
	std::vector<Option>* options = &tabs[current_tab].options;

	for (int i = 0; i < options->size(); i++) {}

	// Do all the outlines at the end.
	interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));

	// Background [outline]
	interfaces->Surface->DrawOutlinedRect(offset_x, offset_y, offset_x + MENU_WIDTH, offset_y + MENU_HEIGHT);

	// Titlebar [outline]
	interfaces->Surface->DrawOutlinedRect(offset_x, offset_y, offset_x + TITLEBAR_WIDTH, offset_y + TITLEBAR_HEIGHT);

	// Titlebar [close button outline]
	interfaces->Surface->DrawOutlinedRect(offset_x + TITLEBAR_WIDTH - BUTTON_WIDTH, offset_y, offset_x + TITLEBAR_WIDTH, offset_y + BUTTON_HEIGHT);

	// Tab separator (vertical)
	interfaces->Surface->DrawLine(offset_x + TAB_WIDTH + TAB_WIDTH_EXTRA, offset_y + TITLEBAR_HEIGHT, offset_x + TAB_WIDTH + TAB_WIDTH_EXTRA, offset_y + TITLEBAR_HEIGHT + MENU_HEIGHT - TITLEBAR_HEIGHT);
}

void Menu::Controls(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		is_clicking = true;
		break;
	case WM_LBUTTONUP:
		is_clicking = false;
		break;
	default:
		break;
	}
}

// Check if the user is pressing the 'close' button at the top right.
bool Menu::IsClosing() {
	return is_clicking &&
		mouse_x == previous_mouse_x &&
		mouse_y == previous_mouse_y &&
		IsInRegion(
			mouse_x, mouse_y,
			offset_x + TITLEBAR_WIDTH - BUTTON_HEIGHT, offset_y,
			BUTTON_WIDTH, BUTTON_HEIGHT
		);
}

bool Menu::HoveringTab(int& result) {
	if (!(offset_x <= mouse_x && mouse_x <= offset_x + (TAB_WIDTH + TAB_WIDTH_EXTRA)))
		return false;

	int y_min = offset_y + TITLEBAR_HEIGHT;
	int y_max = offset_y + TITLEBAR_HEIGHT + (TAB_HEIGHT * static_cast<int>(tabs.size()));

	if (!(y_min <= mouse_y && mouse_y <= y_max))
		return false;

	result = static_cast<int>(std::floor((mouse_y - offset_y) / TAB_HEIGHT)) - 1;

	return true;
}