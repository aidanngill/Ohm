#include <string>

#include "./Visuals.h"

#include "../Config.h"
#include "../Memory.h"
#include "../Netvars.h"

#include "../GUI/Colors.h"
#include "../GUI/Render.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../SDK/CGlobalVars.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/ICollideable.h"

#include "../SDK/Entities/CBasePlayer.h"
#include "../SDK/Entities/CC4.h"

#include "../SDK/Math/Vector.h"

#include "../Utility/Utilities.h"

bool Visuals::GetBoundingBox(IClientEntity* entity, int& x, int& y, int& w, int& h) {
	Vector origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	origin = entity->getOrigin();

	ICollideable* collideable = entity->getCollideable();
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

void Visuals::DrawBoundingBox(int x, int y, int w, int h, Color color) {
	interfaces->Surface->DrawSetColor(color);
	interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Visuals::DrawBombTimer(CC4* bombEntity) {
	float time = bombEntity->getTimer() - memory->GlobalVars->currentTime;
	
	if (time < 0)
		return;

	int w, h;
	interfaces->Surface->GetScreenSize(w, h);

	float length = bombEntity->getLength();
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
	interfaces->Surface->GetTextSize(render->fontBase, text.c_str(), fw, fh);

	render->Text(text.c_str(), ((w / 3) * 2) - ((w / 3) / 2) - (fw / 2), h / 3, render->fontBase, Color(255, 255, 255, 255));
}

void Visuals::DrawBombBox(CC4* bombEntity) {
	float time = bombEntity->getTimer() - memory->GlobalVars->currentTime;

	if (time < 0)
		return;

	int x, y, w, h;

	if (!GetBoundingBox(bombEntity, x, y, w, h))
		return;

	interfaces->Surface->DrawSetColor(Colors::White);
	interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Visuals::Render() {
	if (!interfaces->Engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GetLocalPlayer();

	int maxEntities = interfaces->ClientEntityList->GetHighestEntityIndex();
	int maxClients = interfaces->Engine->GetMaxClients();

	for (int idx = 0; idx < maxEntities; idx++) {
		IClientEntity* entity = interfaces->ClientEntityList->GetClientEntity(idx);

		if (!entity || entity == localPlayer)
			continue;

		if (idx <= maxClients) {
			CBasePlayer* thisPlayer = reinterpret_cast<CBasePlayer*>(entity);

			if (thisPlayer->GetDormant() || !thisPlayer->isAlive())
				continue;

			int x, y, w, h;

			if (!GetBoundingBox(thisPlayer, x, y, w, h))
				continue;

			if (config->visuals.players.isEnabled)
				DrawBoundingBox(x, y, w, h, Color(255, 255, 255, 255));

			if (config->visuals.players.hasHealth) {
				float healthRatio = std::clamp(static_cast<float>(thisPlayer->getHealth()) / 100.f, 0.f, 100.f);
				Color healthColor = Color(
					static_cast<int>(255 - (255 * healthRatio)),
					static_cast<int>(0 + (255 * healthRatio)),
					0, 255
				);

				interfaces->Surface->DrawSetColor(healthColor);
				interfaces->Surface->DrawFilledRect(x - 4, y, x - 2, y + (h * healthRatio));
			}

			if (config->visuals.players.hasArmor) {
				float armorRatio = std::clamp(static_cast<float>(thisPlayer->getArmor()) / 100.f, 0.f, 100.f);

				interfaces->Surface->DrawSetColor(Colors::LightBlue);
				interfaces->Surface->DrawFilledRect(x + w + 2, y, x + w + 4, y + (h * armorRatio));
			}
		}
		else if (entity->isC4()) {
			CC4* thisBomb = reinterpret_cast<CC4*>(entity);

			if (config->visuals.entities.showBomb) {
				DrawBombTimer(thisBomb);
				DrawBombBox(thisBomb);
			}
		}
	}
}

void Glow::Shutdown() {
	for (int idx = 0; idx < memory->GlowObjectManager->glowObjectDefinitions.Count(); idx++) {
		GlowObjectDefinition_t& glowObject = memory->GlowObjectManager->glowObjectDefinitions[idx];
		IClientEntity* entity = glowObject.entity;

		if (glowObject.IsUnused())
			continue;

		if (!entity || entity->GetDormant())
			continue;

		glowObject.glowAlpha = 0.f;
	}
}

void Glow::Render() {
	CBasePlayer* localPlayer = GetLocalPlayer();
	static const float rgbMult = 1.f / 256.f;

	for (int idx = 0; idx < memory->GlowObjectManager->glowObjectDefinitions.Count(); idx++) {
		GlowObjectDefinition_t& glowObject = memory->GlowObjectManager->glowObjectDefinitions[idx];
		IClientEntity* entity = glowObject.entity;

		if (glowObject.IsUnused())
			continue;

		if (!entity || entity->GetDormant())
			continue;

		int classId = entity->GetClientClass()->m_ClassID;
		Color color{};

		if (classId == netvars->classIdentifiers["CCSPlayer"]) {
			CBasePlayer* thisPlayer = reinterpret_cast<CBasePlayer*>(entity);

			if (!config->visuals.glow.showPlayers)
				continue;

			if (!thisPlayer->isAlive())
				continue;

			bool isEnemy = thisPlayer->getTeam() != localPlayer->getTeam();

			if (isEnemy) {
				color = thisPlayer->hasC4() ? Colors::Green : Colors::Red;
			}
			else {
				color = Colors::Blue;
			}
		}
		else if (classId == netvars->classIdentifiers["CChicken"]) {
			if (!config->visuals.glow.showChickens)
				continue;

			*entity->shouldGlow() = true;
			color = Colors::Blue;
		}
		else if (classId == netvars->classIdentifiers["CBaseAnimating"]) {
			if (!config->visuals.glow.showDefuseKits)
				continue;

			color = Colors::Blue;
		}
		else if (classId == netvars->classIdentifiers["CPlantedC4"]) {
			if (!config->visuals.glow.showPlantedC4)
				continue;

			color = Colors::Blue;
		}
		else if (entity->isWeapon()) {
			if (!config->visuals.glow.showDroppedWeapons)
				continue;

			color = Colors::Blue;
		}

		glowObject.glowRed = color.r() * rgbMult;
		glowObject.glowGreen = color.g() * rgbMult;
		glowObject.glowBlue = color.b() * rgbMult;
		glowObject.glowAlpha = color.a() * rgbMult;

		glowObject.renderWhenOccluded = true;
		glowObject.renderWhenUnoccluded = false;

		glowObject.bloomAmount = config->visuals.glow.bloomAmount;
	}
}