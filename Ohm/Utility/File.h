#pragma once

namespace fs = std::filesystem;

bool ReadFileToString(const char* fileName, std::string& contents);

fs::path GetCheatFolder();
std::wstring GetDocumentsFolder();
std::wstring CurrentWorkingDirectory();
std::string MakeConfigPath(std::string fileName);