#include <iostream>
#include <fstream>
#include <filesystem>

#include <cppcodec/base64_rfc4648.hpp>

#include "./Config.h"

#include "./Utility/File.h"

namespace fs = std::filesystem;
using base64 = cppcodec::base64_rfc4648;

Config::Config() {
	fs::path dir = GetCheatFolder();

	if (!fs::exists(dir))
		fs::create_directory(dir);

	fs::path file("config.json");
	fs::path config = dir / file;

	std::string path{ config.u8string() };

	if (!LoadFromFile(path.c_str()))
		DumpToFile(path.c_str());
}

bool Config::LoadFromFile(const char* fileName) {
	struct stat info;

	if (stat(fileName, &info) != 0) {
		return false;
	}
	else if (info.st_mode & S_IFDIR) {
		return false;
	}

	std::string src;

	if (!ReadFileToString(fileName, src))
		return false;

	std::vector<uint8_t> bsfDecoded = base64::decode(src);
	std::string bsfString(bsfDecoded.begin(), bsfDecoded.end());

	data = nlohmann::json::parse(bsfString);

	if (data["aimbot"]["enabled"].is_boolean()) aim.isEnabled = data["aimbot"]["enabled"].get<bool>();
	if (data["aimbot"]["fov"].is_number_float()) aim.fieldOfView = data["aimbot"]["fov"].get<float>();

	if (data["visuals"]["players"]["enabled"].is_boolean()) visuals.players.isEnabled = data["visuals"]["players"]["enabled"].get<bool>();
	if (data["visuals"]["players"]["outlined"].is_boolean()) visuals.players.isOutlined = data["visuals"]["players"]["outlined"].get<bool>();
	if (data["visuals"]["players"]["health"].is_boolean()) visuals.players.hasHealth = data["visuals"]["players"]["health"].get<bool>();
	if (data["visuals"]["players"]["armor"].is_boolean()) visuals.players.hasArmor = data["visuals"]["players"]["armor"].get<bool>();
	if (data["visuals"]["players"]["name"].is_boolean()) visuals.players.hasName = data["visuals"]["players"]["name"].get<bool>();
	if (data["visuals"]["players"]["distance"].is_boolean()) visuals.players.hasDistance = data["visuals"]["players"]["distance"].get<bool>();

	if (data["visuals"]["entities"]["bomb"].is_boolean()) visuals.entities.showBomb = data["visuals"]["entities"]["bomb"].get<bool>();
	if (data["visuals"]["entities"]["dropped_weapon"].is_boolean()) visuals.entities.showDroppedWeapons = data["visuals"]["entities"]["dropped_weapon"].get<bool>();

	if (data["visuals"]["chams"]["player_material"].is_number_integer()) visuals.chams.playerMaterial = data["visuals"]["chams"]["player_material"].get<int>();
	if (data["visuals"]["chams"]["weapon_material"].is_number_integer()) visuals.chams.weaponMaterial = data["visuals"]["chams"]["weapon_material"].get<int>();
	if (data["visuals"]["chams"]["arms_material"].is_number_integer()) visuals.chams.armsMaterial = data["visuals"]["chams"]["arms_material"].get<int>();

	if (data["visuals"]["enemy_only"].is_boolean()) visuals.isOnlyEnemy = data["visuals"]["enemy_only"].get<bool>();
	if (data["visuals"]["on_death"].is_boolean()) visuals.isOnDeath = data["visuals"]["on_death"].get<bool>();
	if (data["visuals"]["snap_lines"].is_boolean()) visuals.hasSnapLines = data["visuals"]["snap_lines"].get<bool>();

	if (data["misc"]["bunny_hop"].is_boolean()) misc.bunnyHop = data["misc"]["bunny_hop"].get<bool>();

	return true;
}

void Config::DumpToFile(const char* fileName) {
	nlohmann::json od = {
		{"aimbot", {
			{"enabled", aim.isEnabled},
			{"fov", aim.fieldOfView}
		}},
		{"visuals", {
			{"players", {
				{"enabled", visuals.players.isEnabled},
				{"outlined", visuals.players.isOutlined},
				{"health", visuals.players.hasHealth},
				{"armor", visuals.players.hasArmor},
				{"name", visuals.players.hasName},
				{"distance", visuals.players.hasDistance},
			}},
			{"entities", {
				{"bomb", visuals.entities.showBomb},
				{"dropped_weapon", visuals.entities.showDroppedWeapons}
			}},
			{"enemy_only", visuals.isOnlyEnemy},
			{"on_death", visuals.isOnDeath},
			{"snap_lines", visuals.hasSnapLines}
		}},
		{"misc", {
			{"bunny_hop", misc.bunnyHop}
		}}
	};

	std::ofstream out(fileName);
	out << base64::encode(od.dump());
	out.close();
}