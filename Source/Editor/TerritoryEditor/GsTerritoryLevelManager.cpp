// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryLevelManager.h"

//UE4
#include "Engine/Selection.h"
#include "Editor.h"
//#include "Editor/WorldBrowser/Private/Tiles/WorldTileModel.h"

//T1 project
#include "Editor/GsSelectionPrevantInterface.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"

//territory editor
#include "TerritoryEditor.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryEditorFileUtil.h"

#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "Editor/TerritoryEditor/Actor/Spawn/SpawnActor/GsTerritoryProp.h"
#include "Actor/Warp/GsTerritoryWarp.h"

#include "Actor/Spot/GsTerritorySpot.h"
#include "Engine/World.h"
#include "Engine/WorldComposition.h"
#include "Engine/LevelStreaming.h"
#include "IAssetTypeActions.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "Private/ContentBrowserUtils.h"
#include "UObjectGlobals.h"
#include "Private/LevelFolders.h"
#include "Blutility/GsTerritoryGroundActionUtility.h"
#include "Map/GsSchemaMapData.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "GsTerritoryEditorSetting.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GsSchemaBase.h"
#include "UTIL/GsTableUtil.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "GsTableManagerEditor.h"
#include "Actor/Spot/GsTerritoryWarpSpot.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Stats.h"
#include "Data/GsTerritoryDataManager.h"
#include "GsTerritoryUniqueIDManager.h"
#include "UTIL/GsText.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "TerritoryEditor/Mode/GsTerrEdModeDefine.h"
#include "ScopedSlowTask.h"
#include "Mode/GsTerrEdModeStateManagerBase.h"
#include "Mode/GsTerrEdEditorModeState.h"
#include "Mode/EditorMode/GsTerrEdEditorModeLevelEdModeState.h"
#include "Actor/Line/GsTerritoryLine.h"
#include "Actor/Line/GsTerritoryLinePatrol.h"
#include "Actor/Spawn/Zone/GsTerritoryZoneBase.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "Actor/Fence/GsTerritoryFenceTownChaos.h"
#include "Actor/Fence/GsTerritoryFenceNonPK.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/Fence/GsSchemaFenceData.h"

#define LOCTEXT_NAMESPACE	"TerritoryEditor"

DEFINE_STAT(STAT_ServerLevelCopy);

#define TEST_SAVE_MAP true
bool FGsTerritoryLevelManager::_isLoadArtLevel = true;
EGsTerrEdShowType FGsTerritoryLevelManager::_widgetShowType = EGsTerrEdShowType::ShowAll;
EGsTerrEdShowType FGsTerritoryLevelManager::_shapeShowType = EGsTerrEdShowType::ShowAll;
EGsTerrEdShowType FGsTerritoryLevelManager::_sightRadiusShowType = EGsTerrEdShowType::ShowAll;
float FGsTerritoryLevelManager::_widgetVisibleDistance = 3000;
float FGsTerritoryLevelManager::_shapeVisibleDistance = 3000;
float FGsTerritoryLevelManager::_sightRadiusVisibleDistance = 3000;
bool FGsTerritoryLevelManager::_isTransparentFloor = false;
uint8 FGsTerritoryLevelManager::_transparentFloorAlpha = 100;
bool FGsTerritoryLevelManager::_isScalabilityQualityLow = false;

UGsTableNpcData* FGsTerritoryLevelManager::_cachedNpcData = nullptr;
UGsTableShapeData* FGsTerritoryLevelManager::_cachedShapeData = nullptr;
UGsTablePropData* FGsTerritoryLevelManager::_cachedPropData = nullptr;
UGsTableMapData* FGsTerritoryLevelManager::_cachedMapData = nullptr;
UGsTableQuest* FGsTerritoryLevelManager::_cachedQuestData = nullptr;
UGsTableAreaData* FGsTerritoryLevelManager::_cachedAreaData = nullptr;

void FGsTerritoryLevelManager::Initialize()
{
	//selection
	_childShapeSelectHandle = USelection::SelectObjectEvent.AddRaw(this, &FGsTerritoryLevelManager::OnCheckSelectShapeComponent);	
	
	ActiveSaveLevel(false);	

#if !REFACTORY_SAVE_ALL_MAP
	SetSaveMapAll(false);
#endif

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_widgetVisibleDistance = settings->_InitialWidgetViewDistance;
		_shapeVisibleDistance = settings->_InitialActorViewDistance;
		_sightRadiusVisibleDistance = settings->_InitialSightRadiusViewDistance;
		_transparentFloorAlpha = settings->_initTransparentFloorAlpha;
	}
}

void FGsTerritoryLevelManager::Deinitialize()
{
	//selection
	if(_childShapeSelectHandle.IsValid())
	{
		USelection::SelectObjectEvent.Remove(_childShapeSelectHandle);
	}	
}

//배치 액터 관련된 함수 ===============================================================================================================
void FGsTerritoryLevelManager::OnCheckSelectShapeComponent(UObject* in_object)
{
	if (in_object)
	{
		if (IGsSelectionPrevantInterface* prevant = Cast<IGsSelectionPrevantInterface>(in_object))
		{
			GEditor->SelectNone(true, true);
		}
	}
}

void FGsTerritoryLevelManager::LoadArtLevelsInEditorTime(const FString& inFileName, bool in_template)
{
	if (false == _isLoadArtLevel)
	{
		return;
	}

	if (UWorld* world = GEditor->GetEditorWorldContext().World())
	{
		if (false == world->IsPlayInEditor())
		{
			if (world->WorldComposition)
			{
				UGsTerritoryLevelUtil::LoadWorkLevels(world, EGsTerritoryLevelType::Art);
			}
		}
	}
}

void FGsTerritoryLevelManager::SetInvalidTerritoryActor(AGsTerritoryActor* inActor, const FString& inInvalidReason)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::SetInvalidTerritoryActor - label : %s"), *inActor->GetActorLabel());

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(inActor->GetLevel()->GetPathName());
	FString levelKey = TEXT("NonServerLevel");
	FString persistantLevelName;
	UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey);

	inActor->SetValid(false);

	const FString& newName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(inActor->GetTerritoryObjType());
	const FString& log = FString::Printf(TEXT("FGsTerritoryLevelManager::SetInvalidTerritoryActor - Invalid territory actor created - name : %s"), *newName);
	FGsTerritoryEditorUtil::ShowWarningNotification(log);

	inActor->SetInvalidReason(inInvalidReason);
	inActor->SetActorLabel(newName);
}

void FGsTerritoryLevelManager::OnDeleteTerritoryActorInNonServerLevel(AActor* inActor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnDeleteTerritoryActorInNonServerLevel - label : %s"), *inActor->GetActorLabel());

	if (inActor)
	{
		if (AGsTerritoryActor* actor = Cast<AGsTerritoryActor>(inActor))
		{
			actor->DestroyManually();

			const FString& log = TEXT("Terratory editor actor can't be created at non server level");
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(log));

			actor->_onTerritoryActorConstruct.Clear();
		}
	}
}

void FGsTerritoryLevelManager::TerritoryActorDeleted(AActor* inActor)
{
	if (nullptr == inActor
		&& inActor->HasAnyFlags(EObjectFlags::RF_Transient)
		&& false == inActor->IsA(AGsTerritoryActor::StaticClass()))
	{
		return;
	}

	if (AGsTerritoryActor* actor = Cast<AGsTerritoryActor>(inActor))
	{
		actor->InitializeTerritoryUniqueID();

		const FString& actorName = inActor->GetActorLabel();
		const FString& levelName = inActor->GetLevel()->GetName();

		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::TerritoryActorDeleted - Delete actor - label : %s \tlevel : %s"), *actorName, *levelName);
	}
}

//맵 로드, 저장 관련된 함수===============================================================================================================
/// <summary>
/// Start save all map
/// </summary>
void FGsTerritoryLevelManager::NavigationAllServerLevel()
{
#if !REFACTORY_SAVE_ALL_MAP
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::StartSaveAllMap"));

	ActiveSaveLevel(true);
	SetSaveMapAll(true);
	_currentLevelIndex = 0;
	_currentLevelId = GetLevelKeyByIndex(_currentLevelIndex);

	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return;
	}

	if (_additionalLevelEventHandle.IsValid())
	{
		_additionalLevelEvent.Remove(_additionalLevelEventHandle);
	}
	
	editorMode->ChangeState(TERR_ED_EDITOR_MODE_SERVER_LEVEL_NAVIGATION_WORK_MODE);
#else
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::StartSaveAllMap"));

	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return;
	}

	editorMode->ChangeState(TERR_ED_EDITOR_MODE_SERVER_LEVEL_NAVIGATION_WORK_MODE);
#endif
}

void FGsTerritoryLevelManager::NavigationAllDesignLevel()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::StartSaveAllMap"));

	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return;
	}

	editorMode->ChangeState(TERR_ED_EDITOR_MODE_DESIGN_LEVEL_NAVIGATION_WORK_MODE);
}

void FGsTerritoryLevelManager::NatigationAllLevel()
{
	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return;
	}

	editorMode->ChangeState(TERR_ED_EDITOR_MODE_LEVEL_NAVIGATION_WORK_MODE);
}

#if !REFACTORY_SAVE_ALL_MAP
void FGsTerritoryLevelManager::FinishSaveAllMap()
{
	FinishSaveMap();

	UE_LOG(LogTemp, Log, TEXT("FGsTerritoryLevelManager::FinishSaveAllMap"));

	ActiveSaveLevel(false);
	SetSaveMapAll(false);

	_currentLevelId = INDEX_NONE;
	if (_additionalLevelEventHandle.IsValid())
	{
		_additionalLevelEvent.Remove(_additionalLevelEventHandle);
	}

	FTerritoryEditor::GetInstance()->ChangeLevelEditMode();
	FTerritoryEditor::GetInstance()->ChangeLevelEdNewCurrentLevelMode();
}

/// <summary>
/// Check still save all map work?
/// </summary>
void FGsTerritoryLevelManager::OnUpdateSaveMap(float inDelta)
{
	//만약 자동 저장 레벨 기능이 꺼져있으면 저장하지 않는다. 
	//맵 로드 - 서버 레벨 로드 - 서버 레벨 수정 - 저장 후 다시 Active 되면 다음 레벨을 저장한다. 
	if (false == _isActiveSaveLevel)
	{
		return;
	}		

	_currentLevelId = GetLevelKeyByIndex(_currentLevelIndex);

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnUpdateSaveMap - index : %d,\tlevel id : %d"), _currentLevelIndex, _currentLevelId);

	if (INDEX_NONE == _currentLevelId)
	{
		FinishSaveAllMap();
	}

	ActiveSaveLevel(false);
	if(false == StartSaveMap(_currentLevelId))
	{
		FinishSaveAllMap();
	}
}

//서버 레벨 저장 
bool FGsTerritoryLevelManager::StartSaveMap(int32 inLevelId)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::StartSaveMap - start"));

	_currentLevelId = inLevelId;
	_isLoadArtLevel = false;	

	//레벨에 없으면 끝
	int32 levelIndex = GetLevelIndexByKey(inLevelId);	
	if (INDEX_NONE == levelIndex)
	{
		FinishSaveMap();
		return false;
	}
	
	const FGsSchemaMapData* mapData = GetMapDataByIndex(levelIndex);
	if (nullptr == mapData)
	{
		FinishSaveMap();
		return false;
	}

	FString levelName = UGsTerritoryLevelUtil::GetLevelName(mapData->id);
	if (levelName.Equals(""))
	{
		FinishSaveMap();
		return true;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::StartSaveMap - End"));

	return OpenLevelForSaveAllMap(levelName);	
}
#endif

