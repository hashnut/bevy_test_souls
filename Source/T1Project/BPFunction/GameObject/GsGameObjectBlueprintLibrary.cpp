#include "GsGameObjectBlueprintLibrary.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/GsMessageHolder.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "Gameobject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Define/GsGameObjectStruct.h"

#include "Input/GsInputEventMsgBase.h"
#include "Item/GsItemManager.h"

void UGsGameObjectBlueprintLibrary::SpawnPlayer(UClass* ActorClass, int64 InGameId, const FVector& StartPos, const FVector& Offset)
{
	//GSGameObject()->SpawnObject(FGsSpawnParam(EGsGameObjectType::LocalPlayer, InGameId, StartPos + Offset, FRotator(0.f, 0.f, 0.f)));
}

UGsGameObjectBase* UGsGameObjectBlueprintLibrary::SpawnNonPlayer(UClass* ActorClass, int64 InGameId, const FVector& StartPos, const FVector& Offset)
{
	//return GSGameObject()->SpawnObject(FGsSpawnParam(EGsGameObjectType::NonPlayer, InGameId, StartPos, FRotator(0.f, 0.f, 0.f)));
	return nullptr;
}

UGsGameObjectBase* UGsGameObjectBlueprintLibrary::SpawnVehicle(UClass* ActorClass, int64 InGameId, const FVector& StartPos, const FVector& Offset)
{
	//return GSGameObject()->SpawnObject(FGsSpawnParam(EGsGameObjectType::Vehicle, InGameId, StartPos, FRotator(0.f, 0.f, 0.f)));
	return nullptr;
}

//TArray<int32> UGsGameObjectBlueprintLibrary::GetCurrentLocalMeshPath()
UGsGameObjectLocalPlayer* UGsGameObjectBlueprintLibrary::GetCurrentObjectLocalPlayer()
{
	if (GSGameObject() != nullptr)
	{
		UGsGameObjectBase* LocalPlayerObjBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (LocalPlayerObjBase)
		{
			return LocalPlayerObjBase->CastGameObject< UGsGameObjectLocalPlayer>();
		}
	}

	return nullptr;
}

bool UGsGameObjectBlueprintLibrary::HoldStreamingLevelbyLocation(const FVector& TargetLocation)
{
	if (nullptr == GLevel())
	{
		GSLOG(Error, TEXT("nullptr == GLevel()  Try GsGameModeWorld !!"));
		return false;
	}
	GLevel()->HoldStreamingLevelByLocation(TargetLocation);
	return true;
}

bool UGsGameObjectBlueprintLibrary::HoldStreamingLevelbyLocalPlayer()
{
	if (nullptr == GSGameObject() )
	{
		GSLOG(Error, TEXT("nullptr == GSGameObject()  Try GsGameModeWorld !!"));
		return false;
	}

	if (nullptr == GLevel())
	{
		GSLOG(Error, TEXT("nullptr == GLevel()  Try GsGameModeWorld !!"));
		return false;
	}

	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		GLevel()->HoldStreamingLevelByLocation(player->GetLocation());
		return true;
	}
	return false;
}

void UGsGameObjectBlueprintLibrary::UnHoldStreamingLevel()
{
	if (nullptr == GLevel())
	{
		GSLOG(Error, TEXT("nullptr == GLevel()  Try GsGameModeWorld !!"));
		return;
	}
	GLevel()->UnHoldStreamingLevel();
}

void UGsGameObjectBlueprintLibrary::ChangeCameraMode()
{
	FGsInputEventMsgBase msg;
	GMessage()->GetInput().SendMessage(MessageInput::ViewChange, msg);
}