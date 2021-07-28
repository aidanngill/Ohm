#pragma once

#include <nlohmann/json.hpp>

class Config {
public:
	Config();

	bool LoadFromFile(const char* fileName);
	void DumpToFile(const char* fileName);

	struct Aim {
		bool isEnabled = false;
		float fieldOfView = 1.0f;
	} aim;

	struct Visuals {

		struct Box {
			bool isEnabled = false;
			bool isOutlined = false;
			bool hasHealth = false;
			bool hasArmor = false;
			bool hasName = false;
			bool hasDistance = false;
		} box;

		bool isOnlyEnemy = false;
		bool isOnDeath = false;
		bool hasSnapLines = false;

	} visuals;

	struct Misc {
		bool bunnyHop = false;
	} misc;

private:
	nlohmann::json data;
};

inline std::unique_ptr<Config> config;