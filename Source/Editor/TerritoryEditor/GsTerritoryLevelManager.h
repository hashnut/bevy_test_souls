
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//ue4
#include "UObject/Class.h"

//territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Private/LevelModel.h"
#include "Actor/Spot/GsTerritorySpot.h"
#include "Actor/Spot/GsTerritoryGameSpot.h"
#include "Actor/Spot/GsTerritorySpaceCrackPortalSpot.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Blutility/Input/GsTerritorySpotInput.h"
#include "Actor/Spot/GsTerritoryWarpSpot.h"
#include "Blutility/Input/GsTerritorySpawnActorInput.h"
#include "GameObject/ObjectClass/Data/GsEnvCondition.h"
#include "Map/GsLineShapeType.h"

DECLARE_MULTICAST_DELEGATE(FTerritoryEditorAdditionalLevelDelegate)
DECLARE_MULTICAST_DELEGATE(FTerritoryEditorSaveMapFinsh)
DECLARE_MULTICAST_DELEGATE(FTerritoryEditorSaveAllMapFinsh)

const FString SERVER_LEVEL_STREAMING_NAME = TEXT("Server");

class AGsTerritoryZone;
class AGsTerritorySpawnActor;
class AGsTerritorySpot;
class UGsTerrEdStreamingLevelHandler;
class AGsTerritoryNpcActor;
struct FGsSchemaMapData;
class UGsTableNpcData;
class UGsTableShapeData;
class UGsTablePropData;
class UGsTableQuest;
/*
* Managing event in level and level object 
*/
enum EGsTerrEdShowType
{
	HideAll,
	ShowOnlySelect,
	ShowAll,
};

class TERRITORYEDITOR_API FGsTerritoryLevelManager
{
public:
	static EGsTerrEdShowType _widgetShowType;
	static EGsTerrEdShowType _shapeShowType;
	static EGsTerrEdShowType _sightRadiusShowType;
	static float _widgetVisibleDistance;	
	static float _shapeVisibleDistance;
	static float _sightRadiusVisibleDistance;
	static bool _isTransparentFloor;
	static uint8 _transparentFloorAlpha;
	static bool _isScalabilityQualityLow;

public:
	static UGsTableNpcData* _cachedNpcData;
	static UGsTableShapeData* _cachedShapeData;
	static UGsTablePropData* _cachedPropData;
	static UGsTableMapData* _cachedMapData;
	static UGsTableQuest* _cachedQuestData;
	static UGsTableAreaData* _cachedAreaData;

	//Level 작업시 델리게이트 핸들러
private:
	//Child actor 선택 델리게이트 핸들러
	FDelegateHandle _childShapeSelectHandle;	

	//맵 저장 시 델리게이트 핸들러
private:		
	//맵 오픈 이벤트 델리게이트 핸들러
	FDelegateHandle _levelOpenHandle;
	//서버 레벨 오픈 델리게이트 핸들러
	FDelegateHandle _addServerLevelHandle;
	//서버 레벨 저장 델리게이트 핸들러
	FDelegateHandle _postSaveHandle;	
	//맵 저장시 추가되는 델리게이트 핸들러
	FDelegateHandle _additionalLevelEventHandle;		
	//모든 맵 저장시 델리게이트 핸들러
	FDelegateHandle _saveAllLevelHandle;

	//맵 저장시 서버레벨에 추가적인 이벤트
public:
	FTerritoryEditorAdditionalLevelDelegate _additionalLevelEvent;
	FTerritoryEditorSaveMapFinsh _saveMapFinishEvent;
	FTerritoryEditorSaveAllMapFinsh _saveAllMapFinishEvent;

	//맵 저장시 ArtLevel을 로드할 것인가?
public:
	static bool _isLoadArtLevel;
	
	//맵 저장시 
private:
	//맵 저장시 현재 저장하고 있는 level id
	int32 _currentLevelId;
	//맵 저장시 현재 저장하고 있는 level index
	int32 _currentLevelIndex;
	//맵 저장시 레벨 리스트
	TArray<TSharedPtr<FLevelModel>> _loadLevelList;
	//맵 저장 기능 활성화
	bool _isActiveSaveLevel;
	//모든 맵 저장 기능 여부
#if !REFACTORY_SAVE_ALL_MAP
	bool _isSaveAllMap;
	//저장까지 대기
	bool _isWaitForSaveAllMap;
#endif

private:
	//맵 Copy시 저장 레벨 Id
	int32 _copySourceLevelId;
	bool _copySpawnActor;
	bool _copySpot;
	bool _copyFence;
	bool _copyLine;
	
public:
	void Initialize();
	void Deinitialize();

