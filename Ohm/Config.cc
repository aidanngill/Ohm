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

	if (data["visuals"]["box"]["enabled"].is_boolean()) visuals.box.isEnabled = data["visuals"]["box"]["enabled"].get<bool>();
	if (data["visuals"]["box"]["outlined"].is_boolean()) visuals.box.isOutlined = data["visuals"]["box"]["outlined"].get<bool>();
	if (data["visuals"]["box"]["health"].is_boolean()) visuals.box.hasHealth = data["visuals"]["box"]["health"].get<bool>();
	if (data["visuals"]["box"]["armor"].is_boolean()) visuals.box.hasArmor = data["visuals"]["box"]["armor"].get<bool>();
	if (data["visuals"]["box"]["name"].is_boolean()) visuals.box.hasName = data["visuals"]["box"]["name"].get<bool>();
	if (data["visuals"]["box"]["distance"].is_boolean()) visuals.box.hasDistance = data["visuals"]["box"]["distance"].get<bool>();

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
			{"box", {
				{"enabled", visuals.box.isEnabled},
				{"outlined", visuals.box.isOutlined},
				{"health", visuals.box.hasHealth},
				{"armor", visuals.box.hasArmor},
				{"name", visuals.box.hasName},
				{"distance", visuals.box.hasDistance},
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