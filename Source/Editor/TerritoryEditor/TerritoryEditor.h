
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define INVALID_ZONE_SHAPE true
#define INVALID_WIDGET true
#define INVALID_SHAPE true
#define TERR_ED_WIDGET_COMP true
#define TERRITORY_EIDTOR_CRASH false

#include "CoreMinimal.h"

//ue4
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "Engine/World.h"
#include "UObject/ReflectedTypeAccessors.h"
#include "Stats.h"

//territory editor
#include "GsTerritoryUniqueIDManager.h"
#include "GsTerritoryLevelManager.h"
#include "Data/GsTerritoryDataManager.h"
#include "Editor/GsTerritoryLevelEvent.h"
#include "IDataTableEditor.h"
#include <functional>
#include "Private/Tiles/WorldTileCollectionModel.h"

using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(LogTerritoryEditor, All, All)
DECLARE_MULTICAST_DELEGATE_OneParam(TerritoryEditorTickEvent, float)
DECLARE_MULTICAST_DELEGATE(TerrEdClearFinderEvent)
DECLARE_STATS_GROUP(TEXT("ServerLevelCopy"), STATGROUP_ServerLevelCopy, STATCAT_Advanced);
DECLARE_MEMORY_STAT_EXTERN(TEXT("Server level copy"), STAT_ServerLevelCopy, STATGROUP_ServerLevelCopy, TERRITORYEDITOR_API)

class FGsDataTableEditor;
class FGsTerritoryEditorTool;
class FGsTerrEdModeStateManager;
class FGsTerrEdEditorModeState;
class FGsTerrEdEditorModeLevelStateBase;

class TERRITORYEDITOR_API FTerritoryEditor : public IModuleInterface
{
public:
	static bool _isActive;

private:
	static FTerritoryEditor* _instance;	
	FTickerDelegate _tickDelegate;
	FDelegateHandle _tickDelegateHandle;

public:
	TerritoryEditorTickEvent _territoryEditorTickEvent;
	static TerrEdClearFinderEvent _territoryEditorClearFinderEvent;
	
private:
	TSharedPtr<FGsTerritoryUniqueIDManager> _uniqueIDManager;
	TSharedPtr<FGsTerritoryLevelManager> _levelManager;
	TSharedPtr<FGsTerritoryDataManager> _dataManager;
	TSharedPtr<FGsTerrEdModeStateManager> _modeStateManager;

	//toolbar 
private:
	TSharedPtr<FExtender> _toolbarExtender;
	//TSharedPtr<FGsTerritoryEditorHelper> _editorHelper;

private:	
	FDelegateHandle _mapOpenedHandle;
	FDelegateHandle _mapChangedHandle;
	FDelegateHandle _postSaveWorldHandle;
	FDelegateHandle _newActorDroppedHandle;

	FDelegateHandle _postPIEStartedHandle;
	FDelegateHandle _endPIEHandle;

private:
	FDelegateHandle _levelActorAddedHandle;
	FDelegateHandle _levelActorDeletedHandle;

public:
	FGsTerritoryLevelEvent _levelEvent;

private:	
	FDelegateHandle _newLevelLoadedHandle;
	FDelegateHandle _postSavedHandle;

private:
	TSharedPtr<FGsTerritoryEditorTool> _tool;

private:
	float _time;
	float _duration;

public:
	virtual void StartupModule() override;	
	virtual void ShutdownModule() override;	

private:
	void InitManagers();
	void ClearManagers();
	bool Tick(float inDelta);

private:
	void BindEditorEvents();
	void UnbindEditorEvents();
	void BindRuntimeEvents();
	void UnbindRuntimeEvents();

	//setting
private:
	bool RegisterSetting();
	bool UnregisterSetting();
	bool OnModifySetting();

	//Commands
private:
	void RegisterCommands();
	void UnregisterCommands();

	//Layout
private:
	void RegisterCustomLayoutForBlutility();
	void UnregisterCustomLayoutForBlutility();

	template<typename StructClass, typename LayoutClass>
	void RegisterCustomPropertyTypeLayout()
	{
		FName name = StaticStruct<StructClass>()->GetFName();

		static FName PropertyEditor("PropertyEditor");
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
		PropertyModule.RegisterCustomPropertyTypeLayout(name, FOnGetPropertyTypeCustomizationInstance::CreateStatic(&LayoutClass::MakeInstance));
	}
	template<typename StructClass>
	void UnregisterCustomPropertyTypeLayout()
	{
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.UnregisterCustomPropertyTypeLayout(StaticStruct<StructClass>()->GetFName());
		}
	}

	//runtime event
private:
	//Don't load server streaming level in gameplay time	
	void OnEndPlayInEditor(bool inIsSimulating);
	
	//editor time event
private:
	//map
	void OnMapOpend(const FString& inFilename, bool inIsTemplate);
	void OnMapChange(uint32 inFalg);

	//adctor
	void OnActorAdded(AActor* inActor);
	void OnActorDeleted(AActor* inActor);		
	void OnPlayInEditor(bool inIsSimulating);	

public:
	void BindMapOpen();
	void UnbindMapOpen();
	void BindMapChange();
	void UnbindMapChange();
	void BindNewCurrentLevel();
	void UnbindNewCurrentLevel();
	void BindPostSave();
	void UnbindPostSave();
	void BindActorAdded();
	void UnbindActorAdded();
	void BindActorDeleted();
	void UnbindActorDeleted();	

private:
	//level
	void OnNewCurrentLevel();	

	//save
	void OnPostSave(uint32 in_saveFlag, UWorld* in_world, bool in_success);

	//table event
private:	
	void OnOpenSpawnDataTable(FString inTableName, IDataTableEditor* inEditor);
	void OnSelectedSpawnDataTable(FString inDataTableName, FName inRowName);
	void OnDoubleClickedSpawnDataTable(FString inDataTableName, FName inRowName);
	bool IsMatchedTableToLevel(const FString& inTableName, const FString& inTargetTableName, FString& outLevelKey);
	
private:
	void RegisterTerritoryToolbar();	
	void UnregisterTerritoryToolbar();	
	void ExtendToolbar(FToolBarBuilder& ToolbarBuilder);
	void OnOpenTerritoryEditorTool();
	static bool OnCheckServerLevelRegistered(const FString& inServerLevel);
	static bool OnUnloadPreLoadedServerLevel();

	///Mode manager
public:
	FGsTerrEdModeStateManager* GetModeStateManager();
	FGsTerrEdEditorModeState* GetEditorModeState();	
	FGsTerrEdEditorModeLevelStateBase* GetEditorLevelEdModeState();

public:
	void ChangeLevelEditMode();
	void ChangeLevelEdNewCurrentLevelMode();
	void ChangeLevelCopyMode();		
	bool SaveForce();

public:	
	FGsTerritoryUniqueIDManager* GetUniqueIDManager();	
	FGsTerritoryLevelManager* GetLevelManager();
	FGsTerritoryDataManager* GetDataManager();	
	static FTerritoryEditor* GetInstance(){return _instance;}	
};