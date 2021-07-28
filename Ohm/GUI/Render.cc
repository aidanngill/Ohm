#include "./Render.h"

#include "../Config.h"

#include "../Features/Visuals.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../Utility/Utilities.h"

bool IsInRegion(int x, int y, int tx, int ty, int tw, int th) {
	return (tx <= x && x <= tx + tw) && (ty <= y && y <= ty + th);
}

Render::Render() {
	this->SetupFonts();
}

void Render::SetupFonts() {
	this->fontBase = 0x5;
}

void Render::Text(const wchar_t* text, int x, int y, Font font, Color color = Color(255, 255, 255, 255)) {
	interfaces->Surface->DrawSetTextPos(x, y);
	interfaces->Surface->DrawSetTextColor(color);
	interfaces->Surface->DrawSetTextFont(font);
	interfaces->Surface->DrawPrintText(text, wcslen(text));
}

void Render::Watermark() {
	this->Text(this->watermarkText, 10, 10, this->fontBase);
}

void Render::Visuals() {
	if (!interfaces->Engine->IsInGame())
		return;

	IClientEntity* localPlayer = GetLocalPlayer();

	int maxEntities = interfaces->ClientEntityList->GetHighestEntityIndex();
	int maxClients = interfaces->Engine->GetMaxClients();

	for (int idx = 0; idx < maxEntities; idx++) {
		IClientEntity* entity = interfaces->ClientEntityList->GetClientEntity(idx);

		if (!entity || entity == localPlayer)
			continue;

		if (idx <= maxClients) {
			if (entity->GetDormant() || !entity->IsAlive())
				continue;

			int x, y, w, h;

			if (!GetBoundingBox(entity, x, y, w, h))
				continue;

			if (config->visuals.box.isEnabled)
				DrawBoundingBox(x, y, w, h, Color(255, 255, 255, 255));
		}
		else if (entity->IsC4()) {
			DrawBombTimer(entity);
		}
	}
}