#include <windows.h>
#include "Hooks.h"
#include "Utilities/MinHook.h"
#include "GameInfo/GameInfo.h"
#include "PakLoader.h"
#include "Memory/mem.h"
#include "UnrealEngineModLoader/Utilities/Globals.h"
#include "UE4/Ue4.hpp"
#include "LoaderUI.h"

bool GameStateClassInitNotRan = true;

namespace Hooks
{
	namespace HookedFunctions
	{
		PVOID(*origInitGameState)(void*);
		PVOID hookInitGameState(void* Ret)
		{
			Log::Info("GameStateHook");
			if (GameStateClassInitNotRan)
			{
				UE4::InitSDK();
				Log::Info("Engine Classes Loaded");

				if (GameProfile::SelectedGameProfile.StaticLoadObject)
				{
					Log::Info("StaticLoadObject Found");
				}
				else
				{
					Log::Error("StaticLoadObject Not Found");
				}
				GameStateClassInitNotRan = false;
			}

			// Clean up existing mod actors on map change
			for (int i = 0; i < Global::GetGlobals()->ModInfoList.size(); i++)
			{
				Global::GetGlobals()->ModInfoList[i].CurrentModActor = nullptr;
			}

			Log::Info("ModInfoList Size: %d", Global::GetGlobals()->ModInfoList.size());

			if (GameProfile::SelectedGameProfile.StaticLoadObject)
			{
				UE4::FTransform transform;
				transform.Translation = UE4::FVector(0, 0, 0);
				transform.Rotation = UE4::FQuat(0, 0, 0, 0);
				transform.Scale3D = UE4::FVector(1, 1, 1);
				UE4::FActorSpawnParameters spawnParams = UE4::FActorSpawnParameters::FActorSpawnParameters();

				for (int i = 0; i < Global::GetGlobals()->ModInfoList.size(); i++)
				{
					std::wstring CurrentMod = Global::GetGlobals()->ModInfoList[i].ModName;
					std::string str(CurrentMod.begin(), CurrentMod.end());
					Log::Info("Processing Mod: %s (Enabled: %d)", str.c_str(), Global::GetGlobals()->ModInfoList[i].IsEnabled);
					
					if (Global::GetGlobals()->ModInfoList[i].IsEnabled)
					{
						const std::wstring Path = L"/Game/Mods/" + CurrentMod + L"/ModActor.ModActor_C";
						std::string pathStr(Path.begin(), Path.end());
						Log::Info("Attempting to load: %s", pathStr.c_str());
						
						UE4::UClass* ModObject = UE4::UClass::LoadClassFromString(Path.c_str(), false);
						Log::Info("LoadClassFromString result: 0x%p", ModObject);
						
						if (ModObject)
						{
							UE4::AActor* ModActor = nullptr;
							if (!GameProfile::SelectedGameProfile.IsUsingDeferedSpawn)
							{
								Log::Info("Using SpawnActor");
								ModActor = UE4::UWorld::GetWorld()->SpawnActor(ModObject, &transform, &spawnParams);
							}
							else
							{
								Log::Info("Using BeginDeferredActorSpawnFromClass");
								ModActor = UE4::UGameplayStatics::BeginDeferredActorSpawnFromClass(ModObject, transform, UE4::ESpawnActorCollisionHandlingMethod::AlwaysSpawn, nullptr);
							}

							Log::Info("SpawnActor result: 0x%p", ModActor);
							if (ModActor)
							{
								Global::GetGlobals()->ModInfoList[i].CurrentModActor = ModActor;
								Log::Info("Successfully Loaded %s", str.c_str());
							}
							else
							{
								Log::Error("Failed to spawn ModActor for %s", str.c_str());
							}
						}
						else
						{
							Log::Info("Could not locate ModActor for %s", str.c_str());
						}
					}
				}
				Log::Info("Finished Spawning PakMods");
			}
			Log::Info("Returning to GameState --------------------------------------------------------");
			return origInitGameState(Ret);
		}
	};

	DWORD __stdcall InitHooks(LPVOID)
	{
		MinHook::Init();
		Log::Info("MinHook Setup");
		Sleep(10);
		PakLoader::ScanLoadedPaks();
		Log::Info("ScanLoadedPaks Setup");
		MinHook::Add(GameProfile::SelectedGameProfile.GameStateInit, &HookedFunctions::hookInitGameState, &HookedFunctions::origInitGameState, "AGameModeBase::InitGameState");
		LoaderUI::GetUI()->CreateUILogicThread();
		if (!GameProfile::SelectedGameProfile.bDelayGUISpawn)
		{
			LoaderUI::HookDX();
		}
		return NULL;
	}

	void SetupHooks()
	{
		Log::Info("Setting Up Loader");
		CreateThread(0, 0, InitHooks, 0, 0, 0);
	}
};