#include <string>

#include "./Visuals.h"

#include "../Interfaces.h"
#include "../InterfaceDep.h"
#include "../Memory.h"

#include "../GUI/Render.h"

#include "../SDK/CGlobalVars.h"
#include "../SDK/ICollideable.h"
#include "../SDK/IClientEntity.h"
#include "../SDK/Math/Vector.h"

#include "../Utility/Utilities.h"

bool GetBoundingBox(IClientEntity* entity, int& x, int& y, int& w, int& h) {
	Vector origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	origin = entity->VecOrigin();

	ICollideable* collideable = entity->GetCollideable();
	min = collideable->OBBMins() + origin;
	max = collideable->OBBMaxs() + origin;

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	if (interfaces->DebugOverlay->ScreenPosition(points[3], flb) || interfaces->DebugOverlay->ScreenPosition(points[5], brt)
		|| interfaces->DebugOverlay->ScreenPosition(points[0], blb) || interfaces->DebugOverlay->ScreenPosition(points[4], frt)
		|| interfaces->DebugOverlay->ScreenPosition(points[2], frb) || interfaces->DebugOverlay->ScreenPosition(points[1], brb)
		|| interfaces->DebugOverlay->ScreenPosition(points[6], blt) || interfaces->DebugOverlay->ScreenPosition(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;
	
	for (int i = 0; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;

		if (bottom < arr[i].y)
			bottom = arr[i].y;

		if (right < arr[i].x)
			right = arr[i].x;

		if (top > arr[i].y)
			top = arr[i].y;
	}

	x = static_cast<int>(left);
	y = static_cast<int>(top);
	w = static_cast<int>(right - left);
	h = static_cast<int>(bottom - top);

	return true;
}

void DrawBoundingBox(int x, int y, int w, int h, Color color) {
	interfaces->Surface->DrawSetColor(color);
	interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void DrawBombTimer(IClientEntity* bomb_entity) {
	float time = bomb_entity->BombTimer() - memory->GlobalVars->current_time;
	
	if (time < 0)
		return;

	int w, h;
	interfaces->Surface->GetScreenSize(w, h);

	float length = bomb_entity->BombLength();
	float ratio = time / length;

	interfaces->Surface->DrawSetColor(Color(32, 32, 32, 255));
	interfaces->Surface->DrawFilledRect(w / 3, h / 3, (w / 3) * 2, (h / 3) + 3);

	interfaces->Surface->DrawSetColor(Color(
		static_cast<int>(255 - (255 * ratio)),
		static_cast<int>(0 + (255 * ratio)),
		0, 255
	));

	interfaces->Surface->DrawFilledRect(
		(w / 3) + 1,
		(h / 3) + 1,
		((w / 3) + ((w / 3) * 1) * ratio) - 1,
		((h / 3) + 3) - 1
	);

	int fw, fh;

	std::wstring text = std::to_wstring(time);
	interfaces->Surface->GetTextSize(render->font_base, text.c_str(), fw, fh);

	render->Text(text.c_str(), ((w / 3) * 2) - ((w / 3) / 2) - (fw / 2), h / 3, render->font_base, Color(255, 255, 255, 255));
}