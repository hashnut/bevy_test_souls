// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryEditor.h"

//UE4 Editor
#include "Editor.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "Engine/WorldComposition.h"
#include "Interfaces/IMainFrameModule.h"
#include "LevelEditor.h"
#include "MultiBoxExtender.h"
#include "Misc/MessageDialog.h"
#include "SlateBasics.h"
#include "Templates/SharedPointer.h"
#include "GameplayTask.h"
#include "GameMapsSettings.h"
#include "PropertyEditorModule.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Containers/Ticker.h"

//T1Project
#include "RSQLEditor/Public/RSQLEditorDelegates.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "UTIL/GsText.h"
#include "RSQLEditor/Private/Helper/GsNotificatorOnDestruction.h"

//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spot/GsTerritorySpot.h"

#include "Blutility/Input/GsTerritorySpawnActorInput.h"
#include "Editor/GsTerritoryNpcInputDetailPanel.h"

#include "Editor/GsTerritoryLevelEditorEx.h"
#include "GsTerritoryEditorSetting.h"
#include "GsTerritoryEditorCommands.h"
#include "GsTerritoryEditorStyle.h"
#include "UTIL/GsLevelUtil.h"
#include "Util/GsTerritoryEditorFileUtil.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Editor/GsTerritoryMapSpotLayout.h"
#include "Blutility/Input/GsTerritorySpotInput.h"
#include "Editor/GsTerritoryWarpInputDetailPanel.h"
#include "Editor/GsTerritoryPropLayout.h"
#include "DataTableEditor/GsDataTableEditor.h"
#include "UnrealEdGlobals.h"
#include "SDockTab.h"
#include "SlateFwd.h"
#include "Slate/GsTerritoryEditorTool.h"
#include "Editor/GsTerrEdSpawnObjectShowHideLayout.h"
#include "Editor/GsTerrEdShapeDataLayout.h"
#include "Editor/GsTerrEdLIneDataLayout.h"
#include "Editor/GsTerrEdEnvConditionLayout.h"
#include "EditorSupportDelegates.h"
#include "TerritoryEditor/Mode/GsTerrEdPlayModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdEditorModeState.h"
#include "Mode/GsTerrEdModeStateManager.h"
#include "Mode/GsTerrEdEditorModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdModeDefine.h"
#include "Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"
#include "CoreGlobals.h"
#include "GameObject/ObjectClass/Data/GsEnvCondition.h"
#include "TerritoryEditor/Editor/GsTerrEdQuestSoundLayout.h"
#include "Mode/EditorMode/GsTerrEdEditorModeLevelEdModeState.h"
#include "Editor/GsTerritoryCurrentMapSpotLayout.h"

#define TERRITORY_WINDOW false

IMPLEMENT_MODULE(FTerritoryEditor, TerritoryEditor);
#define LOCTEXT_NAMESPACE	"TerritoryEditor"

DEFINE_LOG_CATEGORY(LogTerritoryEditor)

FTerritoryEditor* FTerritoryEditor::_instance = nullptr;
bool FTerritoryEditor::_isActive = true;
TerrEdClearFinderEvent FTerritoryEditor::_territoryEditorClearFinderEvent;

void FTerritoryEditor::StartupModule()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::StartupModule"));
#endif

	_instance = this;

	_isActive = true;
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (settings)
	{
		_isActive = settings->_isActive;
		if (false == _isActive)
		{
			UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor is deactive"));
		}
	}

	if (_isActive)
	{
		InitManagers();

		RegisterSetting();
		RegisterCommands();
		RegisterCustomLayoutForBlutility();
		RegisterTerritoryToolbar();

		BindEditorEvents();
		BindRuntimeEvents();

		_tickDelegate = FTickerDelegate::CreateRaw(this, &FTerritoryEditor::Tick);
		_tickDelegateHandle = FTicker::GetCoreTicker().AddTicker(_tickDelegate);

		_duration = 0.2f;
	}
	else
	{
		InitManagers();

		RegisterSetting();

		BindEditorEvents();
		BindRuntimeEvents();

		_tickDelegate = FTickerDelegate::CreateRaw(this, &FTerritoryEditor::Tick);
		_tickDelegateHandle = FTicker::GetCoreTicker().AddTicker(_tickDelegate);

		_duration = 0.2f;		
	}
}

