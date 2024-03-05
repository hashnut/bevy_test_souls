#include "GsTerrEdEditorModeLevelNavigationModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdModeDefine.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/GsTerritoryLevelManager.h"
#include "LevelNavigationMode/GsTerrEdLevelNavigationMapIdleModeState.h"
#include "LevelNavigationMode/GsTerrEdLevelNavigationMapChangeModeState.h"
#include "LevelNavigationMode/GsTerrEdLevelNavigationMapOpenModeState.h"
#include "LevelNavigationMode/GsTerrEdLevelNavigationNewCurrentLevelModeState.h"
#include "DataSchema/Map/Area/GsSchemaAreaData.h"
#include "AssetRegistryModule.h"
#include "Developer/AssetTools/Public/IAssetTypeActions.h"
#include "ModuleManager.h"
#include "Developer/AssetTools/Public/IAssetTools.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Internationalization.h"
#include "WorldBrowser/Public/WorldBrowserModule.h"

#define LOCTEXT_NAMESPACE	"TerritoryEditor"

FGsTerrEdEditorModeLevelNavigationModeState::FGsTerrEdEditorModeLevelNavigationModeState()
{
	//idle
	FGsTerrEdLevelNavigationMapIdleModeState* mapIdle = new FGsTerrEdLevelNavigationMapIdleModeState();
	mapIdle->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_MAP_IDLE_MODE, MakeShareable(mapIdle));

	//map change
	FGsTerrEdLevelNavigationMapChangeModeState* mapChange = new FGsTerrEdLevelNavigationMapChangeModeState();
	mapChange->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE, MakeShareable(mapChange));

	//map open
	FGsTerrEdLevelNavigationMapOpenModeState* mapOpen = new FGsTerrEdLevelNavigationMapOpenModeState();
	mapOpen->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE, MakeShareable(mapOpen));

	//map new current level
	FGsTerrEdLevelNavigationNewCurrentLevelModeState* newCurrentLevel = new FGsTerrEdLevelNavigationNewCurrentLevelModeState();
	newCurrentLevel->SetParents(this);
	AddState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE, MakeShareable(newCurrentLevel));
}

FGsTerrEdEditorModeLevelNavigationModeState::~FGsTerrEdEditorModeLevelNavigationModeState()
{

}

void FGsTerrEdEditorModeLevelNavigationModeState::Enter()
{
	_currentAreaId = 0;
	_currentAreaName = "";

	UGsTableAreaData* areaTable = FGsTerritoryLevelManager::GetCachedAreaData();	
	areaTable->GetAllRows(_areaDataArray);

	_areaDataNum = _areaDataArray.Num();
	_currentAreaDataIndex = 0;

	//맵이 로드 시작 
	if (_mapChangedHandle.IsValid())
	{
		FEditorDelegates::MapChange.Remove(_mapChangedHandle);
	}
	_mapChangedHandle = FEditorDelegates::MapChange.AddRaw(this, &FGsTerrEdEditorModeLevelNavigationModeState::OnMapChange);

	//맵이 로드 끝
	if (_mapOpenedHandle.IsValid())
	{
		FEditorDelegates::OnMapOpened.Remove(_mapOpenedHandle);
	}
	_mapOpenedHandle = FEditorDelegates::OnMapOpened.AddRaw(this, &FGsTerrEdEditorModeLevelNavigationModeState::OnMapOpend);	

	//서버 레벨 설정 
	if (_newLevelLoadedHandle.IsValid())
	{
		FEditorDelegates::NewCurrentLevel.Remove(_newLevelLoadedHandle);
	}
	_newLevelLoadedHandle = FEditorDelegates::NewCurrentLevel.AddRaw(this, &FGsTerrEdEditorModeLevelNavigationModeState::OnNewCurrentLevel);

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

	SetTickEnable(true);

	ChangeState(TERR_ED_EDITOR_MODE_MAP_IDLE_MODE);
}

void FGsTerrEdEditorModeLevelNavigationModeState::Exit()
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

	_additionalEvent.Clear();

	_currentState.Reset();
	_currentStateName = TEXT("");

	_loadLevelList.Empty();
}

void FGsTerrEdEditorModeLevelNavigationModeState::Tick(float inTick)
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

void FGsTerrEdEditorModeLevelNavigationModeState::MapChange()
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE);
}

