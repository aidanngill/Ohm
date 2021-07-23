#pragma once

class IClientEntity;

bool GetBoundingBox(IClientEntity* entity, int& x, int& y, int& w, int& h);
void RenderVisuals();