#include <fstream>
#include <Windows.h>
#include <tchar.h>

#include "./Config.h"

Config::Config() {
	TCHAR tmp[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, tmp);

	_tprintf(L"[+] Current directory @ `%s`\n", tmp);

	if (!LoadFromFile("config.json"))
		DumpToFile("config.json");
}

bool Config::LoadFromFile(const char* file_name) {
	struct stat info;

	if (stat(file_name, &info) != 0) {
		printf("[x] Failed to load data from `%s` (non-existent).\n", file_name);
		return false;
	}
	else if (info.st_mode & S_IFDIR) {
		printf("[x] Failed to load data from `%s` (was directory).\n", file_name);
		return false;
	}

	std::ifstream in(file_name);
	in >> data;

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

	in.close();

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
	out << od;
	out.close();

	printf("[+] Dumped current config to `%s`\n", file_name);
}