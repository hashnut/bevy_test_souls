// Fill out your copyright notice in the Description page of Project Settings.

#include "GsLevelManager.h"

#include "GameMode/GsGameModeWorld.h"

#include "T1Project.h"
#include "T1Project/Json/JsonLoader.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "Runtime/Public/GsTableManager.h"
#include "Map/Area/GsSchemaAreaData.h"

#include "ControllerEx/GsSimplePlayerController.h"

#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaGuildWarHandler.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/WorldComposition.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/NavigationSystem/Public/NavMesh/NavMeshBoundsVolume.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Array.h"
#include "Engine/LevelStreamingDynamic.h"
#include "UTIL/GsSoundUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayTickerSection.h"
#include "Map/GsSchemaMapData.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Management/ScopeGame/GsInvadeBattleLevelManager.h"
#include "DataSchema/InterServer/GsSchemaInvadeWorldConfigDataEx.h"
#include "Components/BoxComponent.h"

const FString OPEN_LEVEL_OPTION = TEXT("?game=/Game/Blueprint/BP_GsGameModeWorld.BP_GsGameModeWorld_C");
const FString JSON_EXTENSION = TEXT(".json");
const FString LAUNCHER_LEVEL_OPTION = TEXT("?game=/Game/Blueprint/BP_GsGameModeLauncher.BP_GsGameModeLauncher_C");
const FString BEGIN_LEVEL_OPTION = TEXT("?game=/Game/Blueprint/BP_GsGameModeLobby.BP_GsGameModeLobby_C");

const int32 LOBBY_LEVEL_KEY_LAUNCHER = 0;
const int32 LOBBY_LEVEL_KEY_BEGIN = 1;

const FString LAUNCHER_LEVEL = TEXT("/Game/Launcher/Launcher");
const FString LOBBY_LEVEL = TEXT("/Game/Maps/Lobby/Lobby");

const FTextKey WARP_CONDITION_LEVEL_TEXT_KEY = TEXT("Warp_Condition_Level");
const FTextKey WARP_CONDITION_QUEST_TEXT_KEY = TEXT("Warp_Condition_Quest");

const FString ART_TYPE = TEXT("Art");
const FString SERVER_TYPE = TEXT("Server");
const FString DESIGN_TYPE = TEXT("Design");

TWeakObjectPtr<UWorld> UGsLevelManager::_world;
bool UGsLevelManager::_isPlayerSpawnComplete = false;

UGsLevelManager::UGsLevelManager()
{
	//load map table
}

void UGsLevelManager::Initialize()
{
	UWorldComposition::_unregisterLevelEvent.Unbind();
	UWorldComposition::_unregisterLevelEvent.BindUObject(this, &UGsLevelManager::OnCheckUnregisterStreamingLevel);
}

void UGsLevelManager::Finalize()
{
	UWorldComposition::_unregisterLevelEvent.Unbind();
}

bool UGsLevelManager::LoadLevelById(UWorld* inWorld, int32 inLevelKey, FVector inPlayerSpawnPos)
{
	const FGsSchemaAreaData* areaTable = GetAreaData(inLevelKey);
	if (nullptr == areaTable)
	{
		GSLOG(Error, TEXT("Missing area data - map id : %d"), inLevelKey);

		return false;
	}

	bool isSameLevelId = (inLevelKey == _currentLevelKey);
	
	GSInterServer()->SetCurrentMap(inLevelKey);
	
	UGsGameObjectUtil::SetSpawnMeCompleteFinFlag(false);

	//If map already loaded, dont' load map
	if (false == _bForcedChangeLevel &&
		IsSameArea(inLevelKey, _currentLevelKey))
	{
		_isSameArea = true;
		GSGameObject()->ClearVisibleSpawnInfo();

		_currentLevelKey = inLevelKey;
		_playerSpawnPos = inPlayerSpawnPos;
		_changedLevel = false;

		GSGameObject()->InitVisibleSpawnInfo(_currentLevelKey);

		if (AGsGameModeWorld* gameModeWorld = Cast<AGsGameModeWorld>(inWorld->GetAuthGameMode()))
		{
			gameModeWorld->StartSpawnPlayer(isSameLevelId);

			return true;
		}
		else
		{
			GSLOG(Error, TEXT("gameModeWorld == nullptr"));
		}
	}
	else
	{
		_isSameArea = false;
		GSGameObject()->ClearVisibleSpawnInfo();

		_bForcedChangeLevel = false;
		_currentLevelKey = inLevelKey;
		_playerSpawnPos = inPlayerSpawnPos;
		_changedLevel = true;

		FGsPrimitiveInt32 param(1);
		GMessage()->GetSystemParam().SendMessage(MessageSystem::BEFORE_CHANGE_LEVEL, &param);
		
		UnHoldStreamingLevel();
		FString FinalPath = FString::Printf(TEXT("/Game/Maps/%s/%s"), *areaTable->levelName, *areaTable->levelName);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*FinalPath), true, OPEN_LEVEL_OPTION);

		GSGameObject()->InitVisibleSpawnInfo(_currentLevelKey);
		
		return true;
	}

	return false;
}