/// <summary>
/// 연속적으로 맵을 저장하는 기능에서 하나의 맵 저장이 끝날 을 때 호출한다.
/// </summary>
void FGsTerritoryLevelManager::FinishSaveMap()
{	
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::FinishSaveMap"));

	_loadLevelList.Empty();
	
#if !REFACTORY_SAVE_ALL_MAP
	if (_isSaveAllMap)
	{
		ActiveSaveLevel(true);
		_currentLevelIndex = ++_currentLevelIndex;
	}
	else
	{
		_currentLevelId = INDEX_NONE;
		_currentLevelIndex = INDEX_NONE;
		_isLoadArtLevel = true;
	}
#else
	_currentLevelId = INDEX_NONE;
	_currentLevelIndex = INDEX_NONE;
	_isLoadArtLevel = true;
#endif
}

void FGsTerritoryLevelManager::ActiveSaveLevel(bool inActive)
{
	_isActiveSaveLevel = inActive;
}

#if !REFACTORY_SAVE_ALL_MAP
void FGsTerritoryLevelManager::SetSaveMapAll(bool inValue)
{
	_isSaveAllMap = inValue;
}

/// <summary>
/// 연속적으로 맵을 저장하기 위해 사용하는 맵 열림 함수
/// </summary>
/// <param name="inLevelName"></param>
/// <returns></returns>
bool FGsTerritoryLevelManager::OpenLevelForSaveAllMap(const FString& inLevelName)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OpenLevelForSaveAllMap - Start"));

	const FString& PackageName = TEXT("/Game/Maps/") + inLevelName + TEXT("/") + inLevelName;
	TArray<FAssetData> ActivatedAssets;
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	if (!AssetRegistry.GetAssetsByPackageName(*PackageName, ActivatedAssets))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("!AssetRegistry.GetAssetsByPackageName(%s, Assets)"), *PackageName);

		FinishSaveMap();
		return false;
	}

	TMap< TSharedRef<IAssetTypeActions>, TArray<UObject*> > TypeActionsToObjects;
	TArray<UObject*> ObjectsWithoutTypeActions;

	const FText& LoadingTemplate = FText::FromString(TEXT("Loading {0}..."));
	const FText& DefaultText = ActivatedAssets.Num() == 1 ? FText::Format(LoadingTemplate, FText::FromName(ActivatedAssets[0].AssetName)) : LOCTEXT("LoadingObjects", "Loading Objects...");
	FScopedSlowTask SlowTask(100, DefaultText);

	// Iterate over all activated assets to map them to AssetTypeActions.
	// This way individual asset type actions will get a batched list of assets to operate on
	for (auto AssetIt = ActivatedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
	{
		const FAssetData& AssetData = *AssetIt;
		if (!AssetData.IsAssetLoaded() && FEditorFileUtils::IsMapPackageAsset(AssetData.ObjectPath.ToString()))
		{
			SlowTask.MakeDialog();
		}

		SlowTask.EnterProgressFrame(75.f / ActivatedAssets.Num(), FText::Format(LoadingTemplate, FText::FromName(AssetData.AssetName)));

		UObject* Asset = (*AssetIt).GetAsset();
		if (Asset != NULL)
		{
			FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
			TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(Asset->GetClass());
			if (AssetTypeActions.IsValid())
			{
				// Add this asset to the list associated with the asset type action object
				TArray<UObject*>& ObjList = TypeActionsToObjects.FindOrAdd(AssetTypeActions.Pin().ToSharedRef());
				ObjList.AddUnique(Asset);
			}
			else
			{
				ObjectsWithoutTypeActions.AddUnique(Asset);
			}
		}
	}

	EAssetTypeActivationMethod::Type ActivationMethod = EAssetTypeActivationMethod::DoubleClicked;
	// Now that we have created our map, activate all the lists of objects for each asset type action.
	for (auto TypeActionsIt = TypeActionsToObjects.CreateConstIterator(); TypeActionsIt; ++TypeActionsIt)
	{
		SlowTask.EnterProgressFrame(25.f / TypeActionsToObjects.Num());

		const TSharedRef<IAssetTypeActions>& TypeActions = TypeActionsIt.Key();
		const TArray<UObject*>& ObjList = TypeActionsIt.Value();

		//현재 맵 레벨 저장하지 않기 
		for (TObjectIterator<UWorld> WorldIt; WorldIt; ++WorldIt)
		{
			UPackage* WorldPackage = WorldIt->GetOutermost();
			if (!WorldPackage->HasAnyPackageFlags(PKG_PlayInEditor)
				&& !WorldPackage->HasAnyFlags(RF_Transient))
			{
				UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OpenLevelForSaveAllMap - OpenLevelForSaveAllMap - SetDirty false : %s"), *WorldPackage->GetName());
				WorldPackage->SetDirtyFlag(false);
			}			
		}

		if (!TypeActions->AssetsActivatedOverride(ObjList, ActivationMethod))
		{
			if (ActivationMethod == EAssetTypeActivationMethod::DoubleClicked || ActivationMethod == EAssetTypeActivationMethod::Opened)
			{
				if (ObjList.Num() == 1)
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(ObjList[0]);
				}
				else if (ObjList.Num() > 1)
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAssets(ObjList);
				}
			}
		}
	}

	return true;
}

/*
맵이 오픈 되고 난 후 콜백
*/
void FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap(const FString& inFilename, bool inIsTemplate)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnOpenLevel Start"));
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnOpenLevel - level index : %d\tlevel key : %d"), _currentLevelIndex, _currentLevelId);

	if (_levelOpenHandle.IsValid())
	{
		FEditorDelegates::OnMapOpened.Remove(_levelOpenHandle);
	}	

	UWorld* world = GEditor->GetEditorWorldContext().World();
	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> levelList;
	if (false == UGsTerritoryLevelUtil::TryGetWorkLevels(levelCollection, levelList, world, EGsTerritoryLevelType::Server))
	{
		//만약 서버레벨이 하나도 없다면 		
		return;
	}
	
	bool isFind = false;
	for (TSharedPtr<FLevelModel> level : levelList)
	{
		const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(level->GetPackageFileName()));		
		FString persistantLevel;
		FString levelKey;
		if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevel, levelKey))
		{
			continue;
		}

		if (FCString::Atoi(*levelKey) == _currentLevelId)
		{
			UE_LOG(LogTemp, Log, TEXT("FGsTerritoryLevelManager - load streaming level start - level index : %d\tlevel key : %d"), _currentLevelIndex, _currentLevelId);

			//Bind OnLoadedServerLevel
			if (_addServerLevelHandle.IsValid())
			{
				FEditorDelegates::NewCurrentLevel.Remove(_addServerLevelHandle);
			}
			
			//맵을 열고 최초 Server level 오픈되고 현재 Level로 설정되는 시점
			_addServerLevelHandle = FEditorDelegates::NewCurrentLevel.AddRaw(this, &FGsTerritoryLevelManager::OnLoadedServerLevelForSaveAllMap);

			TArray<TSharedPtr<FLevelModel>> loadLevelList;
			loadLevelList.Add(level);
			_loadLevelList = loadLevelList;
			level->MakeLevelCurrent();

			isFind = true;
			break;
		}
	}

	//만약 로드하는 레벨이 하나도 없다면
	if (false == isFind)
	{		
		FinishSaveMap();
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnOpenLevel End"));
}

//서버 레벨 추가시 콜백
void FGsTerritoryLevelManager::OnLoadedServerLevelForSaveAllMap()
{	
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnLoadedServerLevel - Start"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	FString levelName;
	FString levelKey;
	if (false == UGsTerritoryLevelUtil::IsValidServerLevel(level, levelName, levelKey)
		&& FCString::Atoi(*levelKey) != _currentLevelId)
	{		
		return;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnAddServerLevel - start"));
	//Unbind onChange level
	if (_addServerLevelHandle.IsValid())
	{
		FEditorDelegates::NewCurrentLevel.Remove(_addServerLevelHandle);
	}

	//액터 수정을 위해 unique id manager 초기화 
	//이곳에서 unique id manager를 초기화 하는 로직은 자동으로 레벨을 저장할 때만 사용한다. 
	FGsTerritoryUniqueIDManager* uniqueIdManager = FTerritoryEditor::GetInstance()->GetUniqueIDManager();
	if (uniqueIdManager)
	{
		uniqueIdManager->Initialize();
		uniqueIdManager->OnCheckServerLevelLoaded();
	}	

	FTerritoryEditor::GetInstance()->BindActorAdded();
	FTerritoryEditor::GetInstance()->BindActorDeleted();

	_additionalLevelEvent.Broadcast();

	SaveServerLevel();	

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnLoadedServerLevel - End"));
}

#endif

void FGsTerritoryLevelManager::SaveServerLevel()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - SaveServerLevel - Start"));
	UWorld* world = GEditor->GetEditorWorldContext().World();

	//bind post save event
	if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}	
	_postSaveHandle = FEditorDelegates::PostSaveWorld.AddRaw(this, &FGsTerritoryLevelManager::OnPostSaveServerLevel);

	//start save 
	FWorldBrowserModule& worldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>("WorldBrowser");
	TWeakPtr<FLevelCollectionModel> levelCollectionModelPtr = worldBrowserModule.SharedWorldModel(world);
	if (levelCollectionModelPtr.IsValid())
	{
		levelCollectionModelPtr.Pin()->SaveLevels(_loadLevelList);
	}	

	_loadLevelList.Empty();

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - SaveServerLevel - End"));
}

//Post Save Event
void FGsTerritoryLevelManager::OnPostSaveServerLevel(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess)
{
	if (GIsCookerLoadingPackage)
	{
		return;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnPostSaveServerLevel - Start"));
	
	if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}

	FGsTerritoryUniqueIDManager* uniqueIdManager = FTerritoryEditor::GetInstance()->GetUniqueIDManager();
	if (uniqueIdManager)
	{
		uniqueIdManager->OnPostSaveUniqueTable(inSaveFlag, inWorld, inSuccess);
	}

	FGsTerritoryDataManager* dataManager = FTerritoryEditor::GetInstance()->GetDataManager();
	if (dataManager)
	{
		dataManager->Save(inSaveFlag, inWorld, inSuccess);
	}	
	
	_saveMapFinishEvent.Broadcast();			

	FinishSaveMap();

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - OnPostSaveServerLevel - End"));
}

//========================================================================================================

