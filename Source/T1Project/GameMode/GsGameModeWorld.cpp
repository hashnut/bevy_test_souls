// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameModeWorld.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/WorldComposition.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "Runtime/Engine/Classes/Engine/LevelScriptActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"
#include "Runtime/Landscape/classes/Landscape.h"
#include "Runtime/Core/Public/Math/Box.h"
#include "GameFramework/PlayerStart.h"
#include "NavigationSystem.h"

#include "T1Project.h"
#include "GsGameInstance.h"
#include "Level/TargetPoint/PlayerSpawnPoint.h"
#include "Level/LevelScriptActor/GsLevelScriptActor.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"

#include "ControllerEx/GsPlayerController.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/GsMessageHolder.h"

#include "Cheat/GsCheatManager.h"

//network
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Net/GsNetSendService.h"
#include "GameObject/HUD/GsHUDLocalPlayer.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "LandscapeStreamingProxy.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "DrawDebugHelpers.h"
#include "Camera/PlayerCameraManager.h"

#include "UTIL/GsLevelUtil.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#define GAMEMODE_WORLD_DEBUG false

const FVector TERRAIN_GAP = FVector(0.f, 0.f, 10000.f);

//------------------------------------------------------------------
AGsGameModeWorld::AGsGameModeWorld()
{
	PrimaryActorTick.bCanEverTick = true;

	_worldComponent = CreateDefaultSubobject<UGsWorldComponent>(TEXT("WorldComponent"));

	HUDClass = AGsHUDLocalPlayer::StaticClass();

	PlayerControllerClass = AGsPlayerController::StaticClass();
}


//------------------------------------------------------------------
void AGsGameModeWorld::StartPlay()
{
	Super::StartPlay();

#if SPAWN_LOCATION_DEBUG
	UGsLevelManager::SetIsPlayerSpawnComplete(false);
#endif

	InitSignificanceManager();
	StartSpawnPlayer();

	//Spawn player for update streaming level
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void AGsGameModeWorld::EndPlay(EEndPlayReason::Type inType)
{
	//Lobby로 전환시 GameMode Scope가 해제되어 매니저가 null이된다. 매니저가 null인지 체크해야한다.
	if (FGsClientSpawnManager* clientSpawnManager = GSClientSpawn())
	{
		clientSpawnManager->Clear();
	}

	if (UGsGameObjectManager* gameObjectManager = GSGameObject())
	{
		UGsGameObjectBase* find = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(find))
		{
			if (FGsFenceHandler* fenceHandler = localPlayer->GetFenceHandler())
			{
				fenceHandler->SetActive(false);
				fenceHandler->Finalize();
			}
		}
	}

	ClearWaitTerrainLoadFinish();
	GMessage()->GetSystem().SendMessage(MessageSystem::GAMEMODE_WORLD_EXIT);

	Super::EndPlay(inType);
}

void AGsGameModeWorld::StartSpawnPlayer(bool inIsSameLevel)
{
	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		GSLOG(Error, TEXT("nullptr == world"));
		return;
	}

	_playerSpawnTransform = UGsLevelUtil::GetPlayerSpawnTransformFromServer();

	AGsPlayerController* pController = nullptr;
	if (UGameInstance* instance = world->GetGameInstance())
	{
		if (ULocalPlayer* pPlayer = instance->FindLocalPlayerFromControllerId(0))
		{
			pController = Cast<AGsPlayerController>(pPlayer->PlayerController);
			if (pController)
			{
				pController->SetInitialSpawnLocation(_playerSpawnTransform.GetLocation());
			}
		}
		else
		{
			GSLOG(Error, TEXT("pPlayer == nullptr"));
		}
	}

#if false
	//월드 컴포지션 맵 갱신시 플레이어 컨트롤러와 카메라 매니저 위치를 통해 갱신
	//스폰시 위치를 카메라 위치로 갱신 시켜줘야 한다. 
	APlayerCameraManager* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (camera)
	{
		FMinimalViewInfo pov;
		pov.Location = _playerSpawnTransform.GetLocation();
		camera->SetCameraCachePOV(pov);
	}

	FVector camLocation = FVector::ZeroVector;
	FRotator camRotation = FRotator::ZeroRotator;
	if (camera)
	{
		camera->GetCameraViewPoint(camLocation, camRotation);
	}
	GSLOG(Log, TEXT("StartSpawnPlayer - camera location : %s"), *camLocation.ToString());
