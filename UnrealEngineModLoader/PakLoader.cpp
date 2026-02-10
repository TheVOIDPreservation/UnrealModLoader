#include "PakLoader.h"
#include <filesystem>
#include "Utilities/Globals.h"

namespace fs = std::filesystem;

namespace PakLoader
{
	void ScanLoadedPaks()
	{
		char path_c[MAX_PATH];
		GetModuleFileNameA(NULL, path_c, MAX_PATH);
		std::string path = std::string(path_c);
		Log::Info("Executable Path: %s", path.c_str());
		path = path.substr(0, path.find_last_of("/\\"));
		path = path.substr(0, path.find_last_of("/\\"));
		path = path.substr(0, path.find_last_of("/\\"));
		path = path + "\\Content\\Paks\\LogicMods\\";
		Log::Info("Scanning for LogicMods in: %s", path.c_str());
		if (!std::filesystem::exists(path))
		{
			Log::Info("LogicMods folder does not exist, creating it.");
			std::filesystem::create_directory(path);
		}
		if(Global::GetGlobals()->ModInfoList.size() >= 1)
			Global::GetGlobals()->ModInfoList.empty();
		
		int pakCount = 0;
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (entry.path().extension().string() == ".pak")
			{
				pakCount++;
				std::wstring filename = entry.path().filename().wstring();
				std::wstring modNameW = filename.substr(0, filename.length() - 4);
				if (modNameW.substr(modNameW.length() - 2, 2) == L"_p")
					modNameW = modNameW.substr(0, modNameW.length() - 2);
				if (modNameW.substr(modNameW.length() - 2, 2) == L"_P")
					modNameW = modNameW.substr(0, modNameW.length() - 2);
				std::string str(modNameW.begin(), modNameW.end());
				Log::Info("PakModLoaded: %s", str.c_str());
				ModInfo CurrentMod;
				CurrentMod.ModName = modNameW;
				CurrentMod.IsEnabled = true;
				Global::GetGlobals()->ModInfoList.push_back(CurrentMod);
			}
		}
		Log::Info("Total Pak Mods Found: %d", pakCount);
	}
};