bool FGsTerritoryLevelManager::OpenLevel(const FString& inLevelName)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OpenLevel - level name : %s"), *inLevelName);

	const FString& PackageName = TEXT("/Game/Maps/") + inLevelName + TEXT("/") + inLevelName;
	TArray<FAssetData> ActivatedAssets;
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	if (!AssetRegistry.GetAssetsByPackageName(*PackageName, ActivatedAssets))
	{
		UE_LOG(LogTemp, Error, TEXT("!AssetRegistry.GetAssetsByPackageName(%s, Assets)"), *PackageName);		
		return false;
	}

	TMap< TSharedRef<IAssetTypeActions>, TArray<UObject*> > TypeActionsToObjects;
	TArray<UObject*> ObjectsWithoutTypeActions;

	const FText& LoadingTemplate = FText::FromString(TEXT("Loading {0}..."));
	const FText& DefaultText = ActivatedAssets.Num() == 1 ? FText::Format(LoadingTemplate, FText::FromName(ActivatedAssets[0].AssetName)) : LOCTEXT("LoadingObjects", "Loading Objects...");
	FScopedSlowTask SlowTask(100, DefaultText);

	// Iterate over all activated assets to map them to AssetTypeActions.
	// This way individual asset type actions will get a batched list of assets to operate on
	for (auto AssetIt = ActivatedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
	{
		const FAssetData& AssetData = *AssetIt;
		if (!AssetData.IsAssetLoaded() && FEditorFileUtils::IsMapPackageAsset(AssetData.ObjectPath.ToString()))
		{
			SlowTask.MakeDialog();
		}
		SlowTask.EnterProgressFrame(75.f / ActivatedAssets.Num(), FText::Format(LoadingTemplate, FText::FromName(AssetData.AssetName)));

		UObject* Asset = (*AssetIt).GetAsset();
		if (Asset != NULL)
		{
			FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
			TWeakPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(Asset->GetClass());
			if (AssetTypeActions.IsValid())
			{
				// Add this asset to the list associated with the asset type action object
				TArray<UObject*>& ObjList = TypeActionsToObjects.FindOrAdd(AssetTypeActions.Pin().ToSharedRef());
				ObjList.AddUnique(Asset);
			}
			else
			{
				ObjectsWithoutTypeActions.AddUnique(Asset);
			}
		}
	}

	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return false;
	}

	editorMode->ChangeState(TERR_ED_EDITOR_MODE_LEVEL_COPY_MODE);

	EAssetTypeActivationMethod::Type ActivationMethod = EAssetTypeActivationMethod::DoubleClicked;
	// Now that we have created our map, activate all the lists of objects for each asset type action.
	for (auto TypeActionsIt = TypeActionsToObjects.CreateConstIterator(); TypeActionsIt; ++TypeActionsIt)
	{
		SlowTask.EnterProgressFrame(25.f / TypeActionsToObjects.Num());

		const TSharedRef<IAssetTypeActions>& TypeActions = TypeActionsIt.Key();
		const TArray<UObject*>& ObjList = TypeActionsIt.Value();

		//현재 맵 레벨 저장하지 않기 
		for (TObjectIterator<UWorld> WorldIt; WorldIt; ++WorldIt)
		{
			UPackage* WorldPackage = WorldIt->GetOutermost();
			if (!WorldPackage->HasAnyPackageFlags(PKG_PlayInEditor)
				&& !WorldPackage->HasAnyFlags(RF_Transient))
			{
				UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OpenLevel - SetDirty false : %s"), *WorldPackage->GetName());
				WorldPackage->SetDirtyFlag(false);
			}
		}

		if (!TypeActions->AssetsActivatedOverride(ObjList, ActivationMethod))
		{
			if (ActivationMethod == EAssetTypeActivationMethod::DoubleClicked || ActivationMethod == EAssetTypeActivationMethod::Opened)
			{
				if (ObjList.Num() == 1)
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(ObjList[0]);
				}
				else if (ObjList.Num() > 1)
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAssets(ObjList);
				}
			}
		}
	}	

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OpenLevel - End"));

	return true;
}

void FGsTerritoryLevelManager::OnOpenLevel(const FString& inFileName, bool inIsTemplate)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevel - start - level key : %d"), _currentLevelId);

	//load art level
	UWorld* world = GEditor->GetEditorWorldContext().World();
	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> levelList;
	if (false == UGsTerritoryLevelUtil::TryGetWorkLevels(levelCollection, levelList, world, EGsTerritoryLevelType::Server))
	{
		//만약 서버레벨이 하나도 없다면 복사 취소
		return;
	}
	
	//서버 레베 로드
	bool isFind = false;
	for (TSharedPtr<FLevelModel> level : levelList)
	{
		const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(level->GetPackageFileName()));
		FString persistantLevel;
		FString levelKey;
		if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevel, levelKey))
		{
			continue;
		}

		if (FCString::Atoi(*levelKey) == _currentLevelId)
		{
			isFind = true;

			UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevel - load streaming level start - level key : %d"), _currentLevelId);

			TArray<TSharedPtr<FLevelModel>> loadLevelList;
			loadLevelList.Add(level);
			_loadLevelList = loadLevelList;
			level->MakeLevelCurrent();

			break;
		}
	}

	//만약 로드하는 레벨이 하나도 없다면
	if (false == isFind)
	{
		BindMessage();	
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevel - End"));
}

void FGsTerritoryLevelManager::OnLoadedServerLevel()
{	
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnLoadedServerLevel - Start"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	FString levelName;
	FString levelKey;
	if (false == UGsTerritoryLevelUtil::IsValidServerLevel(level, levelName, levelKey)
		&& FCString::Atoi(*levelKey) != _currentLevelId)
	{
		return;
	}
	
	//Unbind onChange level
	if (_addServerLevelHandle.IsValid())
	{
		FEditorDelegates::NewCurrentLevel.Remove(_addServerLevelHandle);
	}

	//액터 수정을 위해 unique id manager 초기화 
	//이곳에서 unique id manager를 초기화 하는 로직은 자동으로 레벨을 저장할 때만 사용한다. 
	FGsTerritoryUniqueIDManager* uniqueIdManager = FTerritoryEditor::GetInstance()->GetUniqueIDManager();
	if (uniqueIdManager)
	{
		uniqueIdManager->Initialize();
		uniqueIdManager->OnCheckServerLevelLoaded();
	}

	FTerritoryEditor::GetInstance()->BindActorAdded();
	FTerritoryEditor::GetInstance()->BindActorDeleted();

	if (_additionalLevelEvent.IsBound())
	{
		_additionalLevelEvent.Broadcast();
	}		

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnLoadedServerLevel - End"));
}

void FGsTerritoryLevelManager::OnCopyLevel()
{
	//copy 
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnCopyLevel - start - level : %d"), _currentLevelId);

	const FText& defaultText = FText::FromString(TEXT("ServerLevel Copy"));
	FScopedSlowTask slowTask(100, defaultText);

	_additionalLevelEvent.Clear();

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnCopyLevel - Delete all territory"));

	if (_copySpawnActor)
	{
		DeleteAllSpawnActor();
	}

	if (_copySpot)
	{
		DeleteAllSpotActor();
	}

	if (_copyFence)
	{
		DeleteAllFence();
	}

	if (_copyLine)
	{
		DeleteAllLine();
	}

	slowTask.EnterProgressFrame(25, FText::FromString(TEXT("Delete all server level")));

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnCopyLevel - Copy all territory actor"));

	CopyAllTerritoryActor();

	slowTask.EnterProgressFrame(25, FText::FromString(TEXT("Copy territory actor")));
	
	_isLoadArtLevel = true;

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnCopyLevel - Load art level"));

	//load art level
	UWorld* world = GEditor->GetEditorWorldContext().World();
	UGsTerritoryLevelUtil::LoadWorkLevels(world, EGsTerritoryLevelType::Art);

	slowTask.EnterProgressFrame(50, FText::FromString(TEXT("Load art level")));

	//Finish copy level
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnCopyLevel - Finish copy level"));

	SaveServerLevel();
}

void FGsTerritoryLevelManager::BindMessage()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::BindMessage"));

	FTerritoryEditor::GetInstance()->BindActorAdded();
	FTerritoryEditor::GetInstance()->BindActorDeleted();
	FTerritoryEditor::GetInstance()->BindPostSave();
	FTerritoryEditor::GetInstance()->BindMapOpen();
	FTerritoryEditor::GetInstance()->BindNewCurrentLevel();
}

void FGsTerritoryLevelManager::UnbindMessage()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::UnbindMessage"));

	FTerritoryEditor::GetInstance()->UnbindMapOpen();
	FTerritoryEditor::GetInstance()->UnbindPostSave();
	FTerritoryEditor::GetInstance()->UnbindActorAdded();
	FTerritoryEditor::GetInstance()->UnbindActorDeleted();
	FTerritoryEditor::GetInstance()->UnbindNewCurrentLevel();
}

void FGsTerritoryLevelManager::DeleteAllTerritoryActor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::DeleteAllTerritoryActor"));

	DeleteAllSpawnActor();
	DeleteAllSpotActor();
	DeleteAllFence();
	DeleteAllLine();
}

void FGsTerritoryLevelManager::DeleteAllSpawnActor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::DeleteAllSpawnActor"));

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(false);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}
	
	TArray<AActor*> zones;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryZone::StaticClass(), zones);
	for (AActor* temp : zones)
	{
		if (temp)
		{
			if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(temp))
			{
				zone->DestroyManually();
			}
		}
	}

	//delete npc prop actor out of zone
	TArray<AActor*> npcArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryNpc::StaticClass(), npcArray);
	for (AActor* temp : npcArray)
	{
		if (temp)
		{
			if (AGsTerritoryNpc* npc = Cast<AGsTerritoryNpc>(temp))
			{
				npc->DestroyManually();
			}
		}
	}

	//delete npc prop actor out of zone
	TArray<AActor*> propArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryProp::StaticClass(), propArray);
	for (AActor* temp : propArray)
	{
		if (temp)
		{
			if (AGsTerritoryProp* prop = Cast<AGsTerritoryProp>(temp))
			{
				prop->DestroyManually();
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(true);
}

void FGsTerritoryLevelManager::DeleteAllSpotActor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::DeleteAllSpotActor"));

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(false);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	TArray<AActor*> spotArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritorySpot::StaticClass(), spotArray);
	for (AActor* temp : spotArray)
	{
		if (temp)
		{
			if (AGsTerritorySpot* spot = Cast<AGsTerritorySpot>(temp))
			{
				spot->DestroyManually();
			}
		}
	}

	////delete npc prop actor out of zone
	//TArray<AActor*> rttArray;
	//UGameplayStatics::GetAllActorsOfClass(world, AGsTerrEdRTTActor::StaticClass(), rttArray);
	//for (AActor* temp : rttArray)
	//{
	//	if (temp)
	//	{
	//		if (AGsTerrEdRTTActor* rtt = Cast<AGsTerrEdRTTActor>(temp))
	//		{
	//			rtt->DestroyManually();
	//		}
	//	}
	//}

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(true);
}

void FGsTerritoryLevelManager::DeleteAllFence()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::DeleteAllFence"));

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(false);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	TArray<AActor*> fenceArray;	
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryFenceBase::StaticClass(), fenceArray);
	for (AActor* temp : fenceArray)
	{
		if (nullptr == temp)
		{
			continue;
		}

		AGsTerritoryFenceBase* fence = Cast<AGsTerritoryFenceBase>(temp);
		if (nullptr == fence)
		{
			continue;
		}

		fence->DestroyManually();
	}

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(true);
}