// BSAM - 이미 로드 되있으면 true를 반환한다.
bool UGsLevelManager::LoadLauncher()
{
	GSLOG(Log, TEXT("UGsLevelManager::LoadLauncher() 1"));

	// 이미 런처 로드 되있다면
	if (_currentLevelKey == LOBBY_LEVEL_KEY_LAUNCHER)
		return false;

	_bForcedChangeLevel = false;
	_currentLevelKey = LOBBY_LEVEL_KEY_LAUNCHER;
	_playerSpawnPos = FVector::ZeroVector;
	_changedLevel = true;

	// 리셋
	_warpMapId = 0;
	_warpPos = FVector::ZeroVector;
	_warpReason = WarpReason::INVALID;
	_currLoadTopoEffectType = EGsLoadTopoEffectType::NONE;

	FGsPrimitiveInt32 param(0);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::BEFORE_CHANGE_LEVEL, &param);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*LAUNCHER_LEVEL), true, LAUNCHER_LEVEL_OPTION);

	GSLOG(Log, TEXT("UGsLevelManager::LoadLauncher() 2"));

	return true;
}

bool UGsLevelManager::LoadLobby()
{
	// 이미 비긴 로드 되있다면
	if (_currentLevelKey == LOBBY_LEVEL_KEY_BEGIN)
		return false;

	_bForcedChangeLevel = false;
	_currentLevelKey = LOBBY_LEVEL_KEY_BEGIN;
	_playerSpawnPos = FVector::ZeroVector;
	_changedLevel = true;

	// 리셋
	_warpMapId = 0;
	_warpPos = FVector::ZeroVector;
	_warpReason = WarpReason::INVALID;
	_currLoadTopoEffectType = EGsLoadTopoEffectType::NONE;

	FGsPrimitiveInt32 param(0);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::BEFORE_CHANGE_LEVEL, &param);

	GSLOG(Log, TEXT("UGsLevelManager::LoadLobby()"));
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LOBBY_LEVEL), true, BEGIN_LEVEL_OPTION);

	return true;
}

bool UGsLevelManager::BackToLauncher()
{
	_bForcedChangeLevel = false;
	_currentLevelKey = LOBBY_LEVEL_KEY_LAUNCHER;
	_playerSpawnPos = FVector::ZeroVector;
	_changedLevel = true;

	// 리셋
	_warpMapId = 0;
	_warpPos = FVector::ZeroVector;
	_warpReason = WarpReason::INVALID;
	_currLoadTopoEffectType = EGsLoadTopoEffectType::NONE;	

	FGsPrimitiveInt32 param(1);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::BEFORE_CHANGE_LEVEL, &param);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*LAUNCHER_LEVEL), true, LAUNCHER_LEVEL_OPTION);

	return true;
}

bool UGsLevelManager::BackToLobby()
{
	_bForcedChangeLevel = false;
	_currentLevelKey = LOBBY_LEVEL_KEY_BEGIN;
	_playerSpawnPos = FVector::ZeroVector;
	_changedLevel = true;

	// 리셋
	_warpMapId = 0;
	_warpPos = FVector::ZeroVector;
	_warpReason = WarpReason::INVALID;
	_currLoadTopoEffectType = EGsLoadTopoEffectType::NONE;

	FGsPrimitiveInt32 param(1);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::BEFORE_CHANGE_LEVEL, &param);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*LOBBY_LEVEL), true, BEGIN_LEVEL_OPTION);

	return true;
}

// bak1210 
// 정의된 상수 1004, BP 패스등은 외부에서 세팅될 이슈가 없어 하드코딩으로 대체한다.
const int LevelStreamingHolderID = 1004;
#define HOLDER_CLASS  TEXT("/Game/Blueprint/BP_World.BP_World_C")

bool UGsLevelManager::HoldStreamingLevelByLocation(const FVector& inLocation)
{
	if (0 < _cameraOwnerList.Num())
		return true;

	UnHoldStreamingLevel();

	FString Error;
	_levelStreamingHolder = GetWorld()->GetGameInstance()->CreateLocalPlayer(LevelStreamingHolderID, Error, false);
	if (nullptr != _levelStreamingHolder)
	{
		if (UClass* BpClassPawn = LoadObject<UClass>(nullptr, HOLDER_CLASS))
		{
			FActorSpawnParameters spawnParam = FActorSpawnParameters();
			spawnParam.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
			FTransform trMy = FTransform::Identity;
			trMy.SetLocation(inLocation);

			auto actor = Cast<APawn>(GetWorld()->SpawnActor(BpClassPawn, &trMy, spawnParam));

			UClass* classController = AGsSimplePlayerController::StaticClass();
			auto playerController = GetWorld()->SpawnActor<AGsSimplePlayerController>(classController, spawnParam);

			_levelStreamingHolder->SwitchController(playerController);
			_levelStreamingHolder->PlayerController->Possess(actor);

			playerController->InitInputSystem(); //at tick, input system is auto created and visible
			playerController->SetVirtualJoystickVisibility(false);

			// 조이스틱 터치영역 이벤트 안뺏기게 처리
			playerController->ActivateTouchInterface(nullptr);

			return true;
		}
	}
	return false;
}

