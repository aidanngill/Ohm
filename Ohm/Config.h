#pragma once

#include <nlohmann/json.hpp>

class Config {
public:
	Config();

	bool LoadFromFile(const char* fileName);
	void DumpToFile(const char* fileName);

	struct Aim {
		bool isEnabled = false;
		bool ignoreFlash = false;
		bool silentAim = false;
		bool autoScope = false;
		bool autoShoot = false;
		float fieldOfView = 1.0f;
		float smoothAmount = 1.0f;
	} aim;

	struct Visuals {

		struct Players {
			bool isEnabled = false;
			bool isOutlined = false;
			bool hasHealth = false;
			bool hasArmor = false;
			bool hasName = false;
			bool hasDistance = false;
		} players;

		struct Entities {
			bool showBomb = false;
			bool showDroppedWeapons = false;
		} entities;

		struct Chams {
			int playerMaterial = 0;
			int weaponMaterial = 0;
			int armsMaterial = 0;
		} chams;

		struct Glow {
			bool showPlayers = false;
			bool showDefuseKits = false;
			bool showPlantedC4 = false;
			bool showDroppedWeapons = false;
			bool showChickens = false;
			float bloomAmount = 1.f;
		} glow;

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