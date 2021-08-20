#include "./Glow.h"

#include "../Config.h"
#include "../Memory.h"

#include "../GUI/Colors.h"

#include "../Utility/Utilities.h"

#include "../SDK/Class/CGlowObjectManager.h"

#include "../SDK/Entities/CBasePlayer.h"

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
	CBasePlayer* localPlayer = Utilities::getLocalPlayer();
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

		if (classId == netvars->classIdentifiers.CCSPlayer) {
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
		else if (classId == netvars->classIdentifiers.CChicken) {
			if (!config->visuals.glow.showChickens)
				continue;

			*entity->shouldGlow() = true;
			color = Colors::Blue;
		}
		else if (classId == netvars->classIdentifiers.CBaseAnimating) {
			if (!config->visuals.glow.showDefuseKits)
				continue;

			color = Colors::Blue;
		}
		else if (classId == netvars->classIdentifiers.CPlantedC4) {
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