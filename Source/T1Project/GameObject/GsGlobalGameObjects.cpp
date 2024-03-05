#include "GsGlobalGameObjects.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

void UGsGlobalGameObjects::Initialize()
{
}

void UGsGlobalGameObjects::Finalize()
{
}

void UGsGlobalGameObjects::Shutdown()
{
	RemoveLocalPlayer();
}

void UGsGlobalGameObjects::RemoveLocalPlayer()
{
	_localPlayer = nullptr;
}
