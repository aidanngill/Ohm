#pragma once

class Color;
class IClientEntity;

bool GetBoundingBox(IClientEntity* entity, int& x, int& y, int& w, int& h);
void DrawBoundingBox(int x, int y, int w, int h, Color color);
void DrawBombTimer(IClientEntity* bomb_entity);