	//서버 레벨 관리 기능 
public:
	//Child actor 선택 콜백
	void OnCheckSelectShapeComponent(UObject* inObject);
	//서버 레벨 로드시 아트 레벨 로드
	static void LoadArtLevelsInEditorTime(const FString& inFileName, bool inTemplate = false);	

	//Territory Actor 관련 기능
public:	
	//TerritoryActor 생성시 invalid 설정
	static void SetInvalidTerritoryActor(AGsTerritoryActor* inActor, const FString& inInvalidReason);
	//NonServerLevel에서 TerritoryActor 삭제
	void OnDeleteTerritoryActorInNonServerLevel(AActor* inActor);
	//TerritoryActor 삭제 콜백
	void TerritoryActorDeleted(AActor* inActor);	

	//맵 저장 기능
public:	
	void NavigationAllServerLevel();
	void NavigationAllDesignLevel();
	void NatigationAllLevel();	

public:
	void FinishSaveAllMap();	
	void FinishSaveMap();	
	void ActiveSaveLevel(bool inActive);
#if !REFACTORY_SAVE_ALL_MAP
	bool StartSaveMap(int32 inLevelId);
	void SetSaveMapAll(bool inValue);
	bool GetWaitForSaveAllMap() { return _isWaitForSaveAllMap; }
	void SetWaitForSaveAllMap(bool inSave) { _isWaitForSaveAllMap = inSave; }
#endif

public:
	bool GetActiveSaveLevel() {return _isActiveSaveLevel;}
	int32 GetCurrentLevelId(){return _currentLevelId;}
	void SetCurrentLevelId(int32 inLevelId) { _currentLevelId = inLevelId; }
	int32 GetCurrentLevelIndex() { return _currentLevelIndex; }
	void SetCurrentLevelIndex(int32 inLevelIndex) { _currentLevelIndex = inLevelIndex; }

public:
	static int32 GetLevelKeyByIndex(int32 inLevelIndex);
	static int32 GetLevelIndexByKey(int32 inLevelKey);	
	static const FGsSchemaMapData* GetMapDataByIndex(int32 inLevelIndex);
	static const FGsSchemaAreaData* GetAreaDataById(int32 inAreaId);	

private:
#if !REFACTORY_SAVE_ALL_MAP
	void OnUpdateSaveMap(float inDelta);		
	void OnOpenLevelForSaveAllMap(const FString& inFilename, bool inIsTemplate);
	void OnLoadedServerLevelForSaveAllMap();	
#endif
	void OnPostSaveServerLevel(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess);	
	void OnFinishAddZoneTest();	

public:
	bool OpenLevelForSaveAllMap(const FString& inLevelName);

private:
	bool OpenLevel(const FString& inLevelName);
	void OnOpenLevel(const FString& inFileName, bool inIsTemplate);
	void OnLoadedServerLevel();
	void OnCopyLevel();

private:
	void BindMessage();
	void UnbindMessage();

public:
	void DeleteAllTerritoryActor();

private:
	void DeleteAllSpawnActor();
	void DeleteAllSpotActor();
	void DeleteAllFence();
	void DeleteAllLine();

private:
	void CopyAllTerritoryActor();
	void CopyAllSpawnActor();
	void CopyAllSpot();
	void CopyAllFence();
	void CopyAllLine();

public:
	void UnbindAdditionalLevelEvent();
	void ExcuteAdditionalLevelEvent();

//CopyLevel
public:
	void CopyLevel(int32 inSource, int32 inDest, bool inCopySpawnActor = true, bool inCopySpot = true, bool inCopyFence = true, bool inCopyLine = true);
	void SaveServerLevel();

public:
	void SetShowOptionType(EGsTerrEdShowType inShowOptionType) { _widgetShowType = inShowOptionType; }
	

public:
	static FString ConvertShowTypeString(EGsTerrEdShowType inShowType);
	static EGsTerrEdShowType ConvertShowTypeEnum(FString inShowType);

