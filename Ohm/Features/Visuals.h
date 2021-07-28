#pragma once

class Color;
class IClientEntity;

namespace Visuals {
	bool GetBoundingBox(IClientEntity* entity, int& x, int& y, int& w, int& h);
	void DrawBoundingBox(int x, int y, int w, int h, Color color);
	void DrawBombTimer(IClientEntity* bombEntity);
	void DrawBombBox(IClientEntity* bombEntity);
	void Render();
}