void FTerritoryEditor::ShutdownModule()
{
	if (_isActive)
	{
		if (_tickDelegateHandle.IsValid())
		{
			FTicker::GetCoreTicker().RemoveTicker(_tickDelegateHandle);
		}

		UnbindRuntimeEvents();
		UnbindEditorEvents();

		UnregisterTerritoryToolbar();
		UnregisterCustomLayoutForBlutility();
		UnregisterCommands();
		UnregisterSetting();

		_territoryEditorTickEvent.Clear();
		_territoryEditorClearFinderEvent.Clear();

		ClearManagers();
	}
	else
	{
		if (_tickDelegateHandle.IsValid())
		{
			FTicker::GetCoreTicker().RemoveTicker(_tickDelegateHandle);
		}

		UnbindRuntimeEvents();
		UnbindEditorEvents();

		UnregisterSetting();

		_territoryEditorTickEvent.Clear();
		_territoryEditorClearFinderEvent.Clear();

		ClearManagers();
	}	
}

void FTerritoryEditor::InitManagers()
{
	_levelManager = MakeShareable(new FGsTerritoryLevelManager());
	if (_levelManager.IsValid())
	{
		_levelManager->Initialize();
	}

	_modeStateManager = MakeShareable(new FGsTerrEdModeStateManager());
	if (_modeStateManager.IsValid())
	{
		_modeStateManager->ChangeState(TERR_ED_EDITOR_MODE);
	}

	if (_isActive)
	{
		_uniqueIDManager = MakeShareable(new FGsTerritoryUniqueIDManager());
		if (_uniqueIDManager.IsValid())
		{
			_uniqueIDManager->Initialize();
		}

		_dataManager = MakeShareable(new FGsTerritoryDataManager());
		if (_dataManager.IsValid())
		{
			_dataManager->Initialize();
		}
	}	
}

void FTerritoryEditor::ClearManagers()
{
	if (_uniqueIDManager.IsValid())
	{
		_uniqueIDManager->Deinitialize();
		_uniqueIDManager.Reset();
	}

	if (_levelManager.IsValid())
	{
		_levelManager->Deinitialize();
		_levelManager.Reset();
	}

	if (_dataManager.IsValid())
	{
		_dataManager->Clear();
		_dataManager.Reset();
	}

	if (_modeStateManager.IsValid())
	{
		_modeStateManager.Reset();
	}
}

bool FTerritoryEditor::Tick(float inDelta)
{
	if (_time > _duration)
	{
		_time = 0.0f;
		_territoryEditorTickEvent.Broadcast(inDelta);

		if (_modeStateManager.IsValid())
		{
			_modeStateManager->Tick(inDelta);
		}
	}

	_time += inDelta;

	return true;
}

void FTerritoryEditor::BindEditorEvents()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindEditorEvents"));
#endif

	//Editor time
	if (_mapChangedHandle.IsValid())
	{
		FEditorDelegates::MapChange.Remove(_mapChangedHandle);
	}
	_mapChangedHandle = FEditorDelegates::MapChange.AddRaw(this, &FTerritoryEditor::OnMapChange);

	FGsDataTableEditor::CallbackRegisterRSQLEditor.BindRaw(this, &FTerritoryEditor::OnOpenSpawnDataTable);

	FLevelCollectionModel::_checkRegisterServerLevelEvent.BindStatic(&FTerritoryEditor::OnCheckServerLevelRegistered);
	FLevelCollectionModel::_unloadPreLoadedServerLevelEvent.BindStatic(&FTerritoryEditor::OnUnloadPreLoadedServerLevel);
}

void FTerritoryEditor::UnbindEditorEvents()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnbindEditorEvents"));
#endif

	//Editor time
	UnbindPostSave();
	UnbindMapChange();
	UnbindMapOpen();
	UnbindNewCurrentLevel();
	UnbindActorAdded();
	UnbindActorDeleted();

	FGsDataTableEditor::CallbackRegisterRSQLEditor.Unbind();

	FWorldTileCollectionModel::_checkRegisterServerLevelEvent.Unbind();
	FWorldTileCollectionModel::_unloadPreLoadedServerLevelEvent.Unbind();
}

void FTerritoryEditor::BindRuntimeEvents()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindRuntimeEvents"));
#endif

	//play and end game in editor
	if (_postPIEStartedHandle.IsValid())
	{
		FEditorDelegates::PostPIEStarted.Remove(_postPIEStartedHandle);
	}
	_postPIEStartedHandle = FEditorDelegates::PostPIEStarted.AddRaw(this, &FTerritoryEditor::OnPlayInEditor);

	if (_endPIEHandle.IsValid())
	{
		FEditorDelegates::EndPIE.Remove(_endPIEHandle);
	}
	_endPIEHandle = FEditorDelegates::EndPIE.AddRaw(this, &FTerritoryEditor::OnEndPlayInEditor);
}

void FTerritoryEditor::UnbindRuntimeEvents()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::UnbindRuntimeEvents"));
#endif

	//play and end game in editor
	if (_postPIEStartedHandle.IsValid())
	{
		FEditorDelegates::PostPIEStarted.Remove(_postPIEStartedHandle);
	}

	if (_endPIEHandle.IsValid())
	{
		FEditorDelegates::EndPIE.Remove(_endPIEHandle);
	}
}