// 홀더의 컨트롤러 구하기
APlayerController* UGsLevelManager::GetHolderController()
{
	if (_levelStreamingHolder == nullptr)
	{
		return nullptr;
	}

	return _levelStreamingHolder->GetPlayerController(GetWorld());
}

MapType UGsLevelManager::GetCurrentMapType()
{
	const FGsSchemaMapData* currMapData = GetCurrentMapInfo();
	if (currMapData)
	{
		return currMapData->mapType;
	}

	return MapType::MAX;
}

void UGsLevelManager::UnHoldStreamingLevel()
{
	if (0 < _cameraOwnerList.Num())
		return;

	if (nullptr != _levelStreamingHolder)
	{
		if (APawn* pawn = _levelStreamingHolder->PlayerController->GetPawn())
		{
			GetWorld()->DestroyActor(pawn);
		}
		if (-1 != GetWorld()->GetGameInstance()->GetLocalPlayers().Find(_levelStreamingHolder))
		{
			GetWorld()->GetGameInstance()->RemoveLocalPlayer(_levelStreamingHolder);
		}

		_levelStreamingHolder = nullptr;
	}
}


bool UGsLevelManager::IsHoldingStreamingLevel()
{
	return nullptr != _levelStreamingHolder;
}

// 워프 데이터로 레벨 로드 처리
bool UGsLevelManager::LoadLevelByWarp()
{
	return LoadLevelById(GetWorld(), _warpMapId, _warpPos);
}

bool UGsLevelManager::TryGetLevelName(FString& outLevel, int32 inLevelKey)
{
	if (const FGsSchemaAreaData* areaData = GetAreaData(inLevelKey))
	{
		outLevel = areaData->levelName;

		return true;
	}

	return false;
}

const FGsSchemaMapData* UGsLevelManager::GetCurrentMapInfo()
{
	if (const FGsSchemaMapData* find = GetMapData(_currentLevelKey))
	{
		return find;
	}

	return nullptr;
}

const FGsSchemaAreaData* UGsLevelManager::GetCurrentAreaData()
{
	return GetAreaData(_currentLevelKey);
}

/// <summary>
/// 월드 컴포지션은 기본적으로 레벨에 포함된 레벨은 모두 다 로드하도록 설계되어있다.
/// 이 함수는 월드컴포지션 사용하는 레벨 로드시 이벤트나 퍼포먼스를 위해
/// 미리 로드하지 말아야할 스트리밍 레벨을 체크하는 함수이다.
/// true로 리턴하면 포함하지 않고 false 포함
/// </summary>
/// <param name="inLevel"></param>
/// <returns></returns>
bool UGsLevelManager::OnCheckUnregisterStreamingLevel(const FString& inLevel)
{
	// BSAM - Lobby Level은 앞으로 두개다(Begin)
	if (_currentLevelKey == INDEX_NONE
		|| _currentLevelKey == LOBBY_LEVEL_KEY_BEGIN
		)
	{
		return false;
	}

	TArray<FString> parses;
	inLevel.ParseIntoArray(parses, TEXT("_"), true);

	//지형 서브레벨 이름 예 : LibraDesert_x0_y0
	//Server 이름 예 : LibraDesert_2011055_Server
	//Design 이름 예 : LibraDesert_2011055_Design
	//Art 이름 예 :  LibraDesert_Art	
	if (parses.Num() == 2)
	{
		//아트 레벨은 자동 로드에 포함하지 않는다.
		if (parses[1].Equals(ART_TYPE))
		{
			return true;
		}
	}
	else if (parses.Num() == 3)
	{
		if (parses[1].IsNumeric())
		{
			//디자인 레벨은 자동 로드에 포함하지 않는다.
			if (parses[2].Equals(DESIGN_TYPE))
			{
				return true;
			}

#if WITH_EDITOR
			//서버 레벨은 패키지에 포함되지 않는다. 에디터에서도 로드하지 않는다.
			if (parses[2].Equals(SERVER_TYPE))
			{
				return true;
			}
#endif
		}
	}

	return false;
}

bool UGsLevelManager::IsServerLevel(const FString& inLevel)
{
	TArray<FString> parses;
	if (TryParseLevelName(parses, inLevel))
	{
		return parses[2].Equals("Server");
	}

	return false;
}

