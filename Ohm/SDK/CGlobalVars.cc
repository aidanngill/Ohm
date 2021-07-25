#include "./CGlobalVars.h"
#include "./CUserCmd.h"

#include "../Interfaces.h"
#include "../InterfaceDep.h"

#include "../Utility/Utilities.h"

float CGlobalVars::ServerTime(CUserCmd* cmd) noexcept {
	static int tick;
	static CUserCmd* last_cmd;

	IClientEntity* local_player = GetLocalPlayer();

	if (cmd) {
		if (local_player && (!last_cmd || last_cmd->has_been_predicted)) {
			tick = local_player->TickBase();
		}
		else {
			tick++;
		}

		last_cmd = cmd;
	}

	return tick * interval_per_tick;
}