void FGsTerritoryLevelManager::DeleteAllLine()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::DeleteAllFence"));

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(false);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	TArray<AActor*> lineArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryLine::StaticClass(), lineArray);
	for (AActor* actor : lineArray)
	{
		if (actor)
		{
			if (AGsTerritoryLine* castLine = Cast<AGsTerritoryLine>(actor))
			{
				castLine->DestroyManually();
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(true);
}

void FGsTerritoryLevelManager::CopyAllTerritoryActor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CopyAllTerritoryActor"));

	if (_copySpawnActor)
	{
		CopyAllSpawnActor();
	}
	
	if (_copySpot)
	{
		CopyAllSpot();
	}
	
	if (_copyFence)
	{
		CopyAllFence();
	}	

	if (_copyLine)
	{
		CopyAllLine();
	}
}

void FGsTerritoryLevelManager::CopyAllSpawnActor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CopyAllSpawnActor"));

	const FString& levelId = FString::FromInt(_copySourceLevelId);
	const FString& spawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(levelId);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaSpawnData::StaticStruct(), spawnDataTableName);
	if (nullptr == table)
	{
		return;
	}

	UGsTableSpawnData* castTable = Cast<UGsTableSpawnData>(table);
	if (nullptr == castTable)
	{
		return;
	}

	//spacecrack은 prop에 포함된다.
	TArray<const FGsSchemaSpawnData*> spawnArray;
	castTable->GetAllRows<FGsSchemaSpawnData>(spawnArray);
	int32 spanwDataNum = spawnArray.Num();
	FTransform trs;
	for (int32 i = 0; i < spanwDataNum; ++i)
	{
		const FGsSchemaSpawnData* spawnData = spawnArray[i];
		if (nullptr == spawnData)
		{
			continue;
		}

		trs.SetLocation(spawnData->Region.Center);		
		AGsTerritoryZone* zone = CreateZone(spawnData->SpawnGroupType, spawnData->byClient, spawnData->byDefault, EGsTerritoryZoneShapeType::Circle, trs);		
		if (nullptr == zone)
		{
			continue;
		}

		zone->SetTerritoryUniqueIDForce(spawnData->SpawnGroupId);

		//zone->SetTerritoryUniqueID(spawnData->SpawnGroupId)

		zone->Import(spawnArray[i]);
	}

	//map territory data
	const FString& territoryDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelId);	
	UGsTable* territoryDataTable = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), territoryDataTableName);
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* territoryDataRow;
	territoryDataTable->FindRow<FGsSchemaMapTerritoryData>(*levelId, territoryDataRow);
	if (nullptr == territoryDataRow)
	{
		return;
	}
}

void FGsTerritoryLevelManager::CopyAllSpot()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CopyAllSpot"));

	const FString& levelId = FString::FromInt(_copySourceLevelId);
	const FString& tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelId);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	UGsTable* spotTable;
	FString spotTableName;
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaSpotData, UGsTableSpotData>(spotTable, spotTableName, levelId);
	spotTable = tableManager.GetTable(FGsSchemaSpotData::StaticStruct(), spotTableName);
	const FGsSchemaSpotData* spotData = nullptr;
	if (false == spotTable->FindRow<FGsSchemaSpotData>(*levelId, spotData))
	{
		return;
	}

	const FGsSchemaMapTerritoryData* territoryDataRow;
	table->FindRow<FGsSchemaMapTerritoryData>(*levelId, territoryDataRow);
	if (nullptr == territoryDataRow)
	{
		return;
	}	
	
	TArray<FGsSchemaSpotInfo> spotInfoArray = spotData->SpotList;

	//spawn spot
	{
		FTransform trs;
		int32 index = INDEX_NONE;
		const TArray<FGsSchemaMapSpotInfo>& spawnList = territoryDataRow->spawnList;
		AGsTerritorySpawnSpot* newSpawnSpot = nullptr;
		const FGsSchemaSpotInfo* findSpot = nullptr;
		for (const FGsSchemaMapSpotInfo& spawnSpotInfo : spawnList)
		{
			findSpot = spotInfoArray.FindByPredicate([&](const FGsSchemaSpotInfo& souce)->bool {
				return spawnSpotInfo.spotIndex == souce.id;
				});
			if (nullptr == findSpot)
			{
				continue;
			}
			
			trs.SetLocation(findSpot->pos);
			trs.SetRotation(FGsDir::AngleToRotator(findSpot->dir).Quaternion());
			newSpawnSpot = FGsTerritoryLevelManager::CreateSpawnSpot(findSpot->name, IffTeamType::NEUTRAL, findSpot->radius, trs);
			if (false == newSpawnSpot->SetIndexForce(findSpot->id))
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("Copy spawn spot fail - check territory data - index id : %d"), findSpot->id);
				continue;
			}

			index = spotInfoArray.IndexOfByPredicate([&](const FGsSchemaSpotInfo& spot)->bool {
				return spot.id == findSpot->id;
				});	

			if (spotInfoArray.IsValidIndex(index))
			{
				spotInfoArray.RemoveAt(index);
			}
		}
	}	

	//부활 좌표가 현재 맵이면 복사하고 아니면 복사하지 않음
	//부활 지점은 수동으로 입력하도록 수정 
	//이유 : 현재 부활 지점이 다른 맵에서 부활할 수 있기 때문에 인덱스 중복으로 에러남
	//resurrection spot
	{
		int32 index = INDEX_NONE;
		AGsTerritoryResurrectionSpot* newResurrectSpot;
		const TArray<FGsSchemaMapSpotInfo>& resurrectList = territoryDataRow->resurrectList;
		const FGsSchemaSpotInfo* findSpot = nullptr;
		FTransform trs;
		for (const FGsSchemaMapSpotInfo& ressurectSpotInfo : resurrectList)
		{
			if (nullptr == ressurectSpotInfo.mapId.GetRow())
			{
				continue;
			}

			if(ressurectSpotInfo.mapId.GetRow()->mapId != _copySourceLevelId)
			{
				continue;
			}

			findSpot = spotInfoArray.FindByPredicate([&](const FGsSchemaSpotInfo& souce)->bool {
				return ressurectSpotInfo.spotIndex == souce.id;
				});
			if (nullptr == findSpot)
			{
				continue;
			}
			
			trs.SetLocation(findSpot->pos);
			trs.SetRotation(FGsDir::AngleToRotator(findSpot->dir).Quaternion());
			newResurrectSpot = FGsTerritoryLevelManager::CreateResurrectionSpot(findSpot->name, IffTeamType::NEUTRAL, findSpot->radius, trs);
			if (false == newResurrectSpot->SetIndexForce(findSpot->id))
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("Copy resurrect spot fail - check territory data - index id : %d"), findSpot->id);
				continue;
			}
			
			index = spotInfoArray.IndexOfByPredicate([&](const FGsSchemaSpotInfo& souce)->bool {
				return souce.id == findSpot->id;
				});			

			if (spotInfoArray.IsValidIndex(index))
			{
				spotInfoArray.RemoveAt(index);
			}
		}
	}

	//teleport spot
	{
		FTransform trs;
		int32 index = INDEX_NONE;
		AGsTerritoryTeleportSpot* newTeleportSpot = nullptr;
		const FGsSchemaSpotInfo* findSpot = nullptr;
		const TArray<FGsSchemaMapSpotInfo>& teleportList = territoryDataRow->teleportList;
		for (const FGsSchemaMapSpotInfo& teleportSpotInfo : teleportList)
		{
			findSpot = spotData->SpotList.FindByPredicate([&](const FGsSchemaSpotInfo& souce)->bool {
				return teleportSpotInfo.spotIndex == souce.id;
				});
			if (nullptr == findSpot)
			{
				continue;
			}
			
			trs.SetLocation(findSpot->pos);
			trs.SetRotation(FGsDir::AngleToRotator(findSpot->dir).Quaternion());
			newTeleportSpot = FGsTerritoryLevelManager::CreateTeleportSpot(findSpot->name, IffTeamType::NEUTRAL, findSpot->radius, trs);
			if (false == newTeleportSpot->SetIndexForce(findSpot->id))
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("Copy teleport spot fail - check territory data - index id : %d"), findSpot->id);
				continue;
			}

			index = spotInfoArray.IndexOfByPredicate([&](const FGsSchemaSpotInfo& spot)->bool {
				return spot.id == findSpot->id;
				});
			
			if (spotInfoArray.IsValidIndex(index))
			{
				spotInfoArray.RemoveAt(index);
			}
		}
	}	
	
	//warp 
	{				
		const TArray<FGsSchemaMapWarpInfo>& warpList = territoryDataRow->warpList;
		FGsTerritoryMapWarp warpInput;		
		FTransform trs;
		int32 index = INDEX_NONE;
		AGsTerritoryWarpSpot* warpSpot = nullptr;
		const FGsSchemaSpotInfo* findSpot = nullptr;
		for (const FGsSchemaMapWarpInfo& warpSpotInfo : warpList)
		{
			findSpot = spotInfoArray.FindByPredicate([&](const FGsSchemaSpotInfo& souce)->bool {
				return souce.id == warpSpotInfo.sourceSpot.SpotIndex;
				});
			if (nullptr == findSpot)
			{
				continue;
			}
						
			warpInput.DestMapID = warpSpotInfo.destSpot.MapId.GetRow()->mapId;
			warpInput.SpotIndex = warpSpotInfo.destSpot.SpotIndex;

			trs.SetLocation(findSpot->pos);
			trs.SetRotation(FGsDir::AngleToRotator(findSpot->dir).Quaternion());
			warpSpot = FGsTerritoryLevelManager::CreateWarp(warpInput, warpSpotInfo.WarpType, findSpot->radius, warpSpotInfo.shapeId.GetRow()->id, trs, warpSpotInfo.groupId);
			warpSpot->SetWarpIndexForce(findSpot->id, warpSpotInfo.warpId);
			
			index = spotInfoArray.IndexOfByPredicate([&](const FGsSchemaSpotInfo& spot)->bool {
				return spot.id == findSpot->id;
				});

			if (spotInfoArray.IsValidIndex(index))
			{
				spotInfoArray.RemoveAt(index);
			}
		}
	}	

	//normal spot
	{
		FTransform trs;
		AGsTerritoryNormalSpot* newNormalSpot;
		for (const FGsSchemaSpotInfo& spotInfo : spotInfoArray)
		{
			trs.SetLocation(spotInfo.pos);
			trs.SetRotation(FGsDir::AngleToRotator(spotInfo.dir).Quaternion());
			newNormalSpot = CreateNormalSpot(spotInfo.name, spotInfo.radius, trs);
			if (false == newNormalSpot->SetIndexForce(spotInfo.id))
			{
				UE_LOG(LogTerritoryEditor, Error, TEXT("Copy normal spot fail - check territory data - index id : %d"), spotInfo.id);
				continue;
			}
		}
	}	
}

