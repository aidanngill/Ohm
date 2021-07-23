#include "./Render.h"
#include "../Interfaces.h"
#include "../InterfaceDep.h"

#include "../Features/Visuals.h"

#include "../SDK/Color.h"
#include "../SDK/ISurface.h"

#include "../Utility/Utilities.h"

Render::Render() {
	font_base = interfaces->Surface->CreateFont();
	interfaces->Surface->SetFontGlyphSet(font_base, "Tahoma", 15, 400, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);
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
	int max_clients = interfaces->Engine->GetMaxClients();

	for (int i = 0; i < interfaces->ClientEntityList->GetHighestEntityIndex(); i++) {
		IClientEntity* entity = reinterpret_cast<IClientEntity*>(interfaces->ClientEntityList->GetClientEntity(i));

		if (!entity || !entity->IsAlive() || entity == local_player)
			continue;

		if (i <= max_clients) {
			if (entity->GetDormant())
				continue;

			int x, y, w, h;

			if (!GetBoundingBox(entity, x, y, w, h))
				continue;

			DrawBoundingBox(x, y, w, h, Color(255, 255, 255, 255));
		}
	}
}