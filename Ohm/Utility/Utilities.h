#pragma once

class CBasePlayer;

namespace Utilities {
	void attachConsole();
	void detachConsole();
	uint8_t* patternScan(void* module, const char* signature);
	CBasePlayer* getLocalPlayer();
}