bool UGsLevelManager::IsDesignlevel(const FString& inLevel)
{
	TArray<FString> parses;
	if (TryParseLevelName(parses, inLevel))
	{
		if (_currentLevelKey != FCString::Atoi(*parses[1]))
		{
			return false;
		}

		return parses[2].Equals("Design");
	}

	return false;
}

bool UGsLevelManager::TryParseLevelName(TArray<FString>& outLevel, const FString& inLevel)
{
	TArray<FString> parses;
	int32 num = inLevel.ParseIntoArray(parses, TEXT("_"), true);
	if (num < 3)
	{
		return false;
	}

	outLevel = parses;
	return true;
}

MapType UGsLevelManager::GetMapType()
{
	if (const FGsSchemaMapData* currMapData = GetCurrentMapInfo())
	{
		return currMapData->mapType;
	}

	return MapType::MAX;
}

bool UGsLevelManager::IsCanSpotWarp(int32 inDestLevelId, int32 inSpotIndex, int32 inPlayerLevel/*, QuestId inQuestId*/)
{
	if (false == IsCanMapWarp(inDestLevelId, inPlayerLevel/*, inQuestId*/))
	{
		return false;
	}

	const FGsSchemaMapData* mapData = GLevel()->GetMapData(inDestLevelId);
	if (nullptr == mapData)
	{
		GSLOG(Error, TEXT("Map id missing - map id : %d"), inDestLevelId);
		return false;
	}

	if (inSpotIndex == INDEX_NONE)
	{
		GSLOG(Error, TEXT("Invalid spot index - map id : %d"), inDestLevelId);
		return false;
	}

	const TArray<FGsSchemaMapEnterConditionSpotInfo>& spotInfoList = mapData->enterCondition.SpotList;
	for (const FGsSchemaMapEnterConditionSpotInfo& spotInfo : spotInfoList)
	{
		if (spotInfo.spotIndex != inSpotIndex)
		{
			continue;
		}

		const FGsSchemaQuest* quest = spotInfo.questId.GetRow();
		if (nullptr == quest)
		{
			continue;
		}

		if (false == GSQuest()->IsCompleted(quest->id))
		{
			//퀘스트 조건 미달
			FText format;
			if (FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, WARP_CONDITION_QUEST_TEXT_KEY, format))
			{
				FText errorMsg = FText::Format(format, FText::FromString(FString::Printf(TEXT("%llu"), quest->id)));
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
			}

			return false;
		}

		if (spotInfo.level != 0
			&& spotInfo.level > inPlayerLevel)
		{
			//레벨 조건 미달
			FText format;
			if (FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, WARP_CONDITION_LEVEL_TEXT_KEY, format))
			{
				FText errorMsg = FText::Format(format, FText::FromString(FString::FromInt(inPlayerLevel)));
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
			}

			return false;
		}

		break;
	}


	return true;
}

bool UGsLevelManager::IsCanMapWarp(int32 inDestLevelId, int32 inPlayerLevel /*= 0*//*, QuestId inQuestId = 0*/)
{
	const FGsSchemaMapData* mapData = GLevel()->GetMapData(inDestLevelId);
	if (nullptr == mapData)
	{
		return false;
	}

	if (const FGsSchemaQuest* quest = mapData->enterCondition.QuestId.GetRow())
	{
		if (false == GSQuest()->IsCompleted(quest->id))
		{
			//퀘스트 조건 미달
			FText format;
			if (FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, WARP_CONDITION_QUEST_TEXT_KEY, format))
			{
				FText errorMsg = FText::Format(format, FText::FromString(FString::Printf(TEXT("%llu"), quest->id)));
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
			}

			return false;
		}
	}

	if (mapData->enterCondition.level != 0
		&& mapData->enterCondition.level > inPlayerLevel)
	{
		//레벨 조건 미달
		FText format;
		if (FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, WARP_CONDITION_LEVEL_TEXT_KEY, format))
		{
			FText errorMsg = FText::Format(format, FText::FromString(FString::FromInt(mapData->enterCondition.level)));
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
		}

		return false;
	}

	return true;
}

