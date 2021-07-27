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

bool Config::LoadFromFile(const char* file_name) {
	struct stat info;

	if (stat(file_name, &info) != 0) {
		return false;
	}
	else if (info.st_mode & S_IFDIR) {
		return false;
	}

	std::string src;

	if (!ReadFileToString(file_name, src))
		return false;

	std::vector<uint8_t> dec_first = base64::decode(src);
	std::string dec_final(dec_first.begin(), dec_first.end());

	data = nlohmann::json::parse(dec_final);

	if (data["aimbot"]["enabled"].is_boolean()) aim.enabled = data["aimbot"]["enabled"].get<bool>();
	if (data["aimbot"]["fov"].is_number_float()) aim.fov = data["aimbot"]["fov"].get<float>();

	if (data["visuals"]["box"]["enabled"].is_boolean()) visuals.box.enabled = data["visuals"]["box"]["enabled"].get<bool>();
	if (data["visuals"]["box"]["outlined"].is_boolean()) visuals.box.outlined = data["visuals"]["box"]["outlined"].get<bool>();
	if (data["visuals"]["box"]["health"].is_boolean()) visuals.box.health = data["visuals"]["box"]["health"].get<bool>();
	if (data["visuals"]["box"]["armor"].is_boolean()) visuals.box.armor = data["visuals"]["box"]["armor"].get<bool>();
	if (data["visuals"]["box"]["name"].is_boolean()) visuals.box.name = data["visuals"]["box"]["name"].get<bool>();
	if (data["visuals"]["box"]["distance"].is_boolean()) visuals.box.distance = data["visuals"]["box"]["distance"].get<bool>();

	if (data["visuals"]["enemy_only"].is_boolean()) visuals.enemy_only = data["visuals"]["enemy_only"].get<bool>();
	if (data["visuals"]["on_death"].is_boolean()) visuals.on_death = data["visuals"]["on_death"].get<bool>();
	if (data["visuals"]["snap_lines"].is_boolean()) visuals.snap_lines = data["visuals"]["snap_lines"].get<bool>();

	if (data["misc"]["bunny_hop"].is_boolean()) misc.bunny_hop = data["misc"]["bunny_hop"].get<bool>();

	return true;
}

void Config::DumpToFile(const char* file_name) {
	nlohmann::json od = {
		{"aimbot", {
			{"enabled", aim.enabled},
			{"fov", aim.fov}
		}},
		{"visuals", {
			{"box", {
				{"enabled", visuals.box.enabled},
				{"outlined", visuals.box.outlined},
				{"health", visuals.box.health},
				{"armor", visuals.box.armor},
				{"name", visuals.box.name},
				{"distance", visuals.box.distance},
			}},
			{"enemy_only", visuals.enemy_only},
			{"on_death", visuals.on_death},
			{"snap_lines", visuals.snap_lines}
		}},
		{"misc", {
			{"bunny_hop", misc.bunny_hop}
		}}
	};

	std::ofstream out(file_name);
	out << base64::encode(od.dump());
	out.close();
}