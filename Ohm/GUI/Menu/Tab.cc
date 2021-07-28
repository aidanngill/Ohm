#include <algorithm>

#include "./Tab.h"
#include "./Menu.h"

#include "../Colors.h"
#include "../Render.h"

#include "../../Interfaces/Interfaces.h"
#include "../../Interfaces/Dependencies.h"

#include "../../Utility/Utilities.h"

void DrawOptions(std::vector<Option> options, bool isSubTab = false) {
	int xOffset = menu->offsetX + TAB_WIDTH + TAB_WIDTH_EXTRA;
	int yOffset = menu->offsetY + TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT / 2);

	if (isSubTab)
		yOffset += SUBTAB_HEIGHT + SUBTAB_PAD;

	int btnX, btnY;
	Color insideColor;

	for (size_t i = 0; i < options.size(); i++) {
		switch (options[i].type) {
		case Option::TYPE_BOOL:
			btnX = xOffset + (BUTTON_WIDTH / 2);
			btnY = yOffset;

			insideColor = Colors::Invisible;

			if (IsInRegion(menu->mouseX, menu->mouseY, btnX, btnY, BUTTON_WIDTH, BUTTON_HEIGHT)) {
				if (menu->isClicking) {
					*options[i].boolValue = !(*options[i].boolValue);
					menu->isClicking = false;
				}

				insideColor = Colors::LightGrey;
			}

			if (*options[i].boolValue)
				insideColor = Colors::Green;

			interfaces->Surface->DrawSetColor(Colors::White);
			interfaces->Surface->DrawOutlinedRect(btnX, btnY, btnX + BUTTON_WIDTH, btnY + BUTTON_HEIGHT);

			interfaces->Surface->DrawSetColor(insideColor);
			interfaces->Surface->DrawFilledRect(btnX + BUTTON_INDENT, btnY + BUTTON_INDENT, btnX + BUTTON_WIDTH - BUTTON_INDENT, btnY + BUTTON_HEIGHT - BUTTON_INDENT);

			int tw, th;
			interfaces->Surface->GetTextSize(render->fontBase, options[i].title, tw, th);

			render->Text(options[i].title, xOffset + (BUTTON_WIDTH * 2), yOffset + ((BUTTON_HEIGHT - th) / 2), render->fontBase, Colors::White);

			yOffset += BUTTON_HEIGHT;

			break;
		default:
			break;
		}

		yOffset += OPTION_SEPARATION;
	}
}

SubTab::SubTab(const wchar_t* subTabTitle) {
	title = subTabTitle;
}

void SubTab::Draw() {
	DrawOptions(options, true);
}

Tab::Tab(const wchar_t* tab_title) {
	this->title = tab_title;
}

void Tab::Draw() {
	if (subTabs.empty()) {
		DrawOptions(this->options);
		return;
	}

	int hoveredTab = -1;

	if (menu->HoveringSubtab(subTabs.size(), hoveredTab) && menu->isClicking && !menu->isDragging)
		currentSubTab = std::clamp(hoveredTab, 0, static_cast<int>(subTabs.size() - 1));

	int xStart = menu->offsetX + TAB_WIDTH + TAB_WIDTH_EXTRA + SUBTAB_PAD;
	int yStart = menu->offsetY + TITLEBAR_HEIGHT + SUBTAB_PAD;

	int subTabTotal = (MENU_WIDTH - TAB_WIDTH - TAB_WIDTH_EXTRA - (SUBTAB_PAD * 2));
	int subTabWidth = subTabTotal / subTabs.size();

	interfaces->Surface->DrawSetColor(Colors::Black);
	interfaces->Surface->DrawOutlinedRect(xStart, yStart, xStart + subTabTotal, yStart + SUBTAB_HEIGHT);

	for (size_t idx = 0; idx < subTabs.size(); idx++) {
		Color taskBgColor = Color(48, 48, 48, 255);

		if (idx == currentSubTab)
			taskBgColor = Color(86, 86, 86, 255);
		else if (idx == hoveredTab)
			taskBgColor = Color(67, 67, 67, 255);

		interfaces->Surface->DrawSetColor(taskBgColor);
		interfaces->Surface->DrawFilledRect(
			xStart + (subTabWidth * idx),
			yStart,
			xStart + (subTabWidth * (idx + 1)),
			yStart + SUBTAB_HEIGHT
		);

		int tw, th;
		interfaces->Surface->GetTextSize(render->fontBase, subTabs[idx].title, tw, th);

		render->Text(
			subTabs[idx].title,
			xStart + (subTabWidth / 2) - (tw / 2) + (subTabWidth * idx),
			yStart + (SUBTAB_HEIGHT / 2) - (th / 2) + 1, // TODO: Fix magic number
			render->fontBase,
			Colors::White
		);

		interfaces->Surface->DrawSetColor(Colors::Black);
		interfaces->Surface->DrawLine(
			xStart + (subTabWidth * idx),
			yStart,
			xStart + (subTabWidth * idx),
			yStart + SUBTAB_HEIGHT
		);
	}

	subTabs[currentSubTab].Draw();
}