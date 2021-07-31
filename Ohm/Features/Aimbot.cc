#include "./Aimbot.h"

#include "../Config.h"

#include "../SDK/CGlobalVars.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/IClientEntity.h"

#include "../Utility/Utilities.h"

void Aimbot::Run(CUserCmd* cmd) {
	if (!config->aim.isEnabled)
		return;

	IClientEntity* localPlayer = GetLocalPlayer();

	if (!localPlayer || localPlayer->NextAttack() > memory->GlobalVars->ServerTime() || localPlayer->IsDefusing())
		return;

	if (!config->aim.ignoreFlash && localPlayer->IsFlashed())
		return;

	float bestFov = config->aim.fieldOfView;
	Vector bestAngle{};
	const Vector localPlayerEyePosition = localPlayer->GetEyePosition();
}