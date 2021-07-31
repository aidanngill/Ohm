#include <filesystem>

#include "./Menu.h"
#include "./Option.h"
#include "./Tab.h"

#include "../Render.h"

#include "../../Config.h"

#include "../../Interfaces/Interfaces.h"
#include "../../Interfaces/Dependencies.h"

#include "../../Utility/File.h"

namespace fs = std::filesystem;

void SaveConfig() {
	config->DumpToFile(MakeConfigPath("config.json").c_str());
}

void LoadConfig() {
	config->LoadFromFile(MakeConfigPath("config.json").c_str());
}

Menu::Menu() {
	// [Start] Aim
	Tab tabAim = Tab(L"Aim");
	// [End] Aim

	// [Start] Visual
	Tab tabVisual = Tab(L"Visuals");

	SubTab stVisBox(L"Players");
	stVisBox.options.push_back(Option(L"Enabled", &config->visuals.players.isEnabled));
	stVisBox.options.push_back(Option(L"Outlined", &config->visuals.players.isOutlined));
	stVisBox.options.push_back(Option(L"Health", &config->visuals.players.hasHealth));
	stVisBox.options.push_back(Option(L"Armor", &config->visuals.players.hasArmor));
	stVisBox.options.push_back(Option(L"Name", &config->visuals.players.hasName));
	stVisBox.options.push_back(Option(L"Distance", &config->visuals.players.hasDistance));

	SubTab stVisEnt(L"Entities");
	stVisEnt.options.push_back(Option(L"Bomb", &config->visuals.entities.showBomb));
	stVisEnt.options.push_back(Option(L"Dropped Weapons", &config->visuals.entities.showDroppedWeapons));

	SubTab stVisChams(L"Chams");
	stVisChams.options.push_back(Option(L"Players", &config->visuals.chams.playerMaterial, std::vector<std::wstring>{ L"None", L"Regular", L"Flat", L"Plastic" }));
	stVisChams.options.push_back(Option(L"Weapon", &config->visuals.chams.weaponMaterial, std::vector<std::wstring>{ L"None", L"Regular", L"Flat", L"Plastic" }));
	stVisChams.options.push_back(Option(L"Arms", &config->visuals.chams.armsMaterial, std::vector<std::wstring>{ L"None", L"Regular", L"Flat", L"Plastic" }));

	SubTab stVisGlow(L"Glow");
	stVisGlow.options.push_back(Option(L"Players", &config->visuals.glow.showPlayers));
	stVisGlow.options.push_back(Option(L"Defuse Kits", &config->visuals.glow.showDefuseKits));
	stVisGlow.options.push_back(Option(L"Planted C4", &config->visuals.glow.showPlantedC4));
	stVisGlow.options.push_back(Option(L"Dropped Weapons", &config->visuals.glow.showDroppedWeapons));
	stVisGlow.options.push_back(Option(L"Chickens", &config->visuals.glow.showChickens));

	tabVisual.subTabs.push_back(stVisBox);
	tabVisual.subTabs.push_back(stVisEnt);
	tabVisual.subTabs.push_back(stVisChams);
	tabVisual.subTabs.push_back(stVisGlow);
	// [End] Visual

	// [Start] Misc
	Tab tabMisc = Tab(L"Misc");

	tabMisc.options.push_back(Option(L"Bunny Hop", &config->misc.bunnyHop));
	// [End] Misc

	// [Start] Config
	Tab tabConfig = Tab(L"Config");

	tabConfig.options.push_back(Option(L"Save", SaveConfig));
	tabConfig.options.push_back(Option(L"Load", LoadConfig));
	// [End] Config

	tabs.push_back(tabAim);
	tabs.push_back(tabVisual);
	tabs.push_back(tabMisc);
	tabs.push_back(tabConfig);
}