#endif

	GSLOG(Log, TEXT("StartSpawnPlayer - spawn location : %s"), *_playerSpawnTransform.GetLocation().ToString());

#if WITH_EDITOR
	if (pController)
	{
		if (AActor* viewTarget = pController->GetViewTarget())
		{
			GSLOG(Log, TEXT("StartSpawnPlayer - view target - label : %s\tlocation : %s"), *viewTarget->GetActorLabel(), *viewTarget->GetActorLocation().ToString());
		}
	}
//#else
//	GSLOG(Log, TEXT("StartSpawnPlayer - view target location : %s"), *viewTarget->GetActorLocation().ToString());
#endif

	//월드 컴포지션 맵 갱신
	if (UWorldComposition* worldcomposition = world->WorldComposition)
	{
		// 강제 GC 호출
		// Pending 되어 있는 스트리밍 레벨 (LevelsPendingUnload) 정리가 필요하여 호출
		// 이함수를 제거해야하는 상황이 된다면 스트리밍 레벨 같은 공간 로드/언로드 구간의 확인이 필요.
		GEngine->ForceGarbageCollection(true);

		worldcomposition->UpdateStreamingState(_playerSpawnTransform.GetLocation());
	}

	//같은 리소스 레벨안에서 맵 이동 하면 디자인 레벨을 재로딩한다.	
	if (false == inIsSameLevel)
	{
		UnloadDesignStreamingLevel();
		ClearDesignSteamingLevel();
		TryLoadCurrentDesignStreamingLevel();
	}

	//Deactive until spawn me complete come
	if (FGsClientSpawnManager* clientSpawnMaanger = GSClientSpawn())
	{
		clientSpawnMaanger->SetActive(false);
	}

	//실제로 맵을 갱신, 이 함수 호출시 완전히 로드됨
	GetWorld()->FlushLevelStreaming(EFlushLevelStreamingType::Full);

#ifdef OLD_TERRAIN_LOAD_FINISH
 
#if SPAWN_LOCATION_DEBUG
#if false
	GetWorld()->GetTimerManager().SetTimer(_timeHandle, this, &AGsGameModeWorld::CheckTerrainLoad, 0.2f, true);
#else
	TerrainLoadFinish();
#endif
#else	
	TerrainLoadFinish();
#endif	

#else
	bool isSameArea = false;
	UGsLevelManager* levelManager = GLevel();
	if (levelManager  != nullptr)
	{
		isSameArea = levelManager->IsSameArea();
	}

	if(isSameArea == false)
	{
		TerrainLoadFinish();
	}
	else
	{ 
		GSLOG(Error, TEXT("isSameArea == true"));

		// 처음 체크에서 잘되면 바로 진행 
		// 안되면 대기하면서 진행
		if(true == IsTerrainLoaded())
		{
			TerrainLoadFinish();
		}
		else
		{
			// 기존꺼 삭제
			ClearWaitTerrainLoadFinish();

			_waitTotalCount = 0;
		
			world->GetTimerManager().SetTimer(_waitTerrainLoadFinish, this, &AGsGameModeWorld::CheckTerrainLoadByTrySweepToLand, 0.2f, true);
		}
	}
#endif
}


void AGsGameModeWorld::TerrainLoadFinish()
{
#if SPAWN_LOCATION_DEBUG
	_playerSpawnTransform = UGsLevelUtil::GetPlayerSpawnTransformFromServer();
	if (UGameInstance* instance = GetWorld()->GetGameInstance())
	{
		if (ULocalPlayer* pPlayer = instance->FindLocalPlayerFromControllerId(0))
		{
			if (AGsPlayerController* pController = Cast<AGsPlayerController>(pPlayer->PlayerController))
			{
				UGsLevelUtil::DrawDebugLocation(GetWorld(), pController->GetSpawnLocation(), pController, TEXT("Before load to fininsh"), FColor::Red);
			}
		}
	}

	UGsLevelManager::SetIsPlayerSpawnComplete(false);
#endif	

	Send_CG_ACK_LOAD_TOPO_FIN();
	PlayerGameStart();
}

