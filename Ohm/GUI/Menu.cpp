#include "./Menu.h"
#include "./Render.h"

#include "../Interfaces.h"
#include "../InterfaceDep.h"

Menu::Menu() {}

void Menu::Render() {
	if (!this->is_open)
		return;

	// Background [full]
	interfaces->Surface->DrawSetColor(Color(32, 32, 32, 255));
	interfaces->Surface->DrawFilledRect(this->x, this->y, this->x + 300, this->y + 200);

	// Background [outline]
	interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));
	interfaces->Surface->DrawOutlinedRect(this->x, this->y, this->x + 300, this->y + 200);

	// Titlebar [outline]
	interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));
	interfaces->Surface->DrawOutlinedRect(this->x, this->y, this->x + 300, this->y + 20);

	// Titlebar [text]
	render->Text(L"Ohm v0.1.0", this->x + 6, this->y + 6, render->font_base, Color(255, 255, 255, 255));
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