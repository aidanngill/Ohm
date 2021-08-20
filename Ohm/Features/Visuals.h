#pragma once

class Color;

class CC4;
class CBaseEntity;

namespace Visuals {
	bool GetBoundingBox(CBaseEntity* entity, int& x, int& y, int& w, int& h);
	void DrawBoundingBox(int x, int y, int w, int h, Color color);
	void DrawBombTimer(CC4* bombEntity);
	void DrawBombBox(CC4* bombEntity);
	void Render();
}