void FGsTerritoryLevelManager::CopyAllFence()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CopyAllFence"));

	const FString& levelId = FString::FromInt(_copySourceLevelId);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();

	//fence data
	const FString& fenceTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(levelId);	
	UGsTable* fenceTable = tableManager.GetTable(FGsSchemaFenceData::StaticStruct(), fenceTableName);
	UGsTableFenceData* castFenceTable = Cast<UGsTableFenceData>(fenceTable);

	//territory data	
	const FString& territoryTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelId);	
	UGsTable* territoryTable = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), territoryTableName);	
	UGsTableMapTerritoryData* castTerritoryTable = Cast<UGsTableMapTerritoryData>(territoryTable);
	if (nullptr == castTerritoryTable || nullptr == castFenceTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* findTerritoryData;
	if (false == castTerritoryTable->FindRow(*levelId, findTerritoryData))
	{
		return;
	}

	const FGsSchemaFenceData* findFenceData;
	if (false == castFenceTable->FindRowById(_copySourceLevelId, findFenceData))
	{
		return;
	}

	//copy fence data	
	FTransform trs;
	const FGsSchemaFenceElement* findFence = nullptr;

	AGsTerritoryFenceBase* fence = nullptr;
	for (const FGsSchemaMapTerritoryInfo& territoryInfo : findTerritoryData->territoryList)
	{
		findFence = findFenceData->FenceList.FindByPredicate([&](const FGsSchemaFenceElement& element)->bool {
			return element.FenceId == territoryInfo.fenceId;
			});
		if (nullptr == findFence)
		{
			continue;
		}

		trs.SetLocation(findFence->vertexList[0]);

		//map territory data
		switch (territoryInfo.territoryType)
		{
		case TerritoryType::TOWN:
		{
			fence = FGsTerritoryLevelManager::CreateFenceTown(trs
				, territoryInfo.name
				, territoryInfo.sound);			

			AGsTerritoryFenceTown* safeZone = Cast<AGsTerritoryFenceTown>(fence);
			if (safeZone)
			{
				safeZone->Import(territoryInfo, *findFence);
			}
		}
			break;
		case TerritoryType::SAFE_ZONE:
		{
			fence = FGsTerritoryLevelManager::CreateFenceSafeZone(trs
				, territoryInfo.name
				, territoryInfo.sound);

			AGsTerritoryFenceSafeZone* safeZone = Cast<AGsTerritoryFenceSafeZone>(fence);
			if(safeZone)
			{
				safeZone->Import(territoryInfo, *findFence);
			}			
		}
			break;		
		}			
	}

	//env
	for (const FGsSchemaMapTerritoryEnv& envTerritory : findTerritoryData->envTerritoryList)
	{
		findFence = findFenceData->FenceList.FindByPredicate([&](const FGsSchemaFenceElement& element)->bool {
			return element.FenceId == envTerritory.fenceId;
			});
		if (nullptr == findFence)
		{
			continue;
		}

		trs.SetLocation(findFence->vertexList[0]);

		fence = FGsTerritoryLevelManager::CreateFenceEnv(trs
			, envTerritory.name
			, envTerritory.envConditionList);

		AGsTerritoryFenceEnv* envFence = Cast<AGsTerritoryFenceEnv>(fence);
		if (envFence)
		{
			envFence->Import(envTerritory, (*findFence));
		}
	}

	//sound
	for (const FGsSchemaMapTerritorySound& soundTerritory : findTerritoryData->soundTerritoryList)
	{
		findFence = findFenceData->FenceList.FindByPredicate([&](const FGsSchemaFenceElement& element)->bool {
			return element.FenceId == soundTerritory.fenceId;
			});
		if (nullptr == findFence)
		{
			continue;
		}

		trs.SetLocation(findFence->vertexList[0]);

		fence = FGsTerritoryLevelManager::CreateFenceSound(trs
			, soundTerritory.name
			, soundTerritory.questSoundList);

		AGsTerritoryFenceSound* soundFence = Cast<AGsTerritoryFenceSound>(fence);
		if (soundFence)
		{
			soundFence->Import(soundTerritory, (*findFence));
		}
	}

	//space crack	
	for (const FGsSchemaMapTerritorySpaceCrack& spaceCrackTerritory : findTerritoryData->spaceCrackZoneList)
	{
		findFence = findFenceData->FenceList.FindByPredicate([&](const FGsSchemaFenceElement& element)->bool {
			return element.FenceId == spaceCrackTerritory.fenceId;
			});
		if (nullptr == findFence)
		{
			continue;
		}

		trs.SetLocation(findFence->vertexList[0]);

		fence = FGsTerritoryLevelManager::CreateFenceSpaceCrack(trs
			, spaceCrackTerritory.name);

		AGsTerritoryFenceSpaceCrack* spaceCrack = Cast<AGsTerritoryFenceSpaceCrack>(fence);
		if (spaceCrack)
		{
			spaceCrack->Import(spaceCrackTerritory, (*findFence));
		}
	}
}

void FGsTerritoryLevelManager::CopyAllLine()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CopyAllSpawnActor"));

	const FString& levelId = FString::FromInt(_copySourceLevelId);
	const FString& tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableLineData>(levelId);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaLineData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	UGsTableLineData* castTable = Cast<UGsTableLineData>(table);
	if (nullptr == castTable)
	{
		return;
	}

	//PatorlLine

	//spacecrack은 prop에 포함된다.
	TArray<const FGsSchemaLineData*> lineDataArray;
	castTable->GetAllRows<FGsSchemaLineData>(lineDataArray);
	if (lineDataArray.Num() == 0)
	{
		return;
	}

	const TArray<FGsSchemaLineElement>& lineElementArray = lineDataArray[0]->LineList;
	int32 num = lineElementArray.Num();
	FTransform trs;
	AGsTerritoryLinePatrol* patrolLine = nullptr;
	for (int32 i = 0; i < num; ++i)
	{
		const FGsSchemaLineElement& lineElement = lineElementArray[i];
		trs.SetLocation(lineElement.Center);
		patrolLine = CreatePatrolLine(trs);
		if (nullptr == patrolLine)
		{
			continue;
		}

		patrolLine->SetTerritoryUniqueIDForce(lineElement.LineId);
		patrolLine->Import(lineElement);
	}
}

int32 FGsTerritoryLevelManager::GetLevelKeyByIndex(int32 inLevelIndex)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::GetLevelKeyByIndex"));

	UGsTableMapData* mapTable = GetCachedMapData();
	if (nullptr == mapTable)
	{		
		return INDEX_NONE;
	}
	
	TArray<const FGsSchemaMapData*> mapList;
	mapTable->GetAllRows(mapList);
	if ((mapList.Num() - 1) < inLevelIndex)
	{		
		return INDEX_NONE;
	}

	int32 levelKey = INDEX_NONE;
	for (int32 i = 0; i < mapList.Num(); ++i)
	{
		if (i == inLevelIndex)
		{
			levelKey = mapList[i]->id;
			break;
		}
	}

	return levelKey;
}

int32 FGsTerritoryLevelManager::GetLevelIndexByKey(int32 inLevelKey)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::GetLevelIndexByKey"));

	UGsTableMapData* mapTable = GetCachedMapData();
	if (nullptr == mapTable)
	{		
		return INDEX_NONE;
	}

	int32 index = INDEX_NONE;
	TArray<const FGsSchemaMapData*> mapList;
	mapTable->GetAllRows(mapList);
	for (int32 i = 0; i < mapList.Num(); ++i)
	{
		if (mapList[i]->id == inLevelKey)
		{
			index = i;
			break;
		}
	}

	return index;
}

const FGsSchemaMapData* FGsTerritoryLevelManager::GetMapDataByIndex(int32 inLevelIndex)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::GetMapDataByIndex"));

	UGsTableMapData* mapTable = GetCachedMapData();
	if (nullptr == mapTable)
	{
		return nullptr;
	}

	int32 index = INDEX_NONE;
	TArray<const FGsSchemaMapData*> mapList;
	mapTable->GetAllRows(mapList);
	if (mapList.IsValidIndex(inLevelIndex))
	{
		return mapList[inLevelIndex];
	}	

	return nullptr;
}

const FGsSchemaAreaData* FGsTerritoryLevelManager::GetAreaDataById(int32 inAreaId)
{
	UGsTableAreaData* areaTable = GetCachedAreaData();
	if (nullptr == areaTable)
	{
		return false;
	}

	const FGsSchemaAreaData* areaData;
	areaTable->FindRowById(inAreaId, areaData);

	return areaData;
}

#undef LOCTEXT_NAMESPACE // "TerritoryEditor"

void FGsTerritoryLevelManager::UnbindAdditionalLevelEvent()
{
	if (_additionalLevelEventHandle.IsValid())
	{
		_additionalLevelEvent.Remove(_additionalLevelEventHandle);
	}	
}

void FGsTerritoryLevelManager::ExcuteAdditionalLevelEvent()
{
	if (_additionalLevelEvent.IsBound())
	{
		_additionalLevelEvent.Broadcast();
	}
}

/// <summary>
/// Copy level 
/// </summary>
/// <param name="inSource"></param>
/// <param name="inDest"></param>
void FGsTerritoryLevelManager::CopyLevel(int32 inSource, int32 inDest
	, bool inCopySpawnActor, bool inCopySpot, bool inCopyFence, bool inCopyLine)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CopyLevel - source level : %d\tdest level : %d"), inSource, inDest);

	_currentLevelId = inDest;
	_copySourceLevelId = inSource;

	_copySpawnActor = inCopySpawnActor;
	_copySpot = inCopySpot;
	_copyFence = inCopyFence;
	_copyLine = inCopyLine;

	//get source map data
	const UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	const UGsTableMapData* castMapTable = GetCachedMapData();	
	if (nullptr == castMapTable)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryLevelManager::CopyLevel - map table is missing"));
		return;
	}
	
	const FGsSchemaMapData* sourceMapData = nullptr;
	const FGsSchemaMapData* destMapData = nullptr;
	if (false == castMapTable->FindRowById(inSource, sourceMapData))
	{
		UE_LOG(LogTemp, Error, TEXT("FGsTerritoryLevelManager::CopyLevel - source level is missing : %d"), inSource);
		return;
	}

	if (false == castMapTable->FindRowById(inDest, destMapData))
	{
		UE_LOG(LogTemp, Error, TEXT("FGsTerritoryLevelManager::CopyLevel - dest level is missing : %d"), inDest);
		return;
	}	

	_isLoadArtLevel = false;
	
	//bind devent
	const FString& areaName = destMapData->areaId.GetRow()->levelName;
	if (_additionalLevelEventHandle.IsValid())
	{
		_additionalLevelEvent.Remove(_additionalLevelEventHandle);
	}
	_additionalLevelEventHandle = _additionalLevelEvent.AddRaw(this, &FGsTerritoryLevelManager::OnCopyLevel);

	//is edtior mode
	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return;
	}

	//change level copy mode
	OpenLevel(areaName);	
}

FString FGsTerritoryLevelManager::ConvertShowTypeString(EGsTerrEdShowType inShowType)
{
	switch (inShowType)
	{
	case EGsTerrEdShowType::HideAll:
		return TEXT("HideAll");
		break;
	case EGsTerrEdShowType::ShowOnlySelect:
		return TEXT("ShowOnlySelectActor");
		break;
	case EGsTerrEdShowType::ShowAll:
		return TEXT("ShowAllActor");
		break;
	}

	return TEXT("ShowAllActor");
}

EGsTerrEdShowType FGsTerritoryLevelManager::ConvertShowTypeEnum(FString inShowType)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ConvertShowTypeEnum"));

	if (inShowType.Equals("HideAll"))
	{
		return EGsTerrEdShowType::HideAll;
	}
	else if (inShowType.Equals("ShowOnlySelectActor"))
	{
		return  EGsTerrEdShowType::ShowOnlySelect;
	}
	else if (inShowType.Equals("ShowAllActor"))
	{
		return EGsTerrEdShowType::ShowAll;
	}

	return EGsTerrEdShowType::ShowAll;
}

