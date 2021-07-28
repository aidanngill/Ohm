#pragma once

#include <cstddef>

class CUserCmd;

class CGlobalVars {
public:
	const float realTime;
	const int frameCount;
	const float absoluteFrameTime;
	const std::byte pad[4];
	float currentTime;
	float frameTime;
	const int maxClients;
	const int tickCount;
	const float intervalPerTick;

	float ServerTime(CUserCmd* = nullptr) noexcept;
};