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
	this->font_base = 0x5;
	this->font_small = interfaces->Surface->CreateFont();

	interfaces->Surface->SetFontGlyphSet(this->font_small, "Small Fonts", 14, 400, 0, 0, FONTFLAG_OUTLINE);
}

void Render::Text(const wchar_t* text, int x, int y, Font font, Color color = Color(255, 255, 255, 255)) {
	interfaces->Surface->DrawSetTextPos(x, y);
	interfaces->Surface->DrawSetTextColor(color);
	interfaces->Surface->DrawSetTextFont(font);
	interfaces->Surface->DrawPrintText(text, wcslen(text));
}

void Render::Watermark() {
	this->Text(this->watermark_text, 10, 10, this->font_base);
}

void Render::Visuals() {
	if (!interfaces->Engine->IsInGame())
		return;

	IClientEntity* local_player = GetLocalPlayer();

	int max_entities = interfaces->ClientEntityList->GetHighestEntityIndex();
	int max_clients = interfaces->Engine->GetMaxClients();

	for (int i = 0; i < max_entities; i++) {
		IClientEntity* entity = interfaces->ClientEntityList->GetClientEntity(i);

		if (!entity || entity == local_player)
			continue;

		if (i <= max_clients) {
			if (entity->GetDormant() || !entity->IsAlive())
				continue;

			int x, y, w, h;

			if (!GetBoundingBox(entity, x, y, w, h))
				continue;

			if (config->visuals.box.enabled)
				DrawBoundingBox(x, y, w, h, Color(255, 255, 255, 255));
		}
		else if (entity->IsC4()) {
			DrawBombTimer(entity);
		}
	}
}