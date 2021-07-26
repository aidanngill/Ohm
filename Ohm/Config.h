#pragma once

#include <nlohmann/json.hpp>

class Config {
public:
	Config();

	bool LoadFromFile(const char* file_name);
	void DumpToFile(const char* file_name);

	struct Aim {
		bool enabled = false;
		float fov = 1.0f;
	} aim;

	struct Visuals {

		struct Box {
			bool enabled = false;
			bool outlined = false;
			bool health = false;
			bool armor = false;
			bool name = false;
			bool distance = false;
		} box;

		bool enemy_only = false;
		bool on_death = false;
		bool snap_lines = false;

	} visuals;

	struct Misc {
		bool bunny_hop = false;
	} misc;

private:
	nlohmann::json data;
};

inline std::unique_ptr<Config> config;