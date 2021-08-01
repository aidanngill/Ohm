#include "./CGlobalVars.h"
#include "./CUserCmd.h"

#include "../Entities/CBasePlayer.h"

#include "../../Interfaces/Interfaces.h"
#include "../../Interfaces/Dependencies.h"

#include "../../Utility/Utilities.h"

float CGlobalVars::ServerTime(CUserCmd* cmd) noexcept {
	static int tick;
	static CUserCmd* lastCmd;

	CBasePlayer* localPlayer = GetLocalPlayer();

	if (cmd) {
		if (localPlayer && (!lastCmd || lastCmd->hasBeenPredicted))
			tick = localPlayer->getTickBase();
		else
			tick++;

		lastCmd = cmd;
	}

	return tick * this->intervalPerTick;
}