bool FTerritoryEditor::RegisterSetting()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::RegisterSetting"));
#endif

	ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (nullptr == settingsModule)
	{
		return false;
	}

	ISettingsContainerPtr settingsContainer = settingsModule->GetContainer("Project");

	settingsContainer->DescribeCategory("TerritoryEditor"
		, LOCTEXT("TerritoryEditorCategoryName", "TerritoryEditor")
		, LOCTEXT("TerritoryEditorCategoryDescription", "Territory editor description"));

	ISettingsSectionPtr settingsSection = settingsModule->RegisterSettings("Project"
		, "TerritoryEditor"
		, "TerritoryEditorSetting"
		, LOCTEXT("TerritoryEditor", "Territory editor 설정")
		, LOCTEXT("TerritoryEditorCategoryDescription", "Territory editor 환경 설정 입니다.")
		, GetMutableDefault<UGsTerritoryEditorSetting>());

	if (settingsSection.IsValid())
	{
		settingsSection->OnModified().BindRaw(this, &FTerritoryEditor::OnModifySetting);
	}

	return true;
}

bool FTerritoryEditor::UnregisterSetting()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::UnregisterSetting"));
#endif

	if (ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		settingsModule->UnregisterSettings("Project", "TerritoryEditor", "TerritoryEditorSetting");
	}

	return true;
}

bool FTerritoryEditor::OnModifySetting()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnModifySetting"));
#endif

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		settings->SaveConfig();
		settings->UpdateDefaultConfigFile();
	}

	return true;
}

void FTerritoryEditor::RegisterCommands()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::RegisterCommands"));
#endif

	FGsTerritoryEditorStyle::Initialize();
	FGsTerritoryEditorStyle::ReloadTextures();
	FGsTerritoryEditorCommands::Register();
}

void FTerritoryEditor::UnregisterCommands()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::UnregisterCommands"));
#endif

	FGsTerritoryEditorCommands::Unregister();
	FGsTerritoryEditorStyle::Shutdown();
}

void FTerritoryEditor::RegisterCustomLayoutForBlutility()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::RegisterCustomLayoutForBlutility"));
#endif

	RegisterCustomPropertyTypeLayout<FGsTerritoryNpcInput, FGsTerritoryNpcInputDetailPanel>();
	RegisterCustomPropertyTypeLayout<FGsTerritoryPropInput, FGsTerritoryPropLayout>();
	RegisterCustomPropertyTypeLayout<FGsTerritoryMapWarp, FGsTerritoryWarpInputDetailPanel>();
	RegisterCustomPropertyTypeLayout<FGsTerritoryMapSpot, FGsTerritoryMapSpotLayout>();
	RegisterCustomPropertyTypeLayout<FGsTerritoryCurrentMapSpot, FGsTerritoryCurrentMapSpotLayout>();
	RegisterCustomPropertyTypeLayout<FGsTerritoryShapeId, FGsTerrEdShapeDataLayout>();
	RegisterCustomPropertyTypeLayout<FGsTerritoryLineId, FGsTerrEdLIneDataLayout>();
	RegisterCustomPropertyTypeLayout<FGsTerrEdSpawnObjectShowHide, FGsTerrEdSpawnObjectShowHideLayout>();
	RegisterCustomPropertyTypeLayout<FGsEnvCondition, FGsTerrEdEnvConditionLayout>();
	RegisterCustomPropertyTypeLayout<FGsQuestSound, FGsTerrEdEnvConditionLayout>();
}

void FTerritoryEditor::UnregisterCustomLayoutForBlutility()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnregisterCustomLayoutForBlutility"));
#endif

	UnregisterCustomPropertyTypeLayout<FGsTerritoryNpcInput>();
	UnregisterCustomPropertyTypeLayout<FGsTerritoryPropInput>();
	UnregisterCustomPropertyTypeLayout<FGsTerritoryMapWarp>();
	UnregisterCustomPropertyTypeLayout<FGsTerritoryMapSpot>();
	UnregisterCustomPropertyTypeLayout<FGsTerritoryShapeId>();
	UnregisterCustomPropertyTypeLayout<FGsTerritoryLineId>();
	UnregisterCustomPropertyTypeLayout<FGsTerrEdSpawnObjectShowHide>();
	UnregisterCustomPropertyTypeLayout<FGsEnvCondition>();
	UnregisterCustomPropertyTypeLayout<FGsQuestSound>();
}

