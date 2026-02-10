#include "Mod.h"
#include "Utilities/MinHook.h"
#include "Utilities/Version.h"

Mod* Mod::ModRef;

void Mod::InitGameState()
{
}

void Mod::OnModMenuButtonPressed()
{
}

void Mod::BeginPlay(UE4::AActor* Actor)
{
}

void Mod::PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor)
{
}

void Mod::DX11Present(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView)
{
}

void Mod::DrawImGui()
{
}

void Mod::SetupHooks()
{
	// Event system removed for 4.16 compatibility
}

void Mod::CompleteModCreation()
{
	IsFinishedCreating = true;
	Log::Info("Mod Created: %s", ModName.c_str());
	if (ModLoaderVersion != MODLOADER_VERSION)
	{
		Log::Warn("Mod: %s was created with a different version of the Unreal Mod Loader. This mod may be unstable.", ModName.c_str());
	}
}

void Mod::InitializeMod()
{

}