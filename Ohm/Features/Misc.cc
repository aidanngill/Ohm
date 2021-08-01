#include "./Misc.h"

#include "../Config.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../SDK/CUserCmd.h"

#include "../SDK/Entities/CBasePlayer.h"

#include "../Utility/Utilities.h"

void Misc::BunnyHop(CUserCmd* cmd) {
	if (!config->misc.bunnyHop)
		return;

	CBasePlayer* localPlayer = GetLocalPlayer();

	if (!localPlayer)
		return;

	int flags = localPlayer->getFlags();
	static auto wasLastOnGround{ flags & 1 };

	if (interfaces->InputSystem->IsButtonDown(KEY_SPACE) && !(flags & 1) && !wasLastOnGround)
		cmd->buttons &= ~(1 << 1);

	wasLastOnGround = flags & 1;
}