/// <summary>
/// 맵이 변경 시작시 호출
/// </summary>
/// <param name="inFalg"></param>
void FTerritoryEditor::OnMapChange(uint32 inFalg)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnMapChange"));
#endif

	//LevelEdit, CopyLevel, Sequcenc LevelCpoy의 각 모드 마다의 MapChange 함수를 호출 해줘야한다.
	if (FGsTerrEdEditorModeState* modeState = GetEditorModeState())
	{
		FGsTerrEdModeStateBase* editorMode = modeState->GetCurrentState();
		if (editorMode)
		{
			if (FGsTerrEdEditorModeLevelStateBase* levelInterface = static_cast<FGsTerrEdEditorModeLevelStateBase*>(editorMode))
			{
				levelInterface->MapChange();
			}
		}
	}
}

/// <summary>
/// 맵 변경 완료 후 호출
/// </summary>
/// <param name="inFilename"></param>
/// <param name="inIsTemplate"></param>
void FTerritoryEditor::OnMapOpend(const FString& inFilename, bool inIsTemplate)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnMapOpend"));
#endif

	if (FGsTerrEdEditorModeState* modeState = GetEditorModeState())
	{
		FGsTerrEdModeStateBase* editorMode = modeState->GetCurrentState();
		if (editorMode)
		{
			if (FGsTerrEdEditorModeLevelStateBase* levelInterface = static_cast<FGsTerrEdEditorModeLevelStateBase*>(editorMode))
			{
				levelInterface->MapOpened(inFilename);
			}
		}
	}
}

/// <summary>
///  SubLevel이 로드 될 때 호출된다. 
/// </summary>
void FTerritoryEditor::OnNewCurrentLevel()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnNewCurrentLevel"));

	if (FGsTerrEdEditorModeState* modeState = GetEditorModeState())
	{
		FGsTerrEdModeStateBase* editorMode = modeState->GetCurrentState();
		if (editorMode)
		{
			if (FGsTerrEdEditorModeLevelStateBase* levelInterface = static_cast<FGsTerrEdEditorModeLevelStateBase*>(editorMode))
			{
				levelInterface->NewCurrentLevel();
			}
		}
	}
}

void FTerritoryEditor::OnActorAdded(AActor* inActor)
{
	if (inActor
		&& inActor->IsA(AGsTerritoryActor::StaticClass())
		&& false == inActor->IsA(AGsTerritoryZoneShape::StaticClass()))
	{
		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnActorAdded - label : %s"), *inActor->GetActorLabel());
	}
}

void FTerritoryEditor::OnActorDeleted(AActor* inActor)
{
	if (_levelManager.IsValid())
	{
		_levelManager->TerritoryActorDeleted(inActor);
	}
}

void FTerritoryEditor::OnPlayInEditor(bool inIsSimulating)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnPlayInEditor"));
#endif

	//Play editor
	if (_modeStateManager.IsValid())
	{
		_modeStateManager.Get()->ChangeState(TERR_ED_PLAY_MODE);
	}
}

void FTerritoryEditor::BindMapOpen()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindMapOpen"));
#endif

	if (_mapOpenedHandle.IsValid())
	{
		FEditorDelegates::OnMapOpened.Remove(_mapOpenedHandle);
	}
	_mapOpenedHandle = FEditorDelegates::OnMapOpened.AddRaw(this, &FTerritoryEditor::OnMapOpend);
}

void FTerritoryEditor::UnbindMapOpen()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnbindMapOpen"));
#endif

	if (_mapOpenedHandle.IsValid())
	{
		FEditorDelegates::OnMapOpened.Remove(_mapOpenedHandle);
		_mapOpenedHandle.Reset();
	}
}

void FTerritoryEditor::BindMapChange()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindMapChange"));
#endif

	if (_mapChangedHandle.IsValid())
	{
		FEditorDelegates::MapChange.Remove(_mapChangedHandle);
	}
	_mapChangedHandle = FEditorDelegates::MapChange.AddRaw(this, &FTerritoryEditor::OnMapChange);
}

void FTerritoryEditor::UnbindMapChange()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnbindMapChange"));
#endif

	if (_mapChangedHandle.IsValid())
	{
		FEditorDelegates::MapChange.Remove(_mapChangedHandle);
		_mapChangedHandle.Reset();
	}
}

void FTerritoryEditor::BindNewCurrentLevel()
{
	if (false == _isActive)
	{
		return;
	}

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindNewCurrentLevel"));
#endif

	if (_newLevelLoadedHandle.IsValid())
	{
		FEditorDelegates::NewCurrentLevel.Remove(_newLevelLoadedHandle);
	}
	_newLevelLoadedHandle = FEditorDelegates::NewCurrentLevel.AddRaw(this, &FTerritoryEditor::OnNewCurrentLevel);
}

