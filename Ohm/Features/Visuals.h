#pragma once

class Color;

class CC4;
class IClientEntity;

namespace Visuals {
	bool GetBoundingBox(IClientEntity* entity, int& x, int& y, int& w, int& h);
	void DrawBoundingBox(int x, int y, int w, int h, Color color);
	void DrawBombTimer(CC4* bombEntity);
	void DrawBombBox(CC4* bombEntity);
	void Render();
}

namespace Glow {
	void Shutdown();
	void Render();
}