bool UGsLevelManager::IsCanMonsterWarp(int32 inDestLevelId, int32 inPlayerLevel)
{
	if (false == IsCanMapWarp(inDestLevelId, inPlayerLevel))
	{
		return false;
	}

	const FGsSchemaMapData* mapData = GLevel()->GetMapData(inDestLevelId);
	if (nullptr == mapData)
	{
		GSLOG(Error, TEXT("Map id missing - map id : %d"), inDestLevelId);
		return false;
	}

	const TArray<FGsSchemaMapEnterConditionSpotInfo>& spotInfoList = mapData->enterCondition.SpotList;
	for (const FGsSchemaMapEnterConditionSpotInfo& spotInfo : spotInfoList)
	{
		const FGsSchemaQuest* quest = spotInfo.questId.GetRow();
		if (nullptr == quest)
		{
			continue;
		}

		if (false == GSQuest()->IsCompleted(quest->id))
		{
			//퀘스트 조건 미달
			FText format;
			if (FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, WARP_CONDITION_QUEST_TEXT_KEY, format))
			{
				FText errorMsg = FText::Format(format, FText::FromString(FString::Printf(TEXT("%llu"), quest->id)));
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
			}

			return false;
		}

		if (spotInfo.level != 0
			&& spotInfo.level > inPlayerLevel)
		{
			//레벨 조건 미달
			FText format;
			if (FText::FindText(UI_COMMON_NAMESPACE_TEXT_KEY, WARP_CONDITION_LEVEL_TEXT_KEY, format))
			{
				FText errorMsg = FText::Format(format, FText::FromString(FString::FromInt(inPlayerLevel)));
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
			}

			return false;
		}

		break;
	}

	return true;
}

bool UGsLevelManager::TryGetDestSpot(int32 inLevelId, int64 inWarpId, OUT int32& outDestLevelKey, OUT int32& outDestSpotIndex)
{
	const FGsSchemaMapData* mapData = GLevel()->GetMapData(inLevelId);
	if (nullptr == mapData)
	{
		GSLOG(Error, TEXT("Map id missing - id : %d"), inLevelId);
		return false;
	}

	const FGsSchemaMapTerritoryData* territoryData = mapData->territoryInfo.GetRow();
	if (nullptr == territoryData)
	{
		GSLOG(Error, TEXT("Territory id missing - id : %d"), inLevelId);
		return false;
	}

	const FGsSchemaMapWarpInfo* findWarp = territoryData->warpList.FindByPredicate([&](const FGsSchemaMapWarpInfo& warpInfo)->bool {
		return warpInfo.warpId == inWarpId;
		});

	if (nullptr == findWarp)
	{
		GSLOG(Error, TEXT("Warp id missing - map id : %d\twarp id : %lld"), inLevelId, inWarpId);
		return false;
	}

	const FGsSchemaMapWarpSpot& destWarpSpot = findWarp->destSpot;
	const FGsSchemaSpotData* destSpot = destWarpSpot.MapId.GetRow();

	outDestLevelKey = destSpot->mapId;
	const TArray<FGsSchemaSpotInfo>& spotInfoArray = destSpot->SpotList;

	bool isFind = false;
	for (const FGsSchemaSpotInfo spot : spotInfoArray)
	{
		if (spot.id == destWarpSpot.SpotIndex)
		{
			isFind = true;
			break;
		}
	}

	if (false == isFind)
	{
		//invalid index
		GSLOG(Error, TEXT("Out of index - map id : "), inLevelId);
		return false;
	}

	outDestSpotIndex = destWarpSpot.SpotIndex;

	return true;
}

bool UGsLevelManager::GetLevelBounds(FBoxSphereBounds& bounds)
{
	if (AActor* NavMeshBoundsVolume = UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass()))
	{
		bounds = Cast<ANavMeshBoundsVolume>(NavMeshBoundsVolume)->GetBounds();
		return true;
	}
	return false;
}

TArray<FGsSchemaSpotInfo> UGsLevelManager::GetMapSpot(int32 inLevelKey)
{
	TArray<FGsSchemaSpotInfo> resultSpotInfoList;
	TArray<const FGsSchemaSpotData*> spotList;
	FString levelKey = FString::FromInt(inLevelKey);
	const FString& tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(levelKey);
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), tableName);
	const UGsTableSpotData* spotTable = Cast<UGsTableSpotData>(table);
	if (nullptr == spotTable)
	{
		return resultSpotInfoList;
	}

	spotTable->GetAllRows(spotList);
	if (spotList.Num() < 1)
	{
		return resultSpotInfoList;
	}

	FGsSchemaSpotInfo temp;
	const TArray<FGsSchemaSpotInfo>& spotInfoList = spotList[0]->SpotList;
	for (const FGsSchemaSpotInfo& spotInfo : spotInfoList)
	{
		temp.id = spotInfo.id;
		temp.pos = spotInfo.pos;
		temp.radius = spotInfo.radius;

		resultSpotInfoList.Add(temp);
	}

	return resultSpotInfoList;
}

TArray<FGsSchemaSpotInfo> UGsLevelManager::GetZone(int32 inLevelKey)
{
	TArray<FGsSchemaSpotInfo> resultSpotInfoList;
	TArray<const FGsSchemaSpawnData*> spawnList;
	FString levelKey = FString::FromInt(inLevelKey);
	const FString& tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(levelKey);
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpawnData::StaticStruct(), tableName);
	const UGsTableSpawnData* spawnTable = Cast<UGsTableSpawnData>(table);
	if (nullptr == spawnTable)
	{
		return resultSpotInfoList;
	}

	spawnTable->GetAllRows(spawnList);
	if (spawnList.Num() == 0)
	{
		return resultSpotInfoList;
	}

	FGsSchemaSpotInfo temp;	
	for (const FGsSchemaSpawnData* spawnData : spawnList)
	{
		temp.id = spawnData->SpawnGroupId;
		temp.pos = spawnData->Region.Center;
		temp.radius = spawnData->Region.Radius;

		resultSpotInfoList.Add(temp);
	}

	return resultSpotInfoList;
}