//Show widget =========================================================================
void FGsTerritoryLevelManager::ActiveTerritoryWidget(EGsTerrEdShowType inShow)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ActiveTerritoryWidget"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ActiveTerritoryWidgetInternal<AGsTerritoryZone>(world, inShow);
	ActiveTerritoryWidgetInternal<AGsTerritorySpawnActor>(world, inShow);
	ActiveTerritoryWidgetInternal<AGsTerritorySpot>(world, inShow);
	ActiveTerritoryWidgetInternal<AGsTerritoryWarp>(world, inShow);	
	ActiveTerritoryWidgetInternal<AGsTerritoryFenceBase>(world, inShow);	
	ActiveTerritoryWidgetInternal<AGsTerritoryLine>(world, inShow);
}

void FGsTerritoryLevelManager::ShowAllWidget()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ShowAllWidget"));

	_widgetShowType = EGsTerrEdShowType::ShowAll;
	ActiveTerritoryWidget(_widgetShowType);
}

void FGsTerritoryLevelManager::ShowOnlySelectedWidget()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ShowOnlySelectedWidget"));

	_widgetShowType = EGsTerrEdShowType::ShowOnlySelect;
	ActiveTerritoryWidget(_widgetShowType);
}

void FGsTerritoryLevelManager::HideAllWidget()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::HideAllWidget"));

	_widgetShowType = EGsTerrEdShowType::HideAll;
	ActiveTerritoryWidget(_widgetShowType);
}

void FGsTerritoryLevelManager::InvalidScalabilityQualityLow()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::InvalidScalabilityQualityLow"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	if(_isScalabilityQualityLow)
	{
		//GEngine->Exec(world, TEXT("r.ScreenPercentage 50"));
		GEngine->Exec(world, TEXT("r.ViewDistanceScale 0.4"));		
		GEngine->Exec(world, TEXT("sg.PostProcessQuality 0"));		
		GEngine->Exec(world, TEXT("sg.ShadowQuality 0"));
		GEngine->Exec(world, TEXT("sg.TextureQuality 0"));
		GEngine->Exec(world, TEXT("sg.EffectsQuality 0"));
		//GEngine->Exec(world, TEXT("sg.FoliageQuality 0"));
		GEngine->Exec(world, TEXT("r.DetailMode 0"));
		GEngine->Exec(world, TEXT("r.SkeletalMeshLODBias 0"));
	}
	else
	{
		//float sccale = GEngine->GameUserSettings->GetDefaultResolutionScale();
		int32 viewDistanceQualityLevel = GEngine->GameUserSettings->GetViewDistanceQuality();
		int32 postProcessQualityLevel = GEngine->GameUserSettings->GetPostProcessingQuality();
		int32 shadowQualityLevel = GEngine->GameUserSettings->GetShadowQuality();
		int32 textureQualityLevel = GEngine->GameUserSettings->GetTextureQuality();
		int32 effectQualityLevel = GEngine->GameUserSettings->GetVisualEffectQuality();
		int32 foliageQualityLevel = GEngine->GameUserSettings->GetFoliageQuality();
		

		//GEngine->Exec(world, TEXT("r.ScreenPercentage 100"));
		GEngine->Exec(world, *FString::Printf(TEXT("r.ViewDistanceScale %d"), viewDistanceQualityLevel));
		GEngine->Exec(world, *FString::Printf(TEXT("sg.PostProcessQuality %d"), postProcessQualityLevel));
		GEngine->Exec(world, *FString::Printf(TEXT("sg.ShadowQuality %d"), shadowQualityLevel));
		GEngine->Exec(world, *FString::Printf(TEXT("sg.TextureQuality %d"), textureQualityLevel));
		GEngine->Exec(world, *FString::Printf(TEXT("sg.EffectsQuality %d"), effectQualityLevel));
		//GEngine->Exec(world, *FString::Printf(TEXT("sg.FoliageQuality %d"), foliageQualityLevel));
		GEngine->Exec(world, TEXT("r.DetailMode 1"));
		GEngine->Exec(world, TEXT("r.SkeletalMeshLODBias 1"));
	}
}

//Show shape =========================================================================
void FGsTerritoryLevelManager::ShowAllShape()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ShowAllShape"));

	_shapeShowType = EGsTerrEdShowType::ShowAll;
	ActiveTerritoryShape(_shapeShowType);
}

void FGsTerritoryLevelManager::ShowOnlySelectedShape()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ShowOnlySelectedShape"));

	_shapeShowType = EGsTerrEdShowType::ShowOnlySelect;
	ActiveTerritoryShape(_shapeShowType);
}

void FGsTerritoryLevelManager::HideAllShape()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::HideAllShape"));

	_shapeShowType = EGsTerrEdShowType::HideAll;
	ActiveTerritoryShape(_shapeShowType);
}

void FGsTerritoryLevelManager::ActiveTerritoryShape(EGsTerrEdShowType inShow)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ActiveTerritoryShape"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ActiveTerritoryShapeInternal<AGsTerritoryNpc>(world, inShow);
	ActiveTerritoryShapeInternal<AGsTerritoryProp>(world, inShow);	
}

void FGsTerritoryLevelManager::ShowSightRadius()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ShowSightRadius"));

	_sightRadiusShowType = EGsTerrEdShowType::ShowAll;
	ActiveSightRadius(_sightRadiusShowType);
}

void FGsTerritoryLevelManager::SelectOnlySightRadius()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::SelectOnlySightRadius"));

	_sightRadiusShowType = EGsTerrEdShowType::ShowOnlySelect;
	ActiveSightRadius(_sightRadiusShowType);
}

void FGsTerritoryLevelManager::HideAllSightRadius()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::HideAllSightRadius"));

	_sightRadiusShowType = EGsTerrEdShowType::HideAll;
	ActiveSightRadius(_sightRadiusShowType);
}

void FGsTerritoryLevelManager::ActiveSightRadius(EGsTerrEdShowType inShow)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ActiveSightRadius"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	TArray<AGsTerritoryNpc*> selectedTerritoryActors;
	TArray<AActor*> territoryActors;
	if (USelection* selection = GEditor->GetSelectedActors())
	{
		selection->GetSelectedObjects<AGsTerritoryNpc>(selectedTerritoryActors);
	}

	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryNpc::StaticClass(), territoryActors);
	FVector location;

	for (AActor* temp : territoryActors)
	{
		if (temp)
		{
			if (AGsTerritoryNpc* territoryActor = Cast<AGsTerritoryNpc>(temp))
			{
				if (inShow == EGsTerrEdShowType::ShowAll)
				{
					territoryActor->SetSightRidusVisible(true);
				}
				else if (inShow == EGsTerrEdShowType::ShowOnlySelect)
				{
					if (selectedTerritoryActors.Contains(temp))
					{
						territoryActor->SetSightRidusVisible(true);
					}
					else
					{
						territoryActor->SetSightRidusVisible(false);
					}
				}
				else
				{
					territoryActor->SetSightRidusVisible(false);
				}
			}
		}
	}
}

bool FGsTerritoryLevelManager::IsNavmeshExist(UWorld* inWorld)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::IsNavmeshExist"));

	if (inWorld && inWorld->GetCurrentLevel())
	{
		const FString& name = UGameplayStatics::GetCurrentLevelName(inWorld);

		ANavigationData* navmeshData = FNavigationSystem::GetCurrent<UNavigationSystemV1>(inWorld)->GetDefaultNavDataInstance();
		//ANavigationData* navmeshData = inWorld->GetNavigationSystem()->GetMainNavData(FNavigationSystem::DontCreate);
		if (nullptr == navmeshData)
		{
			const FString& log = FString::Printf(TEXT("No navmesh exist - name : %s"), *name);
			FGsTerritoryEditorUtil::ShowWarningNotification(log, 60);
			return false;
		}
	}

	return true;
}

UClass* FGsTerritoryLevelManager::GetZoneTemplate(const EGsTerritoryZoneShapeType inShape)
{
	UClass* zoneClass = nullptr;
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		switch (inShape)
		{
		case EGsTerritoryZoneShapeType::Circle:
		{
			if (settings->_circleZoneClass)
			{
				zoneClass = settings->_circleZoneClass.Get();
			}

			break;
		}
		case EGsTerritoryZoneShapeType::Line:
		{
			if (settings->_lineZoneClass)
			{
				zoneClass = settings->_lineZoneClass.Get();
			}

			break;
		}
		case EGsTerritoryZoneShapeType::Polygon:
		{
			if (settings->_polygonZoneClass)
			{
				zoneClass = settings->_polygonZoneClass.Get();
			}

			break;
		}
		default:
			break;
		}
	}

	return zoneClass;
}

AGsTerritoryZone* FGsTerritoryLevelManager::CreateZone(const NpcSpawnGroupType inSpawnGroup, const bool inByClient, const bool inByDefualt, const EGsTerritoryZoneShapeType inShape, const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateZone"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{		
		return nullptr;
	}

	if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(world->GetCurrentLevel(), EGsTerritoryObjType::Zone))
	{
		return nullptr;
	}

	if (UClass* zoneClass = GetZoneTemplate(inShape))
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* actor = world->SpawnActor(zoneClass, &inTrs, param);		
		if (nullptr == actor)
		{			
			return nullptr;
		}

		FString invalidReason;
		if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
		{
			zone->_spawnGroupType = inSpawnGroup;
			zone->_byClient = inByClient;
			zone->_byDefault = inByDefualt;

			instance->GetUniqueIDManager()->SetUniqueID(zone);	
						
			//SetActorLabel을 하면 ChildActor가 새로 생긴다. Shape을 다시 그려야한다. 
			zone->InvalidShape();

			return zone;
		}
	}
	
	return nullptr;
}

AGsTerritoryNormalSpot* FGsTerritoryLevelManager::CreateNormalSpot(const FText& inName, const float inRandomSpawnRange, const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateNormalSpot"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	if (settings->_normalSpotClass)
	{			
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_normalSpotClass, &inTrs, param))
		{
			if (AGsTerritoryNormalSpot* spot = Cast<AGsTerritoryNormalSpot>(actor))
			{
				spot->_radius = inRandomSpawnRange;
				spot->_name = inName;
				spot->InvalidShape();				

				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(spot);

				return spot;
			}
		}
	}
	
	return nullptr;
}

AGsTerritorySpawnSpot* FGsTerritoryLevelManager::CreateSpawnSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateSpawnSpot"));

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	return Cast<AGsTerritorySpawnSpot>(CreateTerritoryGameSpot(inName, inTeamType, inRandomSpawnRange, settings->_spawnSpotClass, inTrs));
}

AGsTerritoryResurrectionSpot* FGsTerritoryLevelManager::CreateResurrectionSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateResurrectionSpot"));

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	return Cast<AGsTerritoryResurrectionSpot>(CreateTerritoryGameSpot(inName, inTeamType, inRandomSpawnRange, settings->_ressurectionSpotClass, inTrs));
}

AGsTerritoryTeleportSpot* FGsTerritoryLevelManager::CreateTeleportSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateTeleportSpot"));

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	return Cast<AGsTerritoryTeleportSpot>(CreateTerritoryGameSpot(inName, inTeamType, inRandomSpawnRange, settings->_teleportSpotClass, inTrs));
}