void FTerritoryEditor::UnbindNewCurrentLevel()
{
	if (false == _isActive)
	{
		return;
	}

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnbindNewCurrentLevel"));
#endif

	if (_newLevelLoadedHandle.IsValid())
	{
		FEditorDelegates::NewCurrentLevel.Remove(_newLevelLoadedHandle);
		_newLevelLoadedHandle.Reset();
	}
}

void FTerritoryEditor::BindPostSave()
{
	if (false == _isActive)
	{
		return;
	}

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindPostSave"));
#endif

	if (_postSavedHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSavedHandle);
	}
	_postSavedHandle = FEditorDelegates::PostSaveWorld.AddRaw(this, &FTerritoryEditor::OnPostSave);
}

void FTerritoryEditor::UnbindPostSave()
{
	if (false == _isActive)
	{
		return;
	}

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnbindPostSave"));
#endif

	if (_postSavedHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_postSavedHandle);
		_postSavedHandle.Reset();
	}
}

void FTerritoryEditor::BindActorAdded()
{
	if (false == _isActive)
	{
		return;
	}

	if (nullptr == GEditor)
	{
		return;
	}		

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindActorAdded"));
#endif

	if (_levelActorAddedHandle.IsValid())
	{
		GEditor->OnLevelActorAdded().Remove(_levelActorAddedHandle);
	}
	_levelActorAddedHandle = GEditor->OnLevelActorAdded().AddRaw(this, &FTerritoryEditor::OnActorAdded);

}

void FTerritoryEditor::UnbindActorAdded()
{
	if (false == _isActive)
	{
		return;
	}

	if (nullptr == GEditor)
	{
		return;
	}		

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnbindActorAdded"));
#endif

	if (_levelActorAddedHandle.IsValid())
	{
		GEditor->OnLevelActorAdded().Remove(_levelActorAddedHandle);
		_levelActorAddedHandle.Reset();
	}
}

void FTerritoryEditor::BindActorDeleted()
{
	if (false == _isActive)
	{
		return;
	}

	if (nullptr == GEditor)
	{
		return;
	}		

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::BindActorDeleted"));
#endif

	if (_levelActorDeletedHandle.IsValid())
	{
		GEditor->OnLevelActorDeleted().Remove(_levelActorDeletedHandle);
	}
	_levelActorDeletedHandle = GEditor->OnLevelActorDeleted().AddRaw(this, &FTerritoryEditor::OnActorDeleted);
}

void FTerritoryEditor::UnbindActorDeleted()
{
	if (false == _isActive)
	{
		return;
	}

	if (nullptr == GEditor)
	{
		return;
	}		

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnbindActorDeleted"));
#endif

	if (_levelActorDeletedHandle.IsValid())
	{
		GEditor->OnLevelActorDeleted().Remove(_levelActorDeletedHandle);
		_levelActorDeletedHandle.Reset();
	}
}

void FTerritoryEditor::OnEndPlayInEditor(bool inIsSimulating)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnEndPlayInEditor"));
#endif

	if (_modeStateManager.IsValid())
	{
		_modeStateManager.Get()->ChangeState(TERR_ED_EDITOR_MODE);
	}
}

void FTerritoryEditor::OnPostSave(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess)
{
	if (IsRunningCommandlet())
	{
		return;
	}

#if TERRITORY_EDITOR_DEBUG
	//is server level
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnPostSave"));
#endif

	const FString& levelName = UGameplayStatics::GetCurrentLevelName(inWorld);
	bool isServerLevel = UGsTerritoryLevelUtil::IsRegisteredLevel(levelName);
	if (isServerLevel)
	{
		//check navmesh exist
		if (false == FGsTerritoryLevelManager::IsNavmeshExist(inWorld))
		{
			UE_LOG(LogTemp, Warning, TEXT("TERRITORY_EDITOR_DEBUG - Navmesh is missing - level : %s"), *levelName);
		}
	}

	if (_uniqueIDManager.IsValid())
	{
		_uniqueIDManager->OnPostSaveUniqueTable(inSaveFlag, inWorld, inSuccess);
	}

	if (_dataManager.IsValid())
	{
		_dataManager->Save(inSaveFlag, inWorld, inSuccess);
	}
}