const FGsSchemaSpawnData* UGsLevelManager::GetZone(int32 inLevelKey, int32 inGroupId)
{	
	TArray<const FGsSchemaSpawnData*> spawnList;
	FString levelKey = FString::FromInt(inLevelKey);
	const FString& tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(levelKey);
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaSpawnData::StaticStruct(), tableName);
	const UGsTableSpawnData* spawnTable = Cast<UGsTableSpawnData>(table);
	if (nullptr == spawnTable)
	{
		return nullptr;
	}

	spawnTable->GetAllRows(spawnList);
	if (spawnList.Num() == 0)
	{
		return nullptr;
	}

	const FGsSchemaSpawnData** find = spawnList.FindByPredicate([&](const FGsSchemaSpawnData* spawnData)->bool {
		return spawnData->SpawnGroupId == inGroupId;
		});
	return (*find);
}

bool UGsLevelManager::IsRegisteredLevel(int32 inLevelKey)
{
	return (nullptr == GetAreaData(inLevelKey));
}

const FGsSchemaMapData* UGsLevelManager::GetMapData(int32 inMapKey)
{
	return UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(inMapKey);
}

void UGsLevelManager::SetGameWorld(UWorld* inWorld)
{
	_world = inWorld;
}

UWorld* UGsLevelManager::GetGameWorld()
{
	if (_world.IsValid())
	{
		return _world.Get();
	}

	return nullptr;
}

void UGsLevelManager::ClearGameWorld()
{
	_world.Reset();
}

void UGsLevelManager::SetIsPlayerSpawnComplete(bool inSpawned)
{
	_isPlayerSpawnComplete = inSpawned;
}

bool UGsLevelManager::GetIsPlayerSpawnComplete()
{
	return _isPlayerSpawnComplete;
}

TArray<FVector> UGsLevelManager::GetFloorPointList(class UBoxComponent* inBoxComponent)
{
	FVector extent = inBoxComponent->GetScaledBoxExtent();
	FVector v1 = FVector(extent.X, extent.Y, -extent.Z);
	FVector v2 = FVector(-extent.X, extent.Y, -extent.Z);
	FVector v3 = FVector(-extent.X, -extent.Y, -extent.Z);
	FVector v4 = FVector(extent.X, -extent.Y, -extent.Z);
	TArray<FVector> pointArray;
	pointArray.Add(v1);
	pointArray.Add(v2);
	pointArray.Add(v3);
	pointArray.Add(v4);

	//scale, rotate, trans
	FVector center = inBoxComponent->GetComponentLocation();
	for (int32 i = 0; i < pointArray.Num(); ++i)
	{
		pointArray[i] = pointArray[i].RotateAngleAxis(inBoxComponent->GetComponentRotation().Yaw, FVector::UpVector);
		pointArray[i] += center;
	}	

	return pointArray;
}

const FGsSchemaAreaData* UGsLevelManager::GetAreaData(int32 inLevelKey)
{
	const FGsSchemaMapData* mapTable = GetMapData(inLevelKey);
	if (nullptr == mapTable 
	|| nullptr == mapTable->areaId.GetRow())
	{
		GSLOG(Error, TEXT("Missing map data - id : %d"), inLevelKey);
		return nullptr;
	}

	return mapTable->areaId.GetRow();
}

int32 UGsLevelManager::GetTotalChannelCount()
{
	const FGsSchemaAreaData* areaData = GetCurrentAreaData();

	return areaData->channel.maxCount;
}

const FString UGsLevelManager::GetDesignLevelName(const UWorld* inWorld, int32 inLevelId)
{
	FString mapName;
	int32 levelKey = inLevelId;

#if WITH_EDITOR
	mapName = UWorld::RemovePIEPrefix(inWorld->GetMapName());
#else
	mapName = inWorld->GetMapName();
#endif

	FString levelDesignName = mapName + TEXT("_") + FString::FromInt(levelKey) + TEXT("_") + TEXT("Design");

	return levelDesignName;
}

const FString UGsLevelManager::GetArtLevelName(const UWorld* inWorld, int32 inLevelId)
{
	FString mapName;
	int32 levelKey = inLevelId;

#if WITH_EDITOR
	mapName = UWorld::RemovePIEPrefix(inWorld->GetMapName());
#else
	mapName = inWorld->GetMapName();
#endif

	FString levelName = mapName + TEXT("_") + TEXT("Art");

	return levelName;
}

