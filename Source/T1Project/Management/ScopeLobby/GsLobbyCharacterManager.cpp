// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLobbyCharacterManager.h"

#include "Data/GsResourceManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Item/GsItemManager.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "DataSchema/Camera/GsSchemaCustomizeCameraData.h"
#include "DataSchema/GsSchemaCharacterCreateData.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/Stage/StageLobby/GsStageManagerLobby.h"
#include "GameFlow/Stage/StageLobby/GsStageLobbyBase.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsCharacterLobbyPlayer.h"
#include "ActorEx/GsWeaponActor.h"
#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameObject/Define/EGsPartsType.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/Parts/GsPartsHandlerBase.h"
#include "GameObject/GsSchemaPartsShapeData.h"

#include "Camera/Define/EGsCameraLobbyState.h"
#include "Camera/Lobby/GsCameraHandlerLobby.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"

#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "T1Project.h"

#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"

#include "GameFramework/PlayerController.h"
#include "Classes/Camera/PlayerCameraManager.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Engine/Public/UnrealEngine.h"

//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

void UGsLobbyCharacterManager::Initialize()
{
	
}

void UGsLobbyCharacterManager::Finalize()
{
	RemoveAll();
	DespawnPlayableEventLocal();
}

void UGsLobbyCharacterManager::RemoveAll()
{
	RemoveCharacter();	
}

void UGsLobbyCharacterManager::RemoveCharacter()
{
	if (UWorld* world = GetWorld())
	{
		for (int i = 0; i < (int)CreatureGenderType::MAX; ++i)
		{
			if (_character[i]->IsValidLowLevel())
			{
				world->DestroyActor(_character[i]);
			}
			_character[i] = nullptr;
		}
	}
}

uint32 UGsLobbyCharacterManager::GetPartsID(const EGsPartsType InType, uint32 InItemId)
{
	// 아이템 정보가 있을경우
	if (0 < InItemId)
	{
		if (const FGsSchemaPartsShapeData* partsData = GItem()->FindPartsSchemaDataByTID(InItemId))
		{
			return partsData->id;
		}
	}

	return 0;
}

class AGsCharacterPlayer* UGsLobbyCharacterManager::CreateCharacter(CreatureGenderType InGender, FTransform& InSpawnTm)
{
	if (_character[(int)InGender]->IsValidLowLevel())
	{
		return _character[(int)InGender];
	}

	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return nullptr;
	}

	FString classPath = GetCharacterClassPath(InGender);
	if (TEXT("") == classPath)
	{
 		return nullptr;
 	}

	UClass* bpClass = LoadObject<UClass>(nullptr, *classPath);
 	if (nullptr == bpClass)
 	{
 		return nullptr;
 	}

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	_character[(int)InGender] = Cast<AGsCharacterPlayer>(world->SpawnActor(bpClass, &InSpawnTm, spawnParam));
	
	return _character[(int)InGender];
}

const FGsSchemaCharacterCreateData* UGsLobbyCharacterManager::GetCharacterCreateData(CreatureGenderType InGenderType) const
{
	const UGsTableCharacterCreateData* table = Cast<UGsTableCharacterCreateData>(FGsSchemaCharacterCreateData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaCharacterCreateData* schemaCharacterCreateData = nullptr;
	if (false == table->FindRowById(InGenderType, schemaCharacterCreateData))
	{
		return nullptr;
	}

	return schemaCharacterCreateData;
}

FString UGsLobbyCharacterManager::GetCharacterClassPath(CreatureGenderType InGenderType) const
{
	const FGsSchemaCharacterCreateData* row = GetCharacterCreateData(InGenderType);
	if (nullptr == row)
	{
		return TEXT("");
	}

	FString classPath = row->characterPath.ToString();
	classPath.Append(TEXT("_C"));

	return classPath;
}

// 플레이어블 이벤트 로컬 스폰
void UGsLobbyCharacterManager::SpawnPlayableEventLocal()
{
	// 로드할 bp path 구하기
	FString path =
		GData()->GetGlobalData()->_playableEventLocalBPPath.ToString();

	path += TEXT("_C");
	if (UClass* loadLocalC = LoadObject<UClass>(nullptr, *path))
	{
		if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		{
			return;
		}

		UWorld* world = GEngine->GameViewport->GetWorld();
		if (world == nullptr)
		{
			return;
		}

		// 스폰할 위치, 회전 값 구하기
		FTransform playerSpawnTransform = UGsLevelUtil::GetPlayerStart(world);

		// NetUser데이터는 GameDataManager것을 사용
		GGameData()->SetTId(1);
		FGsSpawnParamLocalPlayer testLocalParam(EGsGameObjectType::LocalPlayer, playerSpawnTransform.GetLocation(),
			playerSpawnTransform.GetRotation().Rotator(), GGameData()->GetUserData());

		//gs game object 만들기
		_playableEventLocal = NewObject<UGsGameObjectPlayableEventLocalPlayer>(this);

		// game object 초기화
		_playableEventLocal->Initialize(&testLocalParam);

		//// spawn actor
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* makeActor =
			world->SpawnActor(loadLocalC, &playerSpawnTransform, spawnParam);

		if (makeActor == nullptr)
		{
			return;
		}

		_playableEventLocal->ActorSpawned(makeActor);

		// 카메라 초기화
		if (APlayerController* controller = world->GetFirstPlayerController())
		{
			controller->SetViewTarget(makeActor);
		}
	}
}
// 플레이어블 이벤트 로컬 디스폰
void UGsLobbyCharacterManager::DespawnPlayableEventLocal()
{
	if (_playableEventLocal == nullptr)
	{
		return;
	}

	_playableEventLocal->Finalize();
	// 액터 소멸
	if (AActor* actor = _playableEventLocal->GetActor())
	{
		actor->Destroy();
		actor->MarkPendingKill();
	}

	_playableEventLocal->ClearActor();

	if (_playableEventLocal->IsRooted())
	{
		_playableEventLocal->RemoveFromRoot();
	}

	_playableEventLocal->MarkPendingKill();

	_playableEventLocal = nullptr;
}

void UGsLobbyCharacterManager::Update(float DeltaTime)
{
	if (nullptr != _playableEventLocal)
	{
		_playableEventLocal->Update(DeltaTime);
	}
}