AGsTerritorySpaceCrackPortalSpot* FGsTerritoryLevelManager::CreateSpaceCrackPortalSpot(const FText& inName, const float inRandomSpawnRange, const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateSpaceCrackPortalSpot"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	if (settings->_spaceCrackPortalClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_spaceCrackPortalClass, &inTrs, param))
		{
			if (AGsTerritorySpaceCrackPortalSpot* spot = Cast<AGsTerritorySpaceCrackPortalSpot>(actor))
			{
				spot->_radius = inRandomSpawnRange;
				spot->InvalidShape();

				return spot;
			}
		}
	}

	return nullptr;
}

AGsTerritoryGameSpot* FGsTerritoryLevelManager::CreateTerritoryGameSpot(const FText& inName, const IffTeamType inTeamType, const float inRandomSpawnRange, UClass* inClass, const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateTerritoryGameSpot"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	if (inClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(inClass, &inTrs, param))
		{
			if (AGsTerritoryGameSpot* spot = Cast<AGsTerritoryGameSpot>(actor))
			{
				spot->_teamType = inTeamType;
				spot->_radius = inRandomSpawnRange;
				spot->_name = inName;

				spot->InvalidShape();
				spot->InvalidWidgetText();

				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(spot);

				return spot;
			}
		}
	}

	return nullptr;
}

AGsTerritoryWarpSpot* FGsTerritoryLevelManager::CreateWarp(const FGsTerritoryMapWarp& inInput, const WarpType inWarpType, const float inRadius, const int32 inShapeID, const FTransform& inTrs, int32 inGroupId)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateWarp"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryWarpSpot* newWarpSpot = nullptr;
	AGsTerritoryWarp* newWarp = nullptr;
	if (settings->_warpSpotClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_warpSpotClass, &inTrs, param))
		{
			if (AGsTerritoryWarpSpot* spot = Cast<AGsTerritoryWarpSpot>(actor))
			{
				spot->_radius = inRadius;
				spot->InvalidShape();				
				newWarpSpot = spot;
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(spot);
			}
		}
	}

	if (settings->_warpClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_warpClass, &inTrs, param))
		{
			if (AGsTerritoryWarp* warp = Cast<AGsTerritoryWarp>(actor))
			{
				warp->_shapeID = inShapeID;
				warp->_warpType = inWarpType;
				//sourceMapID, SourceSpotIndex는 SetWarp시 내부에서 설정한다
				/*warp->_sourceMapID = FCString::Atoi(*levelKey);
				warp->_sourceSpotIndex = newWarpSpot->_index;*/
				warp->_destMapID = inInput.DestMapID;
				warp->_destSpotIndex = inInput.SpotIndex;			
				warp->_groupId = inGroupId;

				newWarp = warp;
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newWarp);
			}
		}
	}

	if (newWarpSpot)
	{
		newWarpSpot->SetWarp(newWarp);			
	}

	return newWarpSpot;
}

TArray<AGsTerritorySpawnActor*> FGsTerritoryLevelManager::CreateSpawnActor(const FGsTerritorySpawnInfo & inSpawn, int32 inSpawnCount, EGsTerritorySpawnShapeType inShape, AGsTerritoryZone* inZone, FTransform& inTrans)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateSpawnActor"));

	TArray<AGsTerritorySpawnActor*> spawnActorArray;

	UWorld* world = GEditor->GetEditorWorldContext().World();
	FVector tempLocatin;
	float zoneRadius = inZone->_radius;
	FVector location = inTrans.GetLocation();
	float radius = zoneRadius - (inZone->GetActorLocation() - location).Size();

	const FGsTerritorySpawnInfo& spawnData = inSpawn;
	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{		
		return spawnActorArray;
	}
	
	AGsTerritorySpawnActor* spawnActor = nullptr;
	EGsTerritoryObjType obejctType = obejctType = FGsTerritoryLevelManager::ConvertSpawnTypeToObjectType(inSpawn.Type);
	float dist;
	float angle;
	FString invalidReason;
	for (int i = 0; i < inSpawnCount; ++i)
	{			
		if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(world->GetCurrentLevel(), obejctType))
		{			
			continue;
		}

		if (0 == i)
		{
			spawnActor = inZone->AddTerritorySpawnActor(spawnData, location);
			if (spawnActor)
			{
				spawnActorArray.Add(spawnActor);
				spawnActor->SetActorRotation(inTrans.Rotator());

				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(spawnActor);
			}						
		}
		else
		{
			dist = FMath::RandRange(0.0f, radius);
			angle = FMath::RandRange(0, 360);
			const FVector& temp = (FVector::ForwardVector.RotateAngleAxis(angle, FVector::UpVector) * dist);
			location = location + temp;

			spawnActor = inZone->AddTerritorySpawnActor(spawnData, location);			
			if (spawnActor)
			{				
				spawnActorArray.Add(spawnActor);

				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(spawnActor);
			}
		}
	}

	return spawnActorArray;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceTown(const FTransform& inTrs, const FText& inName /*= FText() */, const FGsSchemaSoundResDataRow& inSound /*= FGsSchemaSoundResDataRow()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;	
	if (settings->_fenceTownClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceTownClass, &inTrs, param))
		{
			if (AGsTerritoryFenceTown* fence = Cast<AGsTerritoryFenceTown>(actor))
			{
				fence->InitTownFence(inName, inSound);
				newFence = fence;
			}
		}
	}
	
	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceSafeZone(const FTransform& inTrs, const FText& inName /*= FText() */, const FGsSchemaSoundResDataRow& inSound /*= FGsSchemaSoundResDataRow()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceSafeZoneClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceSafeZoneClass, &inTrs, param))
		{
			if (AGsTerritoryFenceSafeZone* fence = Cast<AGsTerritoryFenceSafeZone>(actor))
			{
				fence->InitSafeZoneFence(inName, inSound);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceTownChaos(const FTransform& inTrs, const FText& inName /*= FText() */, const FGsSchemaSoundResDataRow& inSound /*= FGsSchemaSoundResDataRow()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceTownClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceTownChaosClass, &inTrs, param))
		{
			if (AGsTerritoryFenceTownChaos* fence = Cast<AGsTerritoryFenceTownChaos>(actor))
			{
				fence->InitTownFence(inName, inSound);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceArena(const FTransform& inTrs
	, const FText& inName
	, const FGsSchemaSoundResDataRow& inSound)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceArenaClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceArenaClass, &inTrs, param))
		{
			if (AGsTerritoryFenceArena* fence = Cast<AGsTerritoryFenceArena>(actor))
			{
				fence->InitArenaFence(inName, inSound);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceArenaReady(const FTransform& inTrs
	, const FText& inName
	, const FGsSchemaSoundResDataRow& inSound)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceSafeZoneClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceArenaReadyClass, &inTrs, param))
		{
			if (AGsTerritoryFenceArenaReady* fence = Cast<AGsTerritoryFenceArenaReady>(actor))
			{
				fence->InitArenaReadyFence(inName, inSound);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceArrival(const FTransform& inTrs, const FText& inName /*= FText() */, const FGsSchemaSoundResDataRow& inSound /*= FGsSchemaSoundResDataRow()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceSafeZoneClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceArrivalClass, &inTrs, param))
		{
			if (AGsTerritoryFenceArrival* fence = Cast<AGsTerritoryFenceArrival>(actor))
			{
				fence->InitArrivalFence(inName, inSound);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceEnv(const FTransform& inTrs, const FText& inName /*= FText() */, const TArray<FGsEnvCondition>& inEnvConditionArray /*= TArray<FGsEnvCondition>()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceEnvClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceEnvClass, &inTrs, param))
		{
			if (AGsTerritoryFenceEnv* fence = Cast<AGsTerritoryFenceEnv>(actor))
			{
				fence->InitFenceEnv(inName, inEnvConditionArray);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceSound(const FTransform& inTrs, const FText& inName /*= FText() */, const TArray<FGsQuestSound>& inQuestSoundArray /*= TArray<FGsQuestSound>()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceSoundClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceSoundClass, &inTrs, param))
		{
			if (AGsTerritoryFenceSound* fence = Cast<AGsTerritoryFenceSound>(actor))
			{
				fence->InitFenceSound(inName, inQuestSoundArray);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceSpaceCrack(const FTransform& inTrs, const FText& inName /*= FText()*/, EGsEnvEvent inEnvEvent)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceSpaceCrackClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceSpaceCrackClass, &inTrs, param))
		{
			if (AGsTerritoryFenceSpaceCrack* fence = Cast<AGsTerritoryFenceSpaceCrack>(actor))
			{
				fence->InitSpaceCrack(inName, inEnvEvent);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateNoCampZone(const FTransform& inTrs, const FText& inName /*= FText()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceNoCampZoneClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceNoCampZoneClass, &inTrs, param))
		{
			if (AGsTerritoryFenceNoCampZone* fence = Cast<AGsTerritoryFenceNoCampZone>(actor))
			{
				fence->SetName(inName);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

class AGsTerritoryFenceBase* FGsTerritoryLevelManager::CreateFenceNonPK(const FTransform& inTrs, const FText& inName /*= FText()*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	AGsTerritoryFenceBase* newFence = nullptr;
	if (settings->_fenceNonPKClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = world->SpawnActor(settings->_fenceNonPKClass, &inTrs, param))
		{
			if (AGsTerritoryFenceNonPK* fence = Cast<AGsTerritoryFenceNonPK>(actor))
			{
				fence->SetName(inName);
				newFence = fence;
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(newFence);

	return newFence;
}

AGsTerritoryLinePatrol* FGsTerritoryLevelManager::CreatePatrolLine(const FTransform& inTrs)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::CreateTerritoryGameSpot"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	UClass* templateClass = settings->_patrolLineClass;
	if (nullptr == templateClass)
	{
		return nullptr;
	}
	
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* actor = world->SpawnActor(templateClass, &inTrs, param);
	if (nullptr == actor)
	{
		return nullptr;
	}
	
	AGsTerritoryLinePatrol* line = Cast<AGsTerritoryLinePatrol>(actor);
	if (nullptr == line)
	{
		return nullptr;
	}	

	line->InvalidShape();
	line->InvalidWidgetText();

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(line);

	return line;
}

EGsTerritorySpawnType FGsTerritoryLevelManager::ConvertObjectTypeToSpawnType(EGsTerritoryObjType inObjType)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::ConvertObjectTypeToSpawnType"));

	EGsTerritorySpawnType spawnType = EGsTerritorySpawnType::Npc;
	switch (inObjType)
	{
	case EGsTerritoryObjType::Npc:
		spawnType = EGsTerritorySpawnType::Npc;
		break;
	case EGsTerritoryObjType::Prop:
		spawnType = EGsTerritorySpawnType::Prop;
		break;
	default:
		break;
	}

	return spawnType;
}

EGsTerritorySpawnType FGsTerritoryLevelManager::ConvertCreatureTypeToSpawnType(CreatureSpawnType inCreatureType)
{
	EGsTerritorySpawnType spawnType = EGsTerritorySpawnType::Npc;
	switch (inCreatureType)
	{
	case CreatureSpawnType::NPC:
		spawnType = EGsTerritorySpawnType::Npc;
		break;
	case CreatureSpawnType::PROP:
		spawnType = EGsTerritorySpawnType::Prop;
		break;
	default:
		break;
	}

	return spawnType;
}

EGsTerritoryObjType FGsTerritoryLevelManager::ConvertSpawnTypeToObjectType(EGsTerritorySpawnType inSpawnType)
{
	EGsTerritoryObjType spawnType = EGsTerritoryObjType::Npc;
	switch (inSpawnType)
	{
	case EGsTerritorySpawnType::Npc:
		spawnType = EGsTerritoryObjType::Npc;
		break;
	case EGsTerritorySpawnType::Prop:
		spawnType = EGsTerritoryObjType::Prop;
		break;
	default:
		break;
	}

	return spawnType;
}

UGsTableNpcData* FGsTerritoryLevelManager::GetCachedNpcData()
{
	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
	return Cast<UGsTableNpcData>(mgr.GetTable(FGsSchemaNpcData::StaticStruct()));

	/*_cachedNpcData = Cast<UGsTableNpcData>(mgr.GetTable(FGsSchemaNpcData::StaticStruct()));
	
	return _cachedNpcData;*/
}

UGsTableShapeData* FGsTerritoryLevelManager::GetCachedShapeData()
{
	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
	_cachedShapeData = Cast<UGsTableShapeData>(mgr.GetTable(FGsSchemaShapeData::StaticStruct()));
	
	return _cachedShapeData;
}

UGsTablePropData* FGsTerritoryLevelManager::GetCachedPropData()
{
	if (nullptr == _cachedPropData)
	{
		UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
		_cachedPropData = Cast<UGsTablePropData>(mgr.GetTable(FGsSchemaPropData::StaticStruct()));
	}

	return _cachedPropData;
}

UGsTableMapData* FGsTerritoryLevelManager::GetCachedMapData()
{
	if (nullptr == _cachedMapData)
	{
		UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
		_cachedMapData = Cast<UGsTableMapData>(mgr.GetTable(FGsSchemaMapData::StaticStruct()));
	}

	return _cachedMapData;
}

UGsTableQuest* FGsTerritoryLevelManager::GetCachedQuestData()
{
	if (nullptr == _cachedQuestData)
	{
		UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
		_cachedQuestData = Cast<UGsTableQuest>(mgr.GetTable(FGsSchemaQuest::StaticStruct()));
	}

	return _cachedQuestData;
}

class UGsTableAreaData* FGsTerritoryLevelManager::GetCachedAreaData()
{
	if (nullptr == _cachedAreaData)
	{
		UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
		_cachedAreaData = Cast<UGsTableAreaData>(mgr.GetTable(FGsSchemaAreaData::StaticStruct()));
	}

	return _cachedAreaData;
}

void FGsTerritoryLevelManager::InvalidTransparentFloor()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::InvalidTransparentFloor"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	TArray<AActor*> zoneArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryZone::StaticClass(), zoneArray);

	TArray<AActor*> spotArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritorySpot::StaticClass(), spotArray);

	AGsTerritoryZone* zone = nullptr;
	for (AActor* actor : zoneArray)
	{
		if (actor)
		{
			zone = Cast<AGsTerritoryZone>(actor);
			if (zone)
			{
				zone->TransparentFloor();
			}
		}
	}

	AGsTerritorySpot* spot = nullptr;
	for (AActor* actor : spotArray)
	{
		if (actor)
		{
			spot = Cast<AGsTerritorySpot>(actor);
			if (spot)
			{
				spot->TransparentFloor();
			}
		}
	}
}

