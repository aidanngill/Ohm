#include <algorithm>

#include "./Tab.h"
#include "./Menu.h"

#include "../Colors.h"
#include "../Render.h"

#include "../../Interfaces/Interfaces.h"
#include "../../Interfaces/Dependencies.h"

#include "../../Utility/Utilities.h"

void DrawOptions(std::vector<Option> options, bool subtab = false) {
	int x_offset = menu->offset_x + TAB_WIDTH + TAB_WIDTH_EXTRA;
	int y_offset = menu->offset_y + TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT / 2);

	if (subtab)
		y_offset += SUBTAB_HEIGHT + SUBTAB_PAD;

	int btn_x, btn_y;
	Color inside_color;

	for (size_t i = 0; i < options.size(); i++) {
		switch (options[i].type) {
		case TYPE_BOOL:
			btn_x = x_offset + (BUTTON_WIDTH / 2);
			btn_y = y_offset;

			inside_color = Colors::Invisible;

			if (IsInRegion(menu->mouse_x, menu->mouse_y, btn_x, btn_y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
				if (menu->is_clicking) {
					*options[i].boolean_value = !(*options[i].boolean_value);
					menu->is_clicking = false;
				}

				inside_color = Colors::LightGrey;
			}

			if (*options[i].boolean_value)
				inside_color = Colors::Green;

			interfaces->Surface->DrawSetColor(Colors::White);
			interfaces->Surface->DrawOutlinedRect(btn_x, btn_y, btn_x + BUTTON_WIDTH, btn_y + BUTTON_HEIGHT);

			interfaces->Surface->DrawSetColor(inside_color);
			interfaces->Surface->DrawFilledRect(btn_x + BUTTON_INDENT, btn_y + BUTTON_INDENT, btn_x + BUTTON_WIDTH - BUTTON_INDENT, btn_y + BUTTON_HEIGHT - BUTTON_INDENT);

			int tw, th;
			interfaces->Surface->GetTextSize(render->font_base, options[i].title, tw, th);

			render->Text(options[i].title, x_offset + (BUTTON_WIDTH * 2), y_offset + ((BUTTON_HEIGHT - th) / 2), render->font_base, Colors::White);

			y_offset += BUTTON_HEIGHT;

			break;
		default:
			break;
		}

		y_offset += OPTION_SEPARATION;
	}
}

Subtab::Subtab(const wchar_t* subtab_title) {
	this->title = subtab_title;
}

void Subtab::Draw() {
	DrawOptions(this->options, true);
}

Tab::Tab(const wchar_t* tab_title) {
	this->title = tab_title;
}

void Tab::Draw() {
	if (subtabs.empty()) {
		DrawOptions(this->options);
		return;
	}

	int hovered_tab = -1;

	if (menu->HoveringSubtab(subtabs.size(), hovered_tab) && menu->is_clicking && !menu->is_dragging)
		current_subtab = std::clamp(hovered_tab, 0, static_cast<int>(subtabs.size() - 1));

	int x_start = menu->offset_x + TAB_WIDTH + TAB_WIDTH_EXTRA + SUBTAB_PAD;
	int y_start = menu->offset_y + TITLEBAR_HEIGHT + SUBTAB_PAD;

	int subtab_total = (MENU_WIDTH - TAB_WIDTH - TAB_WIDTH_EXTRA - (SUBTAB_PAD * 2));
	int subtab_width = subtab_total / subtabs.size();

	interfaces->Surface->DrawSetColor(Colors::Black);
	interfaces->Surface->DrawOutlinedRect(x_start, y_start, x_start + subtab_total, y_start + SUBTAB_HEIGHT);

	for (size_t i = 0; i < subtabs.size(); i++) {
		Color tab_bg_color = Color(48, 48, 48, 255);

		if (i == current_subtab)
			tab_bg_color = Color(86, 86, 86, 255);
		else if (i == hovered_tab)
			tab_bg_color = Color(67, 67, 67, 255);

		interfaces->Surface->DrawSetColor(tab_bg_color);
		interfaces->Surface->DrawFilledRect(
			x_start + (subtab_width * i),
			y_start,
			x_start + (subtab_width * (i + 1)),
			y_start + SUBTAB_HEIGHT
		);

		int tw, th;
		interfaces->Surface->GetTextSize(render->font_base, subtabs[i].title, tw, th);

		render->Text(
			subtabs[i].title,
			x_start + (subtab_width / 2) - (tw / 2) + (subtab_width * i),
			y_start + (SUBTAB_HEIGHT / 2) - (th / 2) + 1, // TODO: Fix magic number
			render->font_base,
			Colors::White
		);

		interfaces->Surface->DrawSetColor(Colors::Black);
		interfaces->Surface->DrawLine(
			x_start + (subtab_width * i),
			y_start,
			x_start + (subtab_width * i),
			y_start + SUBTAB_HEIGHT
		);
	}

	subtabs[current_subtab].Draw();
}