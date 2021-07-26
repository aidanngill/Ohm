#include "./Tab.h"
#include "./Menu.h"

#include "../Colors.h"
#include "../Render.h"

#include "../../Interfaces.h"
#include "../../InterfaceDep.h"

#include "../../Utility/Utilities.h"

#define OPTION_SEPARATION 20
#define BUTTON_INDENT 2

Tab::Tab(const wchar_t* tab_title) {
	this->title = tab_title;
}

void Tab::Draw() {
	int x_offset = menu->offset_x + TAB_WIDTH + TAB_WIDTH_EXTRA;
	int y_offset = menu->offset_y + TITLEBAR_HEIGHT + (OPTION_SEPARATION / 2);

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

			break;
		default:
			break;
		}
	}
}