void FGsTerrEdEditorModeLevelNavigationModeState::MapOpened(const FString& inFilename)
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE);	
}

//Open server level
void FGsTerrEdEditorModeLevelNavigationModeState::NewCurrentLevel()
{
	ChangeState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE);
}

bool FGsTerrEdEditorModeLevelNavigationModeState::StartLoadLevel(const FString& inLevelName)
{
	return OpenLevelForSaveAllMap(inLevelName);
}

void FGsTerrEdEditorModeLevelNavigationModeState::FinishNavigationLevel()
{
	FinishNavigationCurrentLevel();

	_currentAreaId = 0;
	_currentAreaName = "";	

	if (_additionalEventHandle.IsValid())
	{
		_additionalEvent.Remove(_additionalEventHandle);
	}

	FTerritoryEditor::GetInstance()->ChangeLevelEditMode();
	FTerritoryEditor::GetInstance()->ChangeLevelEdNewCurrentLevelMode();

	SetTickEnable(false);

	FText okTitleText = FText::FromString("Deactive xray vision");
	FText okMessageText = FText::FromString("Deactive xray vision success");
	FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, okTitleText, &okMessageText);
}

void FGsTerrEdEditorModeLevelNavigationModeState::FinishNavigationCurrentLevel()
{	
	_currentAreaDataIndex = ++_currentAreaDataIndex;
}

bool FGsTerrEdEditorModeLevelNavigationModeState::OpenLevelForSaveAllMap(const FString& inLevelName)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::FGsTerrEdEditorModeLevelNavigationModeState - Start"));

	const FString& PackageName = TEXT("/Game/Maps/") + inLevelName + TEXT("/") + inLevelName;
	TArray<FAssetData> ActivatedAssets;
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	if (!AssetRegistry.GetAssetsByPackageName(*PackageName, ActivatedAssets))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("!AssetRegistry.GetAssetsByPackageName(%s, Assets)"), *PackageName);

		FinishNavigationLevel();
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

void FGsTerrEdEditorModeLevelNavigationModeState::OnMapChange(uint32 inFalg)
{
	MapChange();
}

void FGsTerrEdEditorModeLevelNavigationModeState::OnMapOpend(const FString& inFilename, bool inIsTemplate)
{
	MapOpened(inFilename);
}

void FGsTerrEdEditorModeLevelNavigationModeState::OnNewCurrentLevel()
{
	NewCurrentLevel();
}

void FGsTerrEdEditorModeLevelNavigationModeState::ExcuteEvent()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("test - test"));

	if (_additionalEvent.IsBound())
	{
		_additionalEvent.Broadcast();
	}
}

void FGsTerrEdEditorModeLevelNavigationModeState::SaveLevel()
{	
	UWorld* world = GEditor->GetEditorWorldContext().World();

	//bind post save event
	if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}
	_postSaveHandle = FEditorDelegates::PostSaveWorld.AddRaw(this, &FGsTerrEdEditorModeLevelNavigationModeState::OnPostSaveLevel);

	FWorldBrowserModule& worldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>("WorldBrowser");
	TWeakPtr<FLevelCollectionModel> levelCollectionModelPtr = worldBrowserModule.SharedWorldModel(world);
	if (levelCollectionModelPtr.IsValid())
	{
		levelCollectionModelPtr.Pin()->SaveLevels(_loadLevelList);
	}
	_loadLevelList.Empty();
}

void FGsTerrEdEditorModeLevelNavigationModeState::OnPostSaveLevel(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess)
{
	if (_postSaveHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSaveHandle);
	}

	FinishNavigationCurrentLevel();	

	if (_levelChangeHandle.IsValid())
	{
		inWorld->OnSelectedLevelsChanged().Remove(_levelChangeHandle);
	}
	_levelChangeHandle = inWorld->OnSelectedLevelsChanged().AddRaw(this, &FGsTerrEdEditorModeLevelNavigationModeState::OnLevelChange);
}

/// <summary>
/// 레벨이 저장되면 레벨 체인지 함수 호출됨 이 함수에서 변경 해줘야함
/// </summary>
/// <param name="inWorld"></param>
void FGsTerrEdEditorModeLevelNavigationModeState::OnLevelChange()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (_levelChangeHandle.IsValid())
	{
		world->OnSelectedLevelsChanged().Remove(_levelChangeHandle);
	}

	ChangeState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE);
}