void FTerritoryEditor::OnOpenSpawnDataTable(FString inTableName, IDataTableEditor* inEditor)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnOpenSpawnDataTable"));
#endif

	//get table name and level key index
	TArray<FString> parsArray;
	int32 count = inTableName.ParseIntoArray(parsArray, TEXT("_"));
	if (count != 2)
	{
		return;
	}

	if (parsArray[0] != TEXT("DTGsSpawnData"))
	{
		return;
	}

	//register select event and double click event
	if (nullptr == inEditor)
	{
		return;
	}

	FGsDataTableEditor* editor = static_cast<FGsDataTableEditor*>(inEditor);
	if (editor)
	{
		editor->CallbackOnDataTableRowHighlighted.BindRaw(this, &FTerritoryEditor::OnSelectedSpawnDataTable);
		editor->CallbackOnDataTableRowDoubleClicked.BindRaw(this, &FTerritoryEditor::OnDoubleClickedSpawnDataTable);
	}
}

void FTerritoryEditor::OnSelectedSpawnDataTable(FString inDataTableName, FName inRowName)
{

}

void FTerritoryEditor::OnDoubleClickedSpawnDataTable(FString inDataTableName, FName inRowName)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnDoubleClickedSpawnDataTable"));
#endif

	//if opened level is matched to opened spawn data table by level key 
	FString levelKey;
	if (false == IsMatchedTableToLevel(inDataTableName, TEXT("DTGsSpawnData"), levelKey))
	{
		FString levelName = UGsTerritoryLevelUtil::GetLevelName(FCString::Atoi(*levelKey));
		if (levelName.Equals(TEXT("")))
		{
			FString log = TEXT("Unregistered spawn data");
			FGsNotificatorOnDestruction::ShowNotificationNow(FText::FromString(log), SNotificationItem::ECompletionState::CS_Fail);
		}
		else
		{
			FString log = FString::Printf(TEXT("%s is not opened or zone that have unique id %s is not exist"), *levelName, *inRowName.ToString());
			FGsNotificatorOnDestruction::ShowNotificationNow(FText::FromString(log), SNotificationItem::ECompletionState::CS_Fail);
		}

		return;
	}

	//select object by key
	UWorld* world = GEditor->GetEditorWorldContext().World();
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryActor::StaticClass(), actorList);
	for (AActor* actor : actorList)
	{
		AGsTerritoryActor* castActor = Cast<AGsTerritoryActor>(actor);
		if (nullptr == castActor)
		{
			continue;
		}

		FName uniqueId = *FString::Printf(TEXT("%lld"), castActor->GetTerritoryUniqueID());
		if (uniqueId != inRowName)
		{
			continue;
		}

		USelection* select = GEditor->GetSelectedActors();
		if (select)
		{
			TArray<AActor*> selectList;
			selectList.Add(castActor);
			UGsTerritoryLevelUtil::SetSelectedLevelActors(selectList);

			if (GUnrealEd)
			{
				GUnrealEd->Exec(world, TEXT("CAMERA ALIGN ACTIVEVIEWPORTONLY"));
			}

			return;
		}
	}
}

bool FTerritoryEditor::IsMatchedTableToLevel(const FString& inTableName, const FString& inTargetTableName, FString& outLevelKey)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::IsMatchedTableToLevel"));
#endif

	outLevelKey = "";
	TArray<FString> parseArray;
	inTableName.ParseIntoArray(parseArray, TEXT("_"));
	if (parseArray.Num() != 2)
	{
		return false;
	}

	if (false == parseArray[0].Equals(inTargetTableName))
	{
		return false;
	}

	outLevelKey = parseArray[1];
	UWorld* world = GEditor->GetEditorWorldContext().World();
	const FString& path = world->GetCurrentLevel()->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
	{
		return false;
	}

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(path);
	FString levelKey;
	FString persistantLevel;
	if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevel, levelKey))
	{
		return false;
	}

	return levelKey.Equals(parseArray[1]);
}

void FTerritoryEditor::RegisterTerritoryToolbar()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::RegisterTerritoryToolbar"));
#endif

	_toolbarExtender = MakeShareable(new FExtender());
	if (_toolbarExtender.IsValid())
	{
		_toolbarExtender->AddToolBarExtension("Settings",
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FToolBarExtensionDelegate::CreateRaw(this, &FTerritoryEditor::ExtendToolbar));

		// Add skill editor button widget to level editor.
		FLevelEditorModule& levelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		levelEditorModule.GetToolBarExtensibilityManager()->AddExtender(_toolbarExtender);
	}

	_tool = MakeShareable(new FGsTerritoryEditorTool());
	if (_tool.IsValid())
	{
		_tool->ReigsterTab();
	}
}

void FTerritoryEditor::UnregisterTerritoryToolbar()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::UnregisterTerritoryToolbar"));
#endif

	if (_tool.IsValid())
	{
		_tool->UnregisterTab();
	}

	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& levelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
		levelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(_toolbarExtender);
	}
}

