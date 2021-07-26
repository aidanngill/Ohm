#include "./Misc.h"

#include "../Interfaces.h"
#include "../InterfaceDep.h"

#include "../Config.h"

#include "../SDK/CUserCmd.h"
#include "../SDK/IClientEntity.h"

#include "../Utility/Utilities.h"

void Misc::BunnyHop(CUserCmd* cmd) {
	if (!config->misc.bunny_hop)
		return;

	IClientEntity* local_player = GetLocalPlayer();

	if (!local_player)
		return;

	int flags = local_player->Flags();
	static auto was_last_on_ground{ flags & 1 };

	if (interfaces->InputSystem->IsButtonDown(KEY_SPACE) && !(flags & 1) && !was_last_on_ground)
		cmd->buttons &= ~(1 << 1);

	was_last_on_ground = flags & 1;
}