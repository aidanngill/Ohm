#include "./CGlobalVars.h"
#include "./CUserCmd.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../Utility/Utilities.h"

float CGlobalVars::ServerTime(CUserCmd* cmd) noexcept {
	static int tick;
	static CUserCmd* lastCmd;

	IClientEntity* localPlayer = GetLocalPlayer();

	if (cmd) {
		if (localPlayer && (!lastCmd || lastCmd->hasBeenPredicted))
			tick = localPlayer->TickBase();
		else
			tick++;

		lastCmd = cmd;
	}

	return tick * this->intervalPerTick;
}