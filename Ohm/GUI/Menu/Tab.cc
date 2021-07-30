#include <algorithm>

#include "./Tab.h"
#include "./Menu.h"
#include "./Option.h"

#include "../Colors.h"
#include "../Render.h"

#include "../../Interfaces/Interfaces.h"
#include "../../Interfaces/Dependencies.h"

#include "../../Utility/Utilities.h"

void GenericTab::DrawOptions() {
	int xOffset = menu->offsetX + TAB_WIDTH + TAB_WIDTH_EXTRA;
	int yOffset = menu->offsetY + TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT / 2);

	if (isSubTab)
		yOffset += SUBTAB_HEIGHT + SUBTAB_PAD;

	int tw, th;
	Color insideColor;

	for (size_t idx = 0; idx < options.size(); idx++) {
		Option option = options[idx];

		if (option.type == Option::TYPE_BOOL) {
			int btnX = xOffset + (BUTTON_WIDTH / 2);
			int btnY = yOffset;

			insideColor = Colors::Invisible;

			if (IsInRegion(menu->mouseX, menu->mouseY, btnX, btnY, BUTTON_WIDTH, BUTTON_HEIGHT)) {
				if (menu->isClicking) {
					*option.boolValue = !(*option.boolValue);
					menu->isClicking = false;
				}

				insideColor = Colors::LightGrey;
			}

			if (*option.boolValue)
				insideColor = Colors::Green;

			interfaces->Surface->DrawSetColor(Colors::White);
			interfaces->Surface->DrawOutlinedRect(btnX, btnY, btnX + BUTTON_WIDTH, btnY + BUTTON_HEIGHT);

			interfaces->Surface->DrawSetColor(insideColor);
			interfaces->Surface->DrawFilledRect(btnX + BUTTON_INDENT, btnY + BUTTON_INDENT, btnX + BUTTON_WIDTH - BUTTON_INDENT, btnY + BUTTON_HEIGHT - BUTTON_INDENT);

			interfaces->Surface->GetTextSize(render->fontBase, option.title, tw, th);

			render->Text(option.title, xOffset + (BUTTON_WIDTH * 2), yOffset + ((BUTTON_HEIGHT - th) / 2), render->fontBase, Colors::White);

			yOffset += BUTTON_HEIGHT;
		}
		else if (option.type == Option::TYPE_COMBO) {
			int cbX = xOffset + (BUTTON_WIDTH / 2);
			int cbY = yOffset;

			int cbItemsStart = cbY + BUTTON_HEIGHT;

			int cbWidth = (MENU_WIDTH - TAB_WIDTH - TAB_WIDTH_EXTRA - BUTTON_WIDTH) / 2;
			int cbHeight = BUTTON_HEIGHT * option.stringValues.size();

			insideColor = Colors::Invisible;

			if (IsInRegion(menu->mouseX, menu->mouseY, cbX, cbItemsStart, cbWidth, cbHeight) && currentOpened == idx) {
				hoveredComboItem = option.stringValues.size() - std::clamp(((cbItemsStart + cbHeight) - menu->mouseY) / BUTTON_HEIGHT, 0, static_cast<int>(option.stringValues.size()) - 1) - 1;

				if (menu->isClicking) {
					*options[currentOpened].intValue = hoveredComboItem;
					currentOpened = -1;

					menu->isClicking = false;
				}
			}
			else if (IsInRegion(menu->mouseX, menu->mouseY, cbX, cbY, cbWidth, BUTTON_HEIGHT)) {
				if (menu->isClicking) {
					if (currentOpened == -1)
						currentOpened = idx;
					else if (idx <= currentOpened)
						currentOpened = -1;

					hoveredComboItem = -1;
					menu->isClicking = false;
				}

				if (currentOpened == -1) insideColor = Colors::LightGrey;
			}

			interfaces->Surface->DrawSetColor(insideColor);
			interfaces->Surface->DrawFilledRect(cbX, cbY, cbX + cbWidth, cbY + BUTTON_HEIGHT);

			interfaces->Surface->DrawSetColor(Colors::White);
			interfaces->Surface->DrawOutlinedRect(cbX, cbY, cbX + cbWidth, cbY + BUTTON_HEIGHT);

			// draw current option text
			interfaces->Surface->GetTextSize(render->fontBase, option.CurrentString(), tw, th);
			render->Text(option.CurrentString(), cbX + (th / 2), cbY + (BUTTON_HEIGHT / 2) - (th / 2), render->fontBase, Colors::White);

			// draw text on the right
			interfaces->Surface->GetTextSize(render->fontBase, option.title, tw, th);
			render->Text(option.title, cbX + cbWidth + (th / 2), cbY + (BUTTON_HEIGHT / 2) - (th / 2), render->fontBase, Colors::White);

			yOffset += BUTTON_HEIGHT;
		}

		yOffset += OPTION_SEPARATION;
	}
}

void GenericTab::DrawComboContent() {
	if (this->currentOpened == -1)
		return;

	Option option = options[currentOpened];
	int opStrSize = option.stringValues.size();

	int xOffset = menu->offsetX + TAB_WIDTH + TAB_WIDTH_EXTRA;
	int yOffset = menu->offsetY + TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT / 2) + ((BUTTON_HEIGHT + OPTION_SEPARATION) * currentOpened) + BUTTON_HEIGHT;

	if (isSubTab)
		yOffset += SUBTAB_HEIGHT + SUBTAB_PAD;

	int cbX = xOffset + (BUTTON_WIDTH / 2);
	int cbY = yOffset;

	int cbWidth = (MENU_WIDTH - TAB_WIDTH - TAB_WIDTH_EXTRA - BUTTON_WIDTH) / 2;
	int cbHeight = BUTTON_HEIGHT * opStrSize;

	// Draw the full background.
	interfaces->Surface->DrawSetColor(Colors::DarkGrey);
	interfaces->Surface->DrawFilledRect(cbX, cbY, cbX + cbWidth, cbY + cbHeight);

	for (int idx = 0; idx < opStrSize; idx++) {
		// Check if we're hovering over a combo item.
		if (hoveredComboItem == idx) {
			interfaces->Surface->DrawSetColor(Colors::LightGrey);
			interfaces->Surface->DrawFilledRect(cbX, cbY + (BUTTON_HEIGHT * idx), cbX + cbWidth, cbY + (BUTTON_HEIGHT * (idx + 1)));
		}

		int tw, th;
		interfaces->Surface->GetTextSize(render->fontBase, option.stringValues[idx].c_str(), tw, th);

		int txtX = cbX + (th / 2);
		int txtY = cbY + (BUTTON_HEIGHT * idx) + ((BUTTON_HEIGHT / 2) - (th / 2));

		// Render the combo item's text.
		render->Text(option.stringValues[idx].c_str(), txtX, txtY, render->fontBase, Colors::White);
	}

	// Render the full outline over the open combo items.
	interfaces->Surface->DrawSetColor(Colors::White);
	interfaces->Surface->DrawOutlinedRect(cbX, cbY - 1, cbX + cbWidth, cbY + cbHeight);
}

SubTab::SubTab(const wchar_t* subTabTitle) {
	title = subTabTitle;
	isSubTab = true;
}

void SubTab::Draw() {
	DrawOptions();
	DrawComboContent();
}

Tab::Tab(const wchar_t* tabTitle) {
	title = tabTitle;
}

void Tab::Draw() {
	if (subTabs.empty()) {
		DrawOptions();
		DrawComboContent();

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