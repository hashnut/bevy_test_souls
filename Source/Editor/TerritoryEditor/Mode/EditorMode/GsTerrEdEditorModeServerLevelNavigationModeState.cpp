#include "GsTerrEdEditorModeServerLevelNavigationModeState.h"
#include "Mode/GsTerrEdModeDefine.h"
#include "ServerLevelNavigationMode/GsTerrEdServerLevelNavigationMapChangeModeState.h"
#include "ServerLevelNavigationMode/GsTerrEdServerLevelNavigationMapOpenModeState.h"
#include "ServerLevelNavigationMode/GsTerrEdServerLevelNavigationNewCurrentLevelModeState.h"
#include "ServerLevelNavigationMode/GsTerrEdServerLevelNavigationMapIdleModeState.h"
#include "AssetRegistryModule.h"
#include "Developer/AssetTools/Public/IAssetTypeActions.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "WorldBrowser/Public/WorldBrowserModule.h"
#include "Internationalization.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "ServerLevelNavigationMode/GsTerrEdServerLevelNavigationPostSaveModeState.h"

#define LOCTEXT_NAMESPACE	"TerritoryEditor"

FGsTerrEdEditorModeServerLevelNavigationModeState::FGsTerrEdEditorModeServerLevelNavigationModeState()
{
	//맵 대기
	FGsTerrEdServerLevelNavigationMapIdleModeState* mapIdleMode = new FGsTerrEdServerLevelNavigationMapIdleModeState();
	mapIdleMode->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_MAP_IDLE_MODE, MakeShareable(mapIdleMode));

	//맵 로드
	FGsTerrEdServerLevelNavigationMapChangeModeState* mapChangeMode = new FGsTerrEdServerLevelNavigationMapChangeModeState();
	mapChangeMode->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE, MakeShareable(mapChangeMode));

	//맵 오픈 
	FGsTerrEdServerLevelNavigationMapOpenModeState* mapOpenMode = new FGsTerrEdServerLevelNavigationMapOpenModeState();
	mapOpenMode->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE, MakeShareable(mapOpenMode));

	//서버 레벨 설정
	FGsTerrEdServerLevelNavigationNewCurrentLevelModeState* newCurrentLevel = new FGsTerrEdServerLevelNavigationNewCurrentLevelModeState();
	newCurrentLevel->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE, MakeShareable(newCurrentLevel));

	//저장 후 호출
	FGsTerrEdServerLevelNavigationPostSaveModeState* postSaveLevel = new FGsTerrEdServerLevelNavigationPostSaveModeState();
	postSaveLevel->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_POST_SAVE_MODE, MakeShareable(postSaveLevel));
}

FGsTerrEdEditorModeServerLevelNavigationModeState::~FGsTerrEdEditorModeServerLevelNavigationModeState()
{

}

void FGsTerrEdEditorModeServerLevelNavigationModeState::Enter()
{
	_currentServerLevelId = 0;
	_currentServerLevelName = "";

	UGsTableMapData* mapTable = FGsTerritoryLevelManager::GetCachedMapData();
	mapTable->GetAllRows(_mapDataArray);

	_mapDataNum = _mapDataArray.Num();
	_currentServerLevelIndex = 0;

	//맵이 로드 시작 
	if (_mapChangedHandle.IsValid())
	{
		FEditorDelegates::MapChange.Remove(_mapChangedHandle);
	}
	_mapChangedHandle = FEditorDelegates::MapChange.AddRaw(this, &FGsTerrEdEditorModeServerLevelNavigationModeState::OnMapChange);

	//맵이 오픈 끝, 서버 스트리밍 레벨 로드
	if (_mapOpenedHandle.IsValid())
	{
		FEditorDelegates::OnMapOpened.Remove(_mapOpenedHandle);
	}
	_mapOpenedHandle = FEditorDelegates::OnMapOpened.AddRaw(this, &FGsTerrEdEditorModeServerLevelNavigationModeState::OnMapOpend);

	//서버 레벨 로드, 서버 레벨 선택, 커스텀 이벤트 실행, 서버 레벨 저장
	if (_newLevelLoadedHandle.IsValid())
	{
		FEditorDelegates::NewCurrentLevel.Remove(_newLevelLoadedHandle);
	}
	_newLevelLoadedHandle = FEditorDelegates::NewCurrentLevel.AddRaw(this, &FGsTerrEdEditorModeServerLevelNavigationModeState::OnNewCurrentLevel);

	//서버 레벨 저장 후 호출
	if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}
	_postSaveHandle = FEditorDelegates::PostSaveWorld.AddRaw(this, &FGsTerrEdEditorModeServerLevelNavigationModeState::OnPostSaveLevel);
	
	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (instance)
	{
		if (FTerritoryEditor::_isActive)
		{
			instance->_levelEvent.UnbindNpcTable();
			instance->_levelEvent.UnbindTerritoryActorTransform();
		}

		//instance->_levelManager->LoadArtLevelsInEditorTime();
		instance->GetUniqueIDManager()->Clear();
		instance->UnbindMapOpen();
		instance->UnbindMapChange();
		instance->UnbindNewCurrentLevel();
		instance->UnbindPostSave();
		instance->UnbindActorAdded();
		instance->UnbindActorDeleted();
	}

	_additionalEvent.Clear();
	_additionalEvent.AddRaw(this, &FGsTerrEdEditorModeServerLevelNavigationModeState::OnTest);

	SetTickEnable(true);

	ChangeState(TERR_ED_EDITOR_MODE_MAP_IDLE_MODE);
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::Exit()
{
	FGsTerrEdModeStateBase* currentState = GetCurrentState();
	if (currentState)
	{
		currentState->Exit();
	}

	if (_mapOpenedHandle.IsValid())
	{
		FEditorDelegates::OnMapOpened.Remove(_mapOpenedHandle);
		_mapOpenedHandle.Reset();
	}

	if (_mapChangedHandle.IsValid())
	{
		FEditorDelegates::MapChange.Remove(_mapChangedHandle);
		_mapChangedHandle.Reset();
	}

	if (_newLevelLoadedHandle.IsValid())
	{
		FEditorDelegates::NewCurrentLevel.Remove(_newLevelLoadedHandle);
	}

	if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}

	_additionalEvent.Clear();

	_currentState.Reset();
	_currentStateName = TEXT("");

	_loadLevelList.Empty();
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::Tick(float inTick)
{
	if (false == _isTick)
	{
		return;
	}

	if (_currentState.IsValid())
	{
		_currentState.Pin()->Tick(inTick);
	}
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::MapChange()
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE);
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::MapOpened(const FString& inFilename)
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE);
}