	//Show widget
public:
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void ShowAllWidget();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void ShowOnlySelectedWidget();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void HideAllWidget();
	static void ActiveTerritoryWidget(EGsTerrEdShowType inShow);
	template<typename TerritoryClass>
	static void ActiveTerritoryWidgetInternal(UWorld* inWorld, EGsTerrEdShowType inShow)
	{
		TArray<TerritoryClass*> selectedTerritoryActors;
		TArray<AActor*> territoryActors;

		if (USelection* selection = GEditor->GetSelectedActors())
		{
			selection->GetSelectedObjects<TerritoryClass>(selectedTerritoryActors);
		}

		UGameplayStatics::GetAllActorsOfClass(inWorld, TerritoryClass::StaticClass(), territoryActors);
		FVector location;

		for (AActor* temp : territoryActors)
		{
			if (temp)
			{
				if (TerritoryClass* territoryActor = Cast<TerritoryClass>(temp))
				{
					if (inShow == EGsTerrEdShowType::ShowAll)
					{
						location = territoryActor->GetActorLocation();
						territoryActor->InvalidWidgetPosition(location);
						territoryActor->InvalidWidgetText();
						territoryActor->SetWidgetVisibility(true);
					}
					else if (inShow == EGsTerrEdShowType::ShowOnlySelect)
					{
						if (selectedTerritoryActors.Contains(temp))
						{
							location = territoryActor->GetActorLocation();
							territoryActor->InvalidWidgetPosition(location);
							territoryActor->InvalidWidgetText();
							territoryActor->SetWidgetVisibility(true);
						}
						else
						{
							territoryActor->SetWidgetVisibility(false);
						}
					}
					else
					{
						territoryActor->SetWidgetVisibility(false);
					}
				}
			}
		}
	}

	//transparentFloor
public:
	static bool IsTransparentFloor() { return _isTransparentFloor; }
	static void SetTransparentFloor(bool inTransparent) 
	{
		_isTransparentFloor = inTransparent; 
		FGsTerritoryLevelManager::InvalidTransparentFloor();
	}
	static void InvalidTransparentFloor();
	static void SetScalabilityQaulityLow(bool inIsQualityLow)
	{
		_isScalabilityQualityLow = inIsQualityLow;
		FGsTerritoryLevelManager::InvalidScalabilityQualityLow();
	}
	static void InvalidScalabilityQualityLow();

	//Show npc 
public:
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void ShowAllShape();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void ShowOnlySelectedShape();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void HideAllShape();
	static void ActiveTerritoryShape(EGsTerrEdShowType inShow);
	template<typename TerritoryClass>
	static void ActiveTerritoryShapeInternal(UWorld* inWorld, EGsTerrEdShowType inShow)
	{
		TArray<TerritoryClass*> selectedTerritoryActors;
		TArray<AActor*> territoryActors;
		if (USelection* selection = GEditor->GetSelectedActors())
		{
			selection->GetSelectedObjects<TerritoryClass>(selectedTerritoryActors);
		}

		UGameplayStatics::GetAllActorsOfClass(inWorld, TerritoryClass::StaticClass(), territoryActors);
		FVector location;

		for (AActor* temp : territoryActors)
		{
			if (temp)
			{
				if (TerritoryClass* territoryActor = Cast<TerritoryClass>(temp))
				{
					if (inShow == EGsTerrEdShowType::ShowAll)
					{
						territoryActor->SetShapeVisible(true);
					}
					else if (inShow == EGsTerrEdShowType::ShowOnlySelect)
					{
						if (selectedTerritoryActors.Contains(temp))
						{
							territoryActor->SetShapeVisible(true);
						}
						else
						{
							territoryActor->SetShapeVisible(false);
						}
					}
					else
					{
						territoryActor->SetShapeVisible(false);
					}
				}
			}
		}
	}

public:
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void ShowSightRadius();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void SelectOnlySightRadius();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorLevelUtil")
		static void HideAllSightRadius();
	static void ActiveSightRadius(EGsTerrEdShowType inShow);

