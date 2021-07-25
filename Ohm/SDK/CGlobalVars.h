#pragma once

#include <cstddef>

class CUserCmd;

class CGlobalVars {
public:
	const float real_time;
	const int frame_count;
	const float absolute_frame_time;
	const std::byte pad[4];
	float current_time;
	float frame_time;
	const int max_clients;
	const int tick_count;
	const float interval_per_tick;

	float ServerTime(CUserCmd* = nullptr) noexcept;
};