const FString UGsLevelManager::GetDesignLevelFullPath(const UWorld* inWorld, int32 inLevelId)
{
	FString levelName = GetDesignLevelName(inWorld, inLevelId);
	return TEXT("/") + GetLevelFolderPath(inWorld, inLevelId) + TEXT("Design/") + levelName + TEXT(".") + levelName;
}

const FString UGsLevelManager::GetArtLevelFullPath(const UWorld* inWorld, int32 inLevelId)
{
	FString levelName = GetArtLevelName(inWorld, inLevelId);
	return TEXT("/") + GetLevelFolderPath(inWorld, inLevelId) + TEXT("Art/") + levelName + TEXT(".") + levelName;
}

const FString UGsLevelManager::GetLevelFolderPath(const UWorld* inWorld, int32 inLevelId)
{
	FString fullPath = inWorld->PersistentLevel->GetOutermost()->GetFName().ToString();
	TArray<FString> parseArray;
	fullPath.ParseIntoArray(parseArray, TEXT("/"));
	FString folderPath;

	for (int32 i = 0; i < parseArray.Num() - 1; ++i)
	{
		folderPath += parseArray[i];
		folderPath += TEXT("/");
	}

	return folderPath;
}

bool UGsLevelManager::IsInstanceSingleMap()
{
	return IsMapType(MapType::INSTANCE_SINGLE);
}

// 던전맵인가
bool UGsLevelManager::IsDungeonMap()
{	
	return IsMapType(MapType::PUBLIC_DUNGEON);
}

bool UGsLevelManager::IsDungeonMap(int32 InMapId)
{
	const FGsSchemaMapData* mapData = GetMapData(InMapId);
	if (nullptr == mapData)
		return false;
	
	if (MapType::PUBLIC_DUNGEON == mapData->mapType)
		return true;
	
	return false;
}

bool UGsLevelManager::IsGuildDungeonMap()
{
	return IsMapType(MapType::GUILD_DUNGEON);
}

bool UGsLevelManager::IsPartyDungeonMap()
{
	return IsMapType(MapType::PARTY_DUNGEON);
}

bool UGsLevelManager::IsEliteDungeonMap()
{
	return IsMapType(MapType::ELITE_DUNGEON_1);
}

// 현재맵이랑 같은 타입인가
bool UGsLevelManager::IsMapType(MapType In_type)
{
	const FGsSchemaMapData* currMapData = GetCurrentMapInfo();
	if (currMapData)
	{
		if (In_type == currMapData->mapType)
		{
			return true;
		}
	}

	return false;
}

// 시공의 틈새 맵 컨텐츠 인가
bool UGsLevelManager::IsSpaceCrackMapContents()
{
	const FGsSchemaMapData* currMapData = GetCurrentMapInfo();
	if (currMapData)
	{
		if (MapContentsType::SPACE_CRACK == currMapData->ContentsType)
		{
			return true;
		}
	}

	return false;
}

void UGsLevelManager::SetLoadTopoData(int32 InMapId, FVector InPos, WarpReason InReason)
{
	_warpMapId = InMapId;
	_warpPos = InPos;
	_warpReason = InReason;

	_currLoadTopoEffectType = GetLoadTopoEffectType(_warpMapId, _warpReason);
}

EGsLoadTopoEffectType UGsLevelManager::GetLoadTopoEffectType(int32 InMapId, WarpReason InReason)
{
	// 다른 Area인가
	if (false == IsSameArea(InMapId, _currentLevelKey))
	{
		return EGsLoadTopoEffectType::DIFF_MAP;
	}

	// 채널이동인가? 개인 이벤트 채널 입장인가?
	// 시공의 틈새로 이동인가
	if (WarpReason::CHANGE_CHANNEL == InReason ||
		WarpReason::EVENT_ACTION_TO_INSTANCE_SINGLE == InReason ||
		WarpReason::EVENT_ACTION_RETURN_TO_PUBLIC == InReason ||
		WarpReason::SPACE_CRACK == InReason)
	{
		return EGsLoadTopoEffectType::SAME_MAP_SAME_POS;
	}

	// 같은맵, 다른 좌표
	return EGsLoadTopoEffectType::SAME_MAP_DIFF_POS;
}

void UGsLevelManager::PushViewTargetWithBlend(TWeakObjectPtr<AActor> NewViewTarget, float BlendTime /*= 0*/, enum EViewTargetBlendFunction BlendFunc /*= VTBlend_Linear*/, float BlendExp /*= 0*/, bool bLockOutgoing /*= false*/)
{
	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendTime = BlendTime;
	TransitionParams.BlendFunction = BlendFunc;
	TransitionParams.BlendExp = BlendExp;
	TransitionParams.bLockOutgoing = bLockOutgoing;

	PushViewTarget(NewViewTarget, TransitionParams);
}