	int32 GetCurrentLevelld() { return _currentLevelId; }
	void SetLoadLevelList(TArray<TSharedPtr<FLevelModel>> inLevelList){_loadLevelList = inLevelList;}

public:
	static bool IsNavmeshExist(UWorld* inWorld);	

public:
	static class UClass* GetZoneTemplate(const EGsTerritoryZoneShapeType inShape);
	static class AGsTerritoryZone* CreateZone(const NpcSpawnGroupType inSpawnGroup, const bool inByClient, const bool inByDefualt, const EGsTerritoryZoneShapeType inShape, const FTransform& inTrs);
	
public:
	static class AGsTerritoryNormalSpot* CreateNormalSpot(const FText& inName, const float inRandomSpawnRange, const FTransform& inTrs);
	static class AGsTerritorySpawnSpot* CreateSpawnSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, const FTransform& inTrs);
	static class AGsTerritoryResurrectionSpot* CreateResurrectionSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, const FTransform& inTrs);
	static class AGsTerritoryTeleportSpot* CreateTeleportSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, const FTransform& inTrs);
	static class AGsTerritorySpaceCrackPortalSpot* CreateSpaceCrackPortalSpot(const FText& inName, const float inRandomSpawnRange, const FTransform& inTrs);
	static class AGsTerritoryGameSpot* CreateTerritoryGameSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, UClass* inClass, const FTransform& inTrs);
	static class AGsTerritoryWarpSpot* CreateWarp(const FGsTerritoryMapWarp& inInput, const WarpType inWarpType, const float inRadius, const int32 inShapeID, const FTransform& inTrs, int32 inGroupId);
	static TArray<class AGsTerritorySpawnActor*> CreateSpawnActor(const FGsTerritorySpawnInfo& inSpawn, int32 inSpawnCount, EGsTerritorySpawnShapeType inShape, AGsTerritoryZone* inZone, FTransform& inTrans);

public:
	static class AGsTerritoryFenceBase* CreateFenceTown(const FTransform& inTrs
		, const FText& inName = FText()
		, const FGsSchemaSoundResDataRow& inSound = FGsSchemaSoundResDataRow());
	static class AGsTerritoryFenceBase* CreateFenceSafeZone(const FTransform& inTrs
		, const FText& inName = FText()
		, const FGsSchemaSoundResDataRow& inSound = FGsSchemaSoundResDataRow());
	static class AGsTerritoryFenceBase* CreateFenceTownChaos(const FTransform& inTrs
		, const FText& inName = FText()
		, const FGsSchemaSoundResDataRow& inSound = FGsSchemaSoundResDataRow());
	static class AGsTerritoryFenceBase* CreateFenceArena(const FTransform& inTrs
		, const FText& inName = FText()
		, const FGsSchemaSoundResDataRow& inSound = FGsSchemaSoundResDataRow());
	static class AGsTerritoryFenceBase* CreateFenceArenaReady(const FTransform& inTrs
		, const FText& inName = FText()
		, const FGsSchemaSoundResDataRow& inSound = FGsSchemaSoundResDataRow());
	static class AGsTerritoryFenceBase* CreateFenceArrival(const FTransform& inTrs
		, const FText& inName = FText()
		, const FGsSchemaSoundResDataRow& inSound = FGsSchemaSoundResDataRow());
	static class AGsTerritoryFenceBase* CreateFenceEnv(const FTransform& inTrs
		, const FText& inName = FText()
		, const TArray<FGsEnvCondition>& inEnvConditionArray = TArray<FGsEnvCondition>());	
	static class AGsTerritoryFenceBase* CreateFenceSound(const FTransform& inTrs
		, const FText& inName = FText()
		, const TArray<FGsQuestSound>& inQuestSoundArray = TArray<FGsQuestSound>());
	static class AGsTerritoryFenceBase* CreateFenceSpaceCrack(const FTransform& inTrs
		, const FText& inName = FText()
		, EGsEnvEvent inEnvEvent = EGsEnvEvent::NONE);
	static class AGsTerritoryFenceBase* CreateNoCampZone(const FTransform& inTrs
		, const FText& inName = FText());
	static class AGsTerritoryFenceBase* CreateFenceNonPK(const FTransform& inTrs
		, const FText& inName = FText());


public:
	static class AGsTerritoryLinePatrol* CreatePatrolLine(const FTransform& inTrs);

public:
	static EGsTerritorySpawnType ConvertObjectTypeToSpawnType(EGsTerritoryObjType inObjType);
	static EGsTerritorySpawnType ConvertCreatureTypeToSpawnType(CreatureSpawnType inCreatureType);
	static EGsTerritoryObjType ConvertSpawnTypeToObjectType(EGsTerritorySpawnType inSpawnType);
	static AGsTerritoryActor* FindTerritoryActor(int64 inUniqueId);

public:
	static class UGsTableNpcData* GetCachedNpcData();
	static class UGsTableShapeData* GetCachedShapeData();
	static class UGsTablePropData* GetCachedPropData();
	static class UGsTableMapData* GetCachedMapData();
	static class UGsTableQuest* GetCachedQuestData();
	static class UGsTableAreaData* GetCachedAreaData();

	//테스트
//private:
//	void OnAddZoneTest();
//	void OnConvertFenceToFenceBase();
};