AGsTerritoryActor* FGsTerritoryLevelManager::FindTerritoryActor(int64 inUniqueId)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	TArray<AActor*> actorArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryActor::StaticClass(), actorArray);

	AActor** findActor = actorArray.FindByPredicate([=](AActor* actor)->bool {
		if (AGsTerritoryActor* castActor = Cast<AGsTerritoryActor>(actor))
		{
			return inUniqueId == castActor->GetTerritoryUniqueID();
		}

		return false;
		});

	if (findActor)
	{
		return Cast<AGsTerritoryActor>((*findActor));
	}	

	return nullptr;
}

void FGsTerritoryLevelManager::OnFinishAddZoneTest()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnFinishAddZoneTest"));

	if (_additionalLevelEventHandle.IsValid())
	{
		_additionalLevelEvent.Remove(_additionalLevelEventHandle);
	}
}
//
//void FGsTerritoryLevelManager::OnAddZoneTest()
//{
//	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnAddZoneTest"));
//
//	UWorld* world = GEditor->GetEditorWorldContext().World();
//	if (nullptr == world)
//	{
//		return;
//	}
//
//	if (nullptr == world->GetCurrentLevel())
//	{
//		return;
//	}
//
//	const FString& path = world->GetCurrentLevel()->GetPathName();
//	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
//	{
//		FGsTerritoryEditorUtil::OpenWarningDialog("TERRITORY_EDIOTR_TITLE", "ONLY_SEVER_LEVEL_OK");
//		return;
//	}	
//
//	FTerritoryEditor * instance = FTerritoryEditor::GetInstance();
//	if (nullptr == instance)
//	{
//		FGsTerritoryEditorUtil::OpenWarningDialog("TERRITORY_EDIOTR_TITLE", "INSTANCE_MISSING");
//		return;
//	}
//
//	if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(world->GetCurrentLevel(), EGsTerritoryObjType::Zone))
//	{
//		FGsTerritoryEditorUtil::OpenWarningDialog("TERRITORY_EDIOTR_TITLE", "ZONE_UNIQUE_ID_FULL");
//		return;
//	}
//
//	if (UClass* zoneClass = UGsTerritoryGroundActionUtility::GetZoneTemplate(EGsTerritoryZoneShapeType::Circle))
//	{
//		FTransform trs;
//		trs.SetLocation(GEditor->ClickLocation);
//
//		FActorSpawnParameters param;
//		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		AActor * actor = world->SpawnActor(zoneClass, &trs, param);
//		if (nullptr == actor)
//		{
//			FGsTerritoryEditorUtil::OpenWarningDialog("TERRITORY_EDIOTR_TITLE", "CREATE_FAIL_ZONE");
//			return;
//		}
//
//		if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
//		{
//			zone->_byClient = false;
//			zone->_byDefault = true;
//
//			instance->GetUniqueIDManager()->SetUniqueID(zone);
//			zone->InvalidShape();
//		}
//	}
//}
//
//void FGsTerritoryLevelManager::OnConvertFenceToFenceBase()
//{
//	//이전 펜스 삭제
//	DeleteAllFence();
//
//	FString levelKeyString = FString::FromInt(_currentLevelId);
//
//	//FenceBase를 Base로한 펜스 생성 
//	const FString& mapTerritoryDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelKeyString);
//	const FString& fenceDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(levelKeyString);
//
//	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
//	UGsTable* mapTerritoryDataTable = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), mapTerritoryDataTableName);
//	UGsTable* fenceDataTable = tableManager.GetTable(FGsSchemaFenceData::StaticStruct(), fenceDataTableName);
//	if (nullptr == mapTerritoryDataTable
//		|| nullptr == fenceDataTable)
//	{
//		return;
//	}
//
//	const FGsSchemaMapTerritoryData* findMapTerritoryData = nullptr;
//	mapTerritoryDataTable->FindRow<FGsSchemaMapTerritoryData>(*levelKeyString, findMapTerritoryData);
//
//	const FGsSchemaFenceData* findLineData = nullptr;
//	fenceDataTable->FindRow<FGsSchemaFenceData>(*levelKeyString, findLineData);
//	
//	for (const FGsSchemaMapTerritoryInfo& territoryInfo : findMapTerritoryData->territoryList)
//	{
//		const FGsSchemaFenceElement* findLine = findLineData->FenceList.FindByPredicate([&](const FGsSchemaFenceElement& fence)
//			{
//				return fence.FenceId == territoryInfo.fenceId;
//			});
//		
//		switch (territoryInfo.territoryType)
//		{
//		case TerritoryType::TOWN:
//		{
//			FTransform trs;
//			trs.SetLocation(findLine->vertexList[0]);			
//			AGsTerritoryFenceBase* fence = CreateFenceTown(trs, territoryInfo.name, territoryInfo.sound);
//			AGsTerritoryFenceTown* townFence = Cast<AGsTerritoryFenceTown>(fence);
//			if (townFence)
//			{
//				townFence->Import(territoryInfo, *findLine);
//			}
//		}
//			break;
//		case TerritoryType::SAFE_ZONE:
//		{
//			FTransform trs;
//			trs.SetLocation(findLine->vertexList[0]);
//			AGsTerritoryFenceBase* fence = CreateFenceSafeZone(trs, territoryInfo.name, territoryInfo.sound);
//			AGsTerritoryFenceSafeZone* townSafeZone = Cast<AGsTerritoryFenceSafeZone>(fence);
//			if (townSafeZone)
//			{
//				townSafeZone->Import(territoryInfo, *findLine);
//			}
//		}
//			break;
//		case TerritoryType::ARENA:
//		{
//			FTransform trs;
//			trs.SetLocation(findLine->vertexList[0]);
//			AGsTerritoryFenceBase* fence = CreateFenceArena(trs, territoryInfo.name, territoryInfo.sound);
//			AGsTerritoryFenceArena* arenaFence = Cast<AGsTerritoryFenceArena>(fence);
//			if (arenaFence)
//			{
//				arenaFence->Import(territoryInfo, *findLine);
//			}
//		}
//		break;
//		case TerritoryType::ARENA_READY:
//		{
//			FTransform trs;
//			trs.SetLocation(findLine->vertexList[0]);
//			AGsTerritoryFenceBase* fence = CreateFenceArenaReady(trs, territoryInfo.name, territoryInfo.sound);
//			AGsTerritoryFenceArenaReady* arenaReadyFence = Cast<AGsTerritoryFenceArenaReady>(fence);
//			if (arenaReadyFence)
//			{
//				arenaReadyFence->Import(territoryInfo, *findLine);
//			}
//		}
//		break;
//		case TerritoryType::ARRIVAL:
//		{
//			FTransform trs;
//			trs.SetLocation(findLine->vertexList[0]);
//			AGsTerritoryFenceBase* fence = CreateFenceArrival(trs, territoryInfo.name, territoryInfo.sound);
//			AGsTerritoryFenceArrival* arrivalFence = Cast<AGsTerritoryFenceArrival>(fence);
//			if (arrivalFence)
//			{
//				arrivalFence->Import(territoryInfo, *findLine);
//			}
//		}
//		break;
//		case TerritoryType::Env:
//		{
//			FTransform trs;
//			trs.SetLocation(findLine->vertexList[0]);
//			AGsTerritoryFenceBase* fence = CreateFenceEnv(trs, territoryInfo.name, territoryInfo.envConditionArray);
//			AGsTerritoryFenceEnv* townEnv = Cast<AGsTerritoryFenceEnv>(fence);
//			if (townEnv)
//			{
//				townEnv->Import(territoryInfo, *findLine);
//			}
//		}
//		break;
//		case TerritoryType::SOUND:
//		{
//			FTransform trs;
//			trs.SetLocation(findLine->vertexList[0]);
//			AGsTerritoryFenceBase* fence = CreateFenceSound(trs, territoryInfo.name, territoryInfo.questSoundArray);
//			AGsTerritoryFenceSound* townSound = Cast<AGsTerritoryFenceSound>(fence);
//			if (townSound)
//			{
//				townSound->Import(territoryInfo, *findLine);
//			}
//		}
//		break;
//		}
//	}
//	
//	//저장	
//
//}