void Menu::Render() {
	if (!isOpen)
		return;

	interfaces->InputSystem->GetCursorPosition(&mouseX, &mouseY);

	if (IsClosing()) {
		isOpen = false;
		return;
	}

	if (isDragging) {
		if (isClicking) {
			offsetX = (mouseX - previousMouseX) + oldOffsetX;
			offsetY = (mouseY - previousMouseY) + oldOffsetY;
		}
		else {
			previousMouseX = mouseX;
			previousMouseY = mouseY;

			oldOffsetX = offsetX;
			oldOffsetY = offsetY;

			isDragging = false;
		}
	}
	else {
		isDragging = IsInRegion(mouseX, mouseY, offsetX, offsetY, TITLEBAR_WIDTH, TITLEBAR_HEIGHT);
	}

	// Background [full]
	interfaces->Surface->DrawSetColor(Color(32, 32, 32, 255));
	interfaces->Surface->DrawFilledRect(offsetX, offsetY, offsetX + MENU_WIDTH, offsetY + MENU_HEIGHT);

	// Titlebar [text]
	render->Text(L"Ohm v0.1.0", offsetX + 6, offsetY + 6, render->fontBase, Color(255, 255, 255, 255));

	// Titlebar [close button]
	interfaces->Surface->DrawSetColor(Color(255, 0, 0, 255));
	interfaces->Surface->DrawFilledRect(offsetX + TITLEBAR_WIDTH - 20, offsetY, offsetX + TITLEBAR_WIDTH, offsetY + 20);
	
	// Draw the tabs and their titles.
	int hoveredTab = -1;

	if (this->HoveringTab(hoveredTab) && isClicking && !isDragging)
		currentTab = std::clamp(hoveredTab, 0, static_cast<int>(tabs.size() - 1));

	int sx = offsetX;
	int sy = offsetY + TITLEBAR_HEIGHT;
	int ex = offsetX + TAB_WIDTH;
	int ey = sy + TAB_HEIGHT;

	for (size_t idx = 0; idx < tabs.size(); idx++) {
		Color tab_bg_color = Color(48, 48, 48, 255);
		Color tab_side_color = tab_bg_color;

		if (idx == currentTab) {
			tab_bg_color = Color(86, 86, 86, 255);
			tab_side_color = Color(0, 84, 255, 255);
		}
		else if (idx == hoveredTab) {
			tab_bg_color = Color(67, 67, 67, 255);
		}

		interfaces->Surface->DrawSetColor(tab_bg_color);
		interfaces->Surface->DrawFilledRect(sx, sy, ex, ey);

		interfaces->Surface->DrawSetColor(tab_side_color);
		interfaces->Surface->DrawFilledRect(ex, sy, ex + TAB_WIDTH_EXTRA, ey);

		render->Text(tabs[idx].title, offsetX + 6, sy + 6, render->fontBase, Color(255, 255, 255, 255));

		// (sy - 1) to fix https://cdn.discordapp.com/attachments/599271375043297282/869143609528696832/unknown.png
		interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));
		interfaces->Surface->DrawLine(ex, sy - 1, ex, ey);

		// Skip the horizontal line on the first tab.
		if (idx > 0)
			interfaces->Surface->DrawLine(sx, sy, ex + TAB_WIDTH_EXTRA, sy);

		sy += TAB_HEIGHT;
		ey += TAB_HEIGHT;
	}

	interfaces->Surface->DrawLine(sx, sy, ex + TAB_WIDTH_EXTRA, sy);

	// Draw the options within the selected tab.
	tabs[currentTab].Draw();

	// Do all the outlines at the end.
	interfaces->Surface->DrawSetColor(Color(0, 0, 0, 255));

	// Background [outline]
	interfaces->Surface->DrawOutlinedRect(offsetX, offsetY, offsetX + MENU_WIDTH, offsetY + MENU_HEIGHT);

	// Titlebar [outline]
	interfaces->Surface->DrawOutlinedRect(offsetX, offsetY, offsetX + TITLEBAR_WIDTH, offsetY + TITLEBAR_HEIGHT);

	// Titlebar [close button outline]
	interfaces->Surface->DrawOutlinedRect(offsetX + TITLEBAR_WIDTH - BUTTON_WIDTH, offsetY, offsetX + TITLEBAR_WIDTH, offsetY + BUTTON_HEIGHT);

	// Tab separator (vertical)
	interfaces->Surface->DrawLine(offsetX + TAB_WIDTH + TAB_WIDTH_EXTRA, offsetY + TITLEBAR_HEIGHT, offsetX + TAB_WIDTH + TAB_WIDTH_EXTRA, offsetY + TITLEBAR_HEIGHT + MENU_HEIGHT - TITLEBAR_HEIGHT);
}

void Menu::Controls(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		isClicking = true;
		break;
	case WM_LBUTTONUP:
		isClicking = false;
		break;
	default:
		break;
	}
}

// Check if the user is pressing the 'close' button at the top right.
bool Menu::IsClosing() {
	return isClicking &&
		mouseX == previousMouseX &&
		mouseY == previousMouseY &&
		IsInRegion(
			mouseX, mouseY,
			offsetX + TITLEBAR_WIDTH - BUTTON_HEIGHT, offsetY,
			BUTTON_WIDTH, BUTTON_HEIGHT
		);
}

bool Menu::HoveringTab(int& tabHovered) {
	if (!(offsetX <= mouseX && mouseX <= offsetX + (TAB_WIDTH + TAB_WIDTH_EXTRA)))
		return false;

	int yMin = offsetY + TITLEBAR_HEIGHT;
	int yMax = offsetY + TITLEBAR_HEIGHT + (TAB_HEIGHT * static_cast<int>(tabs.size()));

	if (!(yMin <= mouseY && mouseY <= yMax))
		return false;

	tabHovered = static_cast<int>(std::floor((mouseY - offsetY) / TAB_HEIGHT)) - 1;

	return true;
}

bool Menu::HoveringSubtab(int tabCount, int& tabHovered) {
	int subTabWidth = (MENU_WIDTH - TAB_WIDTH - TAB_WIDTH_EXTRA - (SUBTAB_PAD * 2));

	int xMin = this->offsetX + TAB_WIDTH + TAB_WIDTH_EXTRA + SUBTAB_PAD;
	int xMax = xMin + subTabWidth;

	if (!(xMin <= mouseX && mouseX <= xMax))
		return false;

	int yMin = offsetY + TITLEBAR_HEIGHT + SUBTAB_PAD;
	int yMax = yMin + SUBTAB_HEIGHT;

	if (!(yMin <= mouseY && mouseY <= yMax))
		return false;

	tabHovered = static_cast<int>(std::floor(static_cast<float>(mouseX - xMin) / static_cast<float>(subTabWidth / tabCount)));

	return true;
}