void UGsLevelManager::PushViewTarget(TWeakObjectPtr<AActor> NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	if (0 < _cameraOwnerList.Num())
	{
		_cameraOwnerList.Remove(NewViewTarget);
	}

	if (NewViewTarget.IsValid())
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			controller->SetViewTarget(NewViewTarget.Get(), TransitionParams);

			if (NewViewTarget->InputComponent)
			{
				NewViewTarget->InputComponent->BindTouch(EInputEvent::IE_Pressed, GScope(), &UGsScopeHolder::OnTouchPress);
				NewViewTarget->InputComponent->BindTouch(EInputEvent::IE_Released, GScope(), &UGsScopeHolder::OnTouchRelease);
				NewViewTarget->InputComponent->BindTouch(EInputEvent::IE_Repeat, GScope(), &UGsScopeHolder::OnTouchMove);
			}
		}
		_cameraOwnerList.Emplace(NewViewTarget);
	}
}

void UGsLevelManager::PopViewTarget(TWeakObjectPtr<AActor> RemoveViewTarget)
{
	if (RemoveViewTarget.IsValid())
	{
		_cameraOwnerList.Remove(RemoveViewTarget);
	}

	while (0 < _cameraOwnerList.Num() && !_cameraOwnerList.Last().IsValid())
	{
		_cameraOwnerList.RemoveAt(_cameraOwnerList.Num() - 1);
	}

	if (0 < _cameraOwnerList.Num() && _cameraOwnerList.Last().IsValid())
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			controller->SetViewTarget(_cameraOwnerList.Last().Get());
		}
	}
}

// 워프데이터가 다른맵 가는건가
bool UGsLevelManager::IsWarpDataDifferentMap()
{
	if (false == _bForcedChangeLevel &&
		_warpMapId == _currentLevelKey)
	{
		return false;
	}

	return true;
}

bool UGsLevelManager::IsSameArea(int32 InMapId1, int32 InMapId2)
{
	if (InMapId1 == InMapId2)
	{
		return true;
	}

	const FGsSchemaMapData* mapData1 = GetMapData(InMapId1);
	const FGsSchemaMapData* mapData2 = GetMapData(InMapId2);
	if (nullptr != mapData1 && nullptr != mapData2)
	{
		if (mapData1->areaId == mapData2->areaId)
		{
			return true;
		}
	}

	return false;
}

bool UGsLevelManager::TryGetClientSpawnNpcInTown(TArray<const FGsSchemaTownData*>& outNpcList)
{
	outNpcList.Empty();

	TArray<const FGsSchemaTownData*> npcDataArray;
	if (false == TryGetAllSpawnNpcInTown(npcDataArray))
	{
		return false;
	}

	for (const FGsSchemaTownData* spawnData : npcDataArray)
	{
		if (spawnData)
		{
			if (spawnData->isClientSpawn)
			{
				outNpcList.Add(spawnData);
			}
		}
	}

	return outNpcList.Num() > 0;
}

bool UGsLevelManager::TryGetAllSpawnNpcInTown(TArray<const FGsSchemaTownData*>& outNpcList)
{
	outNpcList.Empty();

	int32 levelKey = GLevel()->GetCurrentLevelId();
	FString levelKeyString = FString::FromInt(levelKey);

	//Get town data
	FString townTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableTownData>(levelKeyString);
	const UGsTable* townTable = UGsTableManager::GetInstance().GetTable(FGsSchemaTownData::StaticStruct(), townTableName);
	if (nullptr == townTable)
	{
		return false;
	}
	
	townTable->GetAllRows<FGsSchemaTownData>(outNpcList);	

	return outNpcList.Num() > 0;
}
// 인게임에서 pc 스폰전에 해당 위치 레벨 로드 되었는지 체크
// TrySweepToLand 함수 사용하기 위해 실린더 radius, height 값 저장해놓는다	
// FGsSchemaCreatureCommonInfo 테이블 찾을때 1, 2 가 pc 남, 여 키값인데 
// 일단은 gender 값이랑 같아서 그걸로 찾자 (로비에서 정확히 가리키는 값있다면 그걸로 변경 필요)
// https://jira.com2us.com/jira/browse/C2URWQ-5675 
void UGsLevelManager::SetLocalCylinderSize(int32 In_cylinderRadius, int32 In_cylinderHeight)
{
	_localCylinderRadius = In_cylinderRadius;
	_localCylinderHeight = In_cylinderHeight;
}

// 현재 view target 체크
void UGsLevelManager::CheckViewTarget()
{
	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (gameObjectManager == nullptr)
	{
		return;
	}

	UGsGameObjectBase* localPlayer = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
	if (localPlayer == nullptr)
	{
		return;
	}

	AActor* localChar = localPlayer->GetActor();

	if (localChar == nullptr)
	{
		return;
	}

	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		AActor* target = controller->GetViewTarget();
		if (target != localChar)
		{
			controller->SetViewTarget(localChar);
		}		

	}
}