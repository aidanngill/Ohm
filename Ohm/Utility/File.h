#pragma once

namespace fs = std::filesystem;

bool ReadFileToString(const char* file_name, std::string& contents);
std::wstring GetDocumentsFolder();
fs::path GetCheatFolder();
std::wstring CurrentWorkingDirectory();