void FTerritoryEditor::ExtendToolbar(FToolBarBuilder& ToolbarBuilder)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::ExtendToolbar"));
#endif

	ToolbarBuilder.BeginSection("Territory");
	{
		ToolbarBuilder.AddToolBarButton
		(
			FUIAction
			(
				FExecuteAction::CreateRaw(this, &FTerritoryEditor::OnOpenTerritoryEditorTool)
			),
			NAME_None,
			LOCTEXT("OpenTerritoryEditor", "Territory Editor"),
			LOCTEXT("OpenTerritoryToolTip", "Opens the TerritoryEditor."),
			FSlateIcon(FGsTerritoryEditorStyle::GetStyleSetName(), "TerritoryEditor.OpenTerritoryEditor")
		);
	}
	ToolbarBuilder.EndSection();
}

void FTerritoryEditor::OnOpenTerritoryEditorTool()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnOpenTerritoryEditorTool"));
#endif

	FGlobalTabmanager::Get()->TryInvokeTab(TERRITORY_TOOL_NAME);
}

bool FTerritoryEditor::OnCheckServerLevelRegistered(const FString& inServerLevel)
{
	FString persistantLevelName;
	FString levelKey;
	if (UGsTerritoryLevelUtil::IsValidServerLevelName(inServerLevel, persistantLevelName, levelKey))
	{
		return UGsTerritoryLevelUtil::IsRegisteredLevel(levelKey, persistantLevelName);
	}

	return false;
}

bool FTerritoryEditor::OnUnloadPreLoadedServerLevel()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnUnloadPreLoadedServerLevel"));
#endif

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return false;
	}

	//Finder에 Actor가 Empty 안되있으면 맵 변경시 가비지 콜렉션이 정상적으로 이루어지지 않아 맵 로딩이 느려짐
	FTerritoryEditor::_territoryEditorClearFinderEvent.Broadcast();

	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> serverLevelArray;
	TArray<TSharedPtr<FLevelModel>> unloadServerLevelArray;
	TArray<TSharedPtr<FLevelModel>> dirtyServerLevelArray;

	if (UGsTerritoryLevelUtil::TryGetWorkLevels(levelCollection, serverLevelArray, world, EGsTerritoryLevelType::Server))
	{
		for (TSharedPtr<FLevelModel> level : serverLevelArray)
		{
			if (level.IsValid())
			{
				if (level.Get()->IsVisible() || level.Get()->IsLoaded())
				{
					unloadServerLevelArray.Add(level);
					if (level->IsDirty())
					{
						dirtyServerLevelArray.Add(level);
					}
				}
			}
		}

		//open popup
		bool isLoadLevel = true;
		if (dirtyServerLevelArray.Num() > 0)
		{
			int32 num = dirtyServerLevelArray.Num();
			FString dirtyLevelNames;
			for (int32 i = 0; i < num; ++i)
			{
				if (dirtyServerLevelArray[i].IsValid())
				{
					dirtyLevelNames += dirtyServerLevelArray[i].Get()->GetDisplayName();
					if (i != num - 1)
					{
						dirtyLevelNames += ", ";
					}
				}
			}

			FText title = FText::FromString(TEXT("Territory editor"));
			FString dirtyServerLevelLog = FString::Printf(TEXT("Pre loaded server levels %s are not saved, do you want to load new level without pre server level save?"), *dirtyLevelNames);
			EAppReturnType::Type selectValue = FMessageDialog::Open(EAppMsgType::YesNo,
				FText::FromString(dirtyServerLevelLog),
				&title);

			isLoadLevel = (selectValue == EAppReturnType::Yes);
			if (false == isLoadLevel)
			{
				return false;
			}
		}

		for (TWeakPtr<FLevelModel> level : unloadServerLevelArray)
		{
			if (level.IsValid())
			{
				if (level.Pin()->IsDirty())
				{
					if (isLoadLevel)
					{
						level.Pin()->GetLevelObject()->GetOutermost()->SetDirtyFlag(false);
						UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnUnloadPreLoadedServerLevel - unlaod server level without save - level : %s"), *level.Pin()->GetDisplayName());
						continue;
					}
				}

				UE_LOG(LogTerritoryEditor, Log, TEXT("FTerritoryEditor::OnUnloadPreLoadedServerLevel - unlaod server level : %s"), *level.Pin()->GetDisplayName());
			}
		}

		/*if ()
		{
			levelCollection.Pin()->SaveLevels(dirtyServerLevelArray);
		}*/

		levelCollection.Pin()->UnloadLevels(unloadServerLevelArray);
	}

	//FEditorSupportDelegates::RedrawAllViewports.Broadcast();
	FEditorDelegates::RefreshLayerBrowser.Broadcast();
	//GEditor->NoteSelectionChange();
	//GEngine->BroadcastLevelActorListChanged();
	//FEditorDelegates::RefreshLayerBrowser.Broadcast();

	return true;
}

