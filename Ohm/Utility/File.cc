#include <string>
#include <fstream>
#include <filesystem>

#include <Windows.h>
#include <shlobj.h>

#include "./File.h"

bool ReadFileToString(const char* file_name, std::string& contents) {
	std::ifstream in(file_name, std::ios::in | std::ios::binary);

	if (!in) return false;

	in.seekg(0, std::ios::end);
	contents.resize(static_cast<const unsigned int>(in.tellg()));
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();

	return true;
}

std::wstring GetDocumentsFolder() {
	PWSTR path;
	HRESULT result = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
	std::wstring folder_path;

	if (SUCCEEDED(result))
		folder_path = path;

	CoTaskMemFree(path);

	return folder_path;
}

fs::path GetCheatFolder() {
	fs::path base(GetDocumentsFolder());
	fs::path dir("Ohm");

	return base / dir;
}

std::wstring CurrentWorkingDirectory() {
	TCHAR tmp[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, tmp);

	return std::wstring(tmp);
}

std::string MakeConfigPath(std::string fileName) {
	fs::path dir = GetCheatFolder();

	if (!fs::exists(dir))
		fs::create_directory(dir);

	fs::path file(fileName);
	fs::path cfg = dir / file;

	return cfg.u8string();
}