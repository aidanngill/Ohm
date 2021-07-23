#include "./Menu.h"
#include "./Render.h"

#include "../Interfaces.h"
#include "../InterfaceDep.h"

#define MENU_WIDTH 300
#define MENU_HEIGHT 200

#define TITLEBAR_WIDTH MENU_WIDTH
#define TITLEBAR_HEIGHT 20

bool IsInRegion(int x, int y, int tx, int ty, int tw, int th) {
	return (tx <= x && x <= tx + tw) && (ty <= y && y <= ty + th);
}

Menu::Menu() {}

void Menu::Render() {
	if (!is_open)
		return;

	// Check if we're dragging.
	interfaces->InputSystem->GetCursorPosition(&mouse_x, &mouse_y);

	if (is_dragging) {
		if (is_clicking) {
			offset_x = (mouse_x - previous_mouse_x) + old_offset_x;
			offset_y = (mouse_y - previous_mouse_y) + old_offset_y;
		} else {
			previous_mouse_x = mouse_x;
			previous_mouse_y = mouse_y;

			old_offset_x = offset_x;
			old_offset_y = offset_y;

			is_dragging = false;
		}
	} else {
		is_dragging = IsInRegion(mouse_x, mouse_y, offset_x, offset_y, TITLEBAR_WIDTH, TITLEBAR_HEIGHT);
	}

	// Background [full]
	interfaces->Surface->DrawSetColor(Color(32, 32, 32, 255));
	interfaces->Surface->DrawFilledRect(offset_x, offset_y, offset_x + MENU_WIDTH, offset_y + MENU_HEIGHT);

	// Background [outline]
	interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));
	interfaces->Surface->DrawOutlinedRect(offset_x, offset_y, offset_x + MENU_WIDTH, offset_y + MENU_HEIGHT);

	// Titlebar [outline]
	interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));
	interfaces->Surface->DrawOutlinedRect(offset_x, offset_y, offset_x + TITLEBAR_WIDTH, offset_y + TITLEBAR_HEIGHT);

	// Titlebar [text]
	render->Text(L"Ohm v0.1.0", offset_x + 6, offset_y + 6, render->font_base, Color(255, 255, 255, 255));

	// Titlebar [close button]
	interfaces->Surface->DrawSetColor(Color(255, 0, 0, 255));
	interfaces->Surface->DrawFilledRect(offset_x + TITLEBAR_WIDTH - 20, offset_y, offset_x + TITLEBAR_WIDTH, offset_y + 20);

	// Titlebar [close button outline]
	interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));
	interfaces->Surface->DrawOutlinedRect(offset_x + TITLEBAR_WIDTH - 20, offset_y, offset_x + TITLEBAR_WIDTH, offset_y + 20);
}

void Menu::Controls(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		this->is_clicking = true;
		break;
	case WM_LBUTTONUP:
		this->is_clicking = false;
		break;
	default:
		break;
	}
}