//Open server level
void FGsTerrEdEditorModeServerLevelNavigationModeState::NewCurrentLevel()
{
	ChangeState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE);
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::PostSave()
{
	ChangeState(TERR_ED_EDITOR_MODE_POST_SAVE_MODE);
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::FinishNavigationServerLevel()
{
	FText okTitleText = FText::FromString("Finish save all server level");
	FText okMessageText = FText::FromString("Finish save all server level");
	FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, okTitleText, &okMessageText);

	_currentServerLevelId = 0;
	_currentServerLevelName = "";

	if (_additionalEventHandle.IsValid())
	{
		_additionalEvent.Remove(_additionalEventHandle);
	}

	FTerritoryEditor::GetInstance()->ChangeLevelEditMode();
	FTerritoryEditor::GetInstance()->ChangeLevelEdNewCurrentLevelMode();

	SetTickEnable(false);
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::FinishNavigationCurrentServerLevel()
{
	_currentServerLevelIndex = ++_currentServerLevelIndex;
	ChangeState(TERR_ED_EDITOR_MODE_MAP_IDLE_MODE);
}

/// <summary>
/// 서버 레벨 열기 
/// </summary>
/// <param name="inLevelName"></param>
/// <returns></returns>
bool FGsTerrEdEditorModeServerLevelNavigationModeState::OpenLevel(const FString& inLevelName)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeServerLevelNavigationModeState::OpenLevel - Start"));

	const FString& PackageName = TEXT("/Game/Maps/") + inLevelName + TEXT("/") + inLevelName;
	TArray<FAssetData> ActivatedAssets;
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	if (!AssetRegistry.GetAssetsByPackageName(*PackageName, ActivatedAssets))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("!AssetRegistry.GetAssetsByPackageName(%s, Assets)"), *PackageName);

		FinishNavigationServerLevel();
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
void FGsTerrEdEditorModeServerLevelNavigationModeState::LoadServerStreamingLevel()
{	
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeServerLevelNavigationModeState::OnOpenLevelForSaveAllMap - OnOpenLevel - level index : %d\tlevel key : %d"), _currentServerLevelIndex, _currentServerLevelId);

	//if (_mapOpenedHandle.IsValid())
	//{
	//	FEditorDelegates::OnMapOpened.Remove(_mapOpenedHandle);
	//}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> levelList;
	if (false == UGsTerritoryLevelUtil::TryGetWorkLevels(levelCollection, levelList, world, EGsTerritoryLevelType::Server))
	{
		FinishNavigationCurrentServerLevel();
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

		if (FCString::Atoi(*levelKey) == _currentServerLevelId)
		{
			UE_LOG(LogTemp, Log, TEXT("FGsTerritoryLevelManager - load streaming level start - level index : %d\tlevel key : %d"), _currentServerLevelIndex, _currentServerLevelId);

			//Bind OnLoadedServerLevel
			//if (_addServerLevelHandle.IsValid())
			//{
			//	FEditorDelegates::NewCurrentLevel.Remove(_addServerLevelHandle);
			//}

			TArray<TSharedPtr<FLevelModel>> loadLevelList;
			loadLevelList.Add(level);
			_loadLevelList = loadLevelList;
			level->MakeLevelCurrent();

			isFind = true;
			return;
		}
	}

	//만약 로드하는 레벨이 하나도 없다면
	if (false == isFind)
	{
		FinishNavigationCurrentServerLevel();
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeServerLevelNavigationModeState::OnOpenLevelForSaveAllMap - OnOpenLevel End"));
}


//서버 레벨 추가시 콜백
void FGsTerrEdEditorModeServerLevelNavigationModeState::SaveServerStreamingLevel()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeServerLevelNavigationModeState::SaveServerStreamingLevel - OnLoadedServerLevel - Start"));

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		FinishNavigationCurrentServerLevel();
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	FString levelName;
	FString levelKey;
	if (false == UGsTerritoryLevelUtil::IsValidServerLevel(level, levelName, levelKey)
		&& FCString::Atoi(*levelKey) != _currentServerLevelId)
	{
		FinishNavigationCurrentServerLevel();
		return;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeServerLevelNavigationModeState::SaveServerStreamingLevel - OnAddServerLevel - start"));
	//Unbind onChange level
	//if (_addServerLevelHandle.IsValid())
	//{
	//	FEditorDelegates::NewCurrentLevel.Remove(_addServerLevelHandle);
	//}

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

	ExcuteEvent();

	SaveServerLevelInternal();

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeServerLevelNavigationModeState::SaveServerStreamingLevel - OnLoadedServerLevel - End"));
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::SaveServerLevelInternal()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevelForSaveAllMap - SaveServerLevel - Start"));
	UWorld* world = GEditor->GetEditorWorldContext().World();

	//bind post save event
	/*if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}
	_postSaveHandle = FEditorDelegates::PostSaveWorld.AddRaw(this, &FGsTerritoryLevelManager::OnPostSaveServerLevel);*/

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

void FGsTerrEdEditorModeServerLevelNavigationModeState::OnMapChange(uint32 inFalg)
{
	MapChange();
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::OnMapOpend(const FString& inFilename, bool inIsTemplate)
{
	MapOpened(inFilename);
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::OnNewCurrentLevel()
{
	NewCurrentLevel();
}


void FGsTerrEdEditorModeServerLevelNavigationModeState::OnPostSaveLevel(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess)
{
	FTerritoryEditor::GetInstance()->SaveForce();
	
	PostSave();

	//if (_postSaveHandle.IsValid())
	//{
	//	FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	//}

	//FinishNavigationCurrentServerLevel();

	//if (_levelChangeHandle.IsValid())
	//{
	//	inWorld->OnSelectedLevelsChanged().Remove(_levelChangeHandle);
	//}
	//_levelChangeHandle = inWorld->OnSelectedLevelsChanged().AddRaw(this, &FGsTerrEdEditorModeServerLevelNavigationModeState::OnLevelChange);
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::ExcuteEvent()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("test - test"));

	if (_additionalEvent.IsBound())
	{
		_additionalEvent.Broadcast();
	}
}


void FGsTerrEdEditorModeServerLevelNavigationModeState::OnTest()
{
	UE_LOG(LogTemp, Log, TEXT("FGsTerrEdEditorModeServerLevelNavigationModeState::OnTest"));
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::SaveLevel()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	//bind post save event
	/*if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}
	_postSaveHandle = FEditorDelegates::PostSaveWorld.AddRaw(this, &FGsTerrEdEditorModeServerLevelNavigationModeState::OnPostSaveLevel);*/

	FWorldBrowserModule& worldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>("WorldBrowser");
	TWeakPtr<FLevelCollectionModel> levelCollectionModelPtr = worldBrowserModule.SharedWorldModel(world);
	if (levelCollectionModelPtr.IsValid())
	{
		levelCollectionModelPtr.Pin()->SaveLevels(_loadLevelList);
	}
	_loadLevelList.Empty();
}

void FGsTerrEdEditorModeServerLevelNavigationModeState::SetCurrentServerLevelName(const FString& inServerLevel)
{
	_currentServerLevelName = inServerLevel;
}

/// <summary>
/// 레벨이 저장되면 레벨 체인지 함수 호출됨 이 함수에서 변경 해줘야함
/// </summary>
/// <param name="inWorld"></param>
void FGsTerrEdEditorModeServerLevelNavigationModeState::OnLevelChange()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (_levelChangeHandle.IsValid())
	{
		world->OnSelectedLevelsChanged().Remove(_levelChangeHandle);
	}

	ChangeState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE);
}
