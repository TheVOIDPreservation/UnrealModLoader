#pragma once
#include "UE4.hpp"
#include <map>
#include <iostream>
#include <string>
#include "../UMLDefs.h"
#include "../Mod/Mod.h"

struct ModInfo
{
	std::wstring ModName;
	std::string ModAuthor = "Unknown Author";
	std::string ModDescription = "No Description";
	std::string ModVersion = "Unknown Version";
	UE4::AActor* CurrentModActor = nullptr;
	bool IsEnabled = 0;
	bool ContainsButton = 0;
	bool WasInitialized = 0;
	std::vector<std::string> ModButtons;
};

class LOADER_API Global {
public:
	std::vector<ModInfo> ModInfoList;
	std::string GameName;
	bool bIsMenuOpen;

	static Global* GetGlobals();


private:
	static Global* glb;
};