void FTerritoryEditor::ChangeLevelEditMode()
{
	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return;
	}

	editorMode->ChangeState(TERR_ED_EDITOR_MODE_LEVEL_EDIT_MODE);
}

void FTerritoryEditor::ChangeLevelEdNewCurrentLevelMode()
{
	FGsTerrEdEditorModeLevelStateBase* state = FTerritoryEditor::GetInstance()->GetEditorLevelEdModeState();
	if (nullptr == state)
	{
		return;
	}

	FGsTerrEdEditorModeLevelEdModeState* editorState = static_cast<FGsTerrEdEditorModeLevelEdModeState*>(state);
	if (nullptr == editorState)
	{
		return;
	}

	editorState->ChangeState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE);
}

void FTerritoryEditor::ChangeLevelCopyMode()
{
	FGsTerrEdEditorModeState* editorMode = FTerritoryEditor::GetInstance()->GetEditorModeState();
	if (nullptr == editorMode)
	{
		return;
	}

	editorMode->ChangeState(TERR_ED_EDITOR_MODE_LEVEL_COPY_MODE);
}

bool FTerritoryEditor::SaveForce()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	ULevel* level = world->GetCurrentLevel();
	if (nullptr == level)
	{
		return false;
	}

	const FString& path = level->GetPathName();
	const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(path));

	FString levelKey;
	FString persistantLevelName;	
	EGsTerritoryMapType mapType = EGsTerritoryMapType::Art;
	if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		mapType = EGsTerritoryMapType::Server;
	}
	else if (UGsTerritoryLevelUtil::IsValidDesignLevelName(levelName, persistantLevelName, levelKey))
	{
		mapType = EGsTerritoryMapType::Design;
	}

	if (mapType == EGsTerritoryMapType::Art)
	{
		return false;
	}

	bool isServerLevel = UGsTerritoryLevelUtil::IsRegisteredLevel(persistantLevelName);
	if (false == isServerLevel)
	{
		return false;
	}

	//check navmesh exist
	if (false == FGsTerritoryLevelManager::IsNavmeshExist(world))
	{
		UE_LOG(LogTemp, Warning, TEXT("TERRITORY_EDITOR_DEBUG - Navmesh is missing - level : %s"), *levelName);
	}
	
	if (EGsTerritoryMapType::Server == mapType)
	{
		if (_uniqueIDManager.IsValid())
		{
			_uniqueIDManager->OnPostSaveUniqueTable(0, world, true);
		}
	}

	if (_dataManager.IsValid())
	{
		if (false == _dataManager->Save(0, world, true, true))
		{
			return false;
		}
	}

	return true;
}

FGsTerritoryUniqueIDManager* FTerritoryEditor::GetUniqueIDManager()
{
	if (_uniqueIDManager.IsValid())
	{
		return _uniqueIDManager.Get();
	}

	return nullptr;
}

FGsTerritoryLevelManager* FTerritoryEditor::GetLevelManager()
{
	if (_levelManager.IsValid())
	{
		return _levelManager.Get();
	}

	return nullptr;
}

FGsTerritoryDataManager* FTerritoryEditor::GetDataManager()
{
	if (_dataManager.IsValid())
	{
		return _dataManager.Get();
	}

	return nullptr;
}

FGsTerrEdModeStateManager* FTerritoryEditor::GetModeStateManager()
{
	if (_modeStateManager.IsValid())
	{
		return _modeStateManager.Get();
	}

	return nullptr;
}

FGsTerrEdEditorModeState* FTerritoryEditor::GetEditorModeState()
{
	if (_modeStateManager.IsValid())
	{
		TWeakPtr<FGsTerrEdModeStateBase> state = _modeStateManager.Get()->GetState(TERR_ED_EDITOR_MODE);
		if (state.IsValid())
		{
			FGsTerrEdModeStateBase* mode = state.Pin().Get();
			if (mode)
			{
				return static_cast<FGsTerrEdEditorModeState*>(mode);
			}
		}
	}

	return nullptr;
}

FGsTerrEdEditorModeLevelStateBase* FTerritoryEditor::GetEditorLevelEdModeState()
{
	FGsTerrEdEditorModeState* editorMode = GetEditorModeState();
	if (editorMode)
	{
		TWeakPtr<FGsTerrEdModeStateBase> workStatePtr = editorMode->GetState(TERR_ED_EDITOR_MODE_LEVEL_EDIT_MODE);
		if (workStatePtr.IsValid())
		{
			FGsTerrEdModeStateBase* state = workStatePtr.Pin().Get();
			if (state)
			{
				return static_cast<FGsTerrEdEditorModeLevelStateBase*>(state);
			}
		}
	}

	return nullptr;
}

