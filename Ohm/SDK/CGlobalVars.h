#pragma once

#include <cstddef>

class CUserCmd;

class CGlobalVars {
public:
	float realTime;
	int frameCount;
	float absoluteFrameTime;
	float absoluteFrameStartTime;
	float currentTime;
	float frameTime;
	int maxClients;
	int tickCount;
	float intervalPerTick;

	float ServerTime(CUserCmd* = nullptr) noexcept;
};