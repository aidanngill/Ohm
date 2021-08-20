#include <string>

#include "./Visuals.h"

#include "../Config.h"
#include "../Memory.h"

#include "../GUI/Colors.h"
#include "../GUI/Render.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../SDK/Class/CGlobalVars.h"
#include "../SDK/Class/CGlowObjectManager.h"

#include "../SDK/Interfaces/ICollideable.h"

#include "../SDK/Entities/CBasePlayer.h"
#include "../SDK/Entities/CC4.h"

#include "../Utility/Utilities.h"

static bool ScreenTransform(const Vector& in, Vector& out) {
	static auto& wtsMatrix = interfaces->Engine->WorldToScreenMatrix();

	out.x = wtsMatrix.m[0][0] * in.x + wtsMatrix.m[0][1] * in.y + wtsMatrix.m[0][2] * in.z + wtsMatrix.m[0][3];
	out.y = wtsMatrix.m[1][0] * in.x + wtsMatrix.m[1][1] * in.y + wtsMatrix.m[1][2] * in.z + wtsMatrix.m[1][3];
	out.z = 0.0f;

	float w = wtsMatrix.m[3][0] * in.x + wtsMatrix.m[3][1] * in.y + wtsMatrix.m[3][2] * in.z + wtsMatrix.m[3][3];

	if (w < 0.001f)
		return false;

	out.x /= w;
	out.y /= w;

	return true;
}

bool WorldToScreen(const Vector& in, Vector& out) {
	if (!ScreenTransform(in, out))
		return false;

	int w, h;
	interfaces->Engine->GetScreenSize(w, h);

	out.x = (w / 2.0f) + (out.x * w) / 2.0f;
	out.y = (h / 2.0f) - (out.y * h) / 2.0f;

	return true;
}

void VectorTransform(const Vector& vecIn, const matrix3x4_t& matrixIn, Vector& vecOut) {
	for (int i = 0; i < 3; i++)
		vecOut[i] = vecIn.dotMultiply(matrixIn[i]) + matrixIn[i][3];
}

bool Visuals::GetBoundingBox(CBaseEntity* entity, int& x, int& y, int& w, int& h) {
	ICollideable* collideable = entity->getCollideable();

	if (!collideable)
		return false;

	Vector min, max;

	min = collideable->OBBMins();
	max = collideable->OBBMaxs();

	const matrix3x4_t& trans = entity->getCoordinateFrame();

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

	Vector pointsTransformed[8] = {};

	for (int i = 0; i < 8; i++)
		VectorTransform(points[i], trans, pointsTransformed[i]);

	Vector pointsScreen[8] = {};

	for (int i = 0; i < 8; i++)
		if (!WorldToScreen(pointsTransformed[i], pointsScreen[i]))
			return false;

	float left = pointsScreen[0].x;
	float top = pointsScreen[0].y;
	float right = pointsScreen[0].x;
	float bottom = pointsScreen[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > pointsScreen[i].x)
			left = pointsScreen[i].x;

		if (top < pointsScreen[i].y)
			top = pointsScreen[i].y;

		if (right < pointsScreen[i].x)
			right = pointsScreen[i].x;

		if (bottom > pointsScreen[i].y)
			bottom = pointsScreen[i].y;
	}

	x = static_cast<int>(left);
	y = static_cast<int>(bottom);
	w = static_cast<int>(right - left);
	h = static_cast<int>(top - bottom);

	// Wow this took a lot of debugging, but the result is much better
	// looking, matching up much closer than before with IVDebugOverlay.
	// https://on.wii.golf/G61OkDBsaP.png

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

	interfaces->Surface->DrawSetColor(Colors::DarkGrey);
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
	int x, y, w, h;

	if (!GetBoundingBox(reinterpret_cast<CBaseEntity*>(bombEntity), x, y, w, h))
		return;

	interfaces->Surface->DrawSetColor(Colors::White);
	interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Visuals::Render() {
	if (!interfaces->Engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Utilities::getLocalPlayer();
	CBaseCombatWeapon* currentWeapon = localPlayer->getCurrentWeapon();

	int maxEntities = interfaces->ClientEntityList->GetHighestEntityIndex();
	int maxClients = interfaces->Engine->GetMaxClients();

	for (int idx = 0; idx < maxEntities; idx++) {
		CBaseEntity* entity = interfaces->ClientEntityList->GetClientEntity(idx);

		if (!entity || entity == localPlayer)
			continue;

		if (entity->GetDormant())
			continue;

		if (idx <= maxClients) {
			CBasePlayer* thisPlayer = reinterpret_cast<CBasePlayer*>(entity);

			if (!thisPlayer->isAlive())
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
			// Don't render the bomb box if we're holding it.
			if (!currentWeapon || currentWeapon->GetClientClass()->m_ClassID == netvars->classIdentifiers.CC4)
				continue;

			// Don't render it if we are holding it in our off-hand.
			if (memory->IsC4Owner(reinterpret_cast<uintptr_t>(localPlayer)))
				continue;

			CC4* thisBomb = reinterpret_cast<CC4*>(entity);

			if (config->visuals.entities.showBomb) {
				DrawBombTimer(thisBomb);
				DrawBombBox(thisBomb);
			}
		}
		else if (entity->isWeapon()) {
			if (!config->visuals.entities.showDroppedWeapons)
				continue;

			CBaseCombatWeapon* thisWeapon = reinterpret_cast<CBaseCombatWeapon*>(entity);

			// Don't render guns that are being held.
			if (thisWeapon->getOwnerEntity().IsValid())
				continue;

			int x, y, w, h;

			if (!GetBoundingBox(entity, x, y, w, h))
				continue;

			DrawBoundingBox(x, y, w, h, Colors::White);
		}
	}
}