//플레이어가 정상적으로 스테틱 메쉬 위에 스폰했는지 체크하기 위한 디버깅용 함수
void AGsGameModeWorld::CheckTerrainLoad()
{
#if SPAWN_LOCATION_DEBUG
	_playerSpawnTransform = UGsLevelUtil::GetPlayerSpawnTransformFromServer();
	FVector origin = _playerSpawnTransform.GetLocation();
	if (UGameInstance* instance = GetWorld()->GetGameInstance())
	{
		if (ULocalPlayer* pPlayer = instance->FindLocalPlayerFromControllerId(0))
		{
			if (AGsPlayerController* pController = Cast<AGsPlayerController>(pPlayer->PlayerController))
			{
				FHitResult hit;
				if (UGsLevelUtil::TryLineTraceToLand(hit, GetWorld(), origin, origin.Z, origin.Z - 1000))
				{
#if false
					GetWorld()->GetTimerManager().ClearTimer(_waitPlayerSpawnTimer);
#endif

					GSLOG(Log, TEXT("CheckTerrainLoad finish"));
				}
			}
		}
	}
#endif
}

void AGsGameModeWorld::PlayerGameStart()
{
	GMessage()->GetSystem().SendMessage(MessageSystem::GAMEMODE_WORLD_START_PLAY);
}

void AGsGameModeWorld::Send_CG_ACK_LOAD_TOPO_FIN()
{
	int32 levelKey = GLevel()->GetCurrentLevelId();
	FGsNetSendService::SendGatewayLoadTopoFin(levelKey);

	GMessage()->GetSystem().SendMessage(MessageSystem::AFTER_LOAD_TOPO_FIN);
}
// 인게임에서 pc 스폰전에 해당 위치 레벨 로드 되었는지 체크
// TrySweepToLand 함수 사용하기 위해 실린더 radius, height 값 저장해놓는다	
// FGsSchemaCreatureCommonInfo 테이블 찾을때 1, 2 가 pc 남, 여 키값인데 
// 일단은 gender 값이랑 같아서 그걸로 찾자 (로비에서 정확히 가리키는 값있다면 그걸로 변경 필요)
// https://jira.com2us.com/jira/browse/C2URWQ-5675
void AGsGameModeWorld::CheckTerrainLoadByTrySweepToLand()
{
	if (true == IsTerrainLoaded())
	{
		ClearWaitTerrainLoadFinish();
		// 로드 성공 호출
		TerrainLoadFinish();
	}
	else
	{
		// 실패 횟수 체크
		++_waitTotalCount;
		// 10회(2초) 지났으면

		int waitCount =
			GData()->GetGlobalData()->_waitCountLoadTerrain;

		if (_waitTotalCount >= waitCount)
		{
			// 끝
			ClearWaitTerrainLoadFinish();
			// 로드 성공 호출
			TerrainLoadFinish();
		}
	}
}
void AGsGameModeWorld::ClearWaitTerrainLoadFinish()
{
	GSLOG(Log, TEXT("[ClearWaitTerrainLoadFinish]"));

	// 타이머 제거
	UWorld* world = GetWorld();
	if (nullptr != world)
	{
		if (_waitTerrainLoadFinish.IsValid() == true)
		{
			world->GetTimerManager().ClearTimer(_waitTerrainLoadFinish);
			_waitTerrainLoadFinish.Invalidate();
		}
	}
}
bool AGsGameModeWorld::IsTerrainLoaded()
{
	GSLOG(Log, TEXT("[IsTerrainLoaded]"));

	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return false;
	}

	UGsLevelManager* levelManager = GLevel();
	if (levelManager == nullptr)
	{
		return false;
	}

	FHitResult hit;
	// 높이값 여유롭게 4배
	// 서버 에서 더 높게 내려줄수 있어서
	bool isLoaded = UGsLevelUtil::TrySweepToLand(hit, world, _playerSpawnTransform.GetLocation(), 
		levelManager->GetLocalCylinderRadius(), levelManager->GetLocalCylinderHeight() * 4.0f);
	return isLoaded;
}