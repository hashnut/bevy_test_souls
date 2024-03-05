
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateManagerBase.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeServerLevelNavigationModeState : public FGsTerrEdEditorModeLevelStateBase, public FGsTerrEdModeStateManagerBase<FGsTerrEdModeStateBase>
{
public:
	FGsTerrEdEditorModeServerLevelNavigationModeState();
	virtual ~FGsTerrEdEditorModeServerLevelNavigationModeState();

public:
	int32 _currentServerLevelId = 0;
	int32 _currentServerLevelIndex = 0;
	FString _currentServerLevelName;
	int32 _mapDataNum = 0;

public:
	FDelegateHandle _mapOpenedHandle;
	FDelegateHandle _mapChangedHandle;
	FDelegateHandle _newLevelLoadedHandle;
	FDelegateHandle _postSaveHandle;
	FDelegateHandle _levelChangeHandle;
	
public:
	TArray<const FGsSchemaMapData*> _mapDataArray;	
	TArray<TSharedPtr<FLevelModel>> _loadLevelList;

public:
	FDelegateHandle _additionalEventHandle;
	FTerritoryEditorAdditionalLevelDelegate _additionalEvent;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(float inTick) override;

public:
	virtual void MapChange() override;
	virtual void MapOpened(const FString& inFilename) override;
	virtual void NewCurrentLevel() override;
	virtual void PostSave() override;

public:	
	void FinishNavigationServerLevel();
	void FinishNavigationCurrentServerLevel();
	bool OpenLevel(const FString& inLevelName);
	void LoadServerStreamingLevel();
	void SaveServerStreamingLevel();

private:
	void SaveServerLevelInternal();

public:
	void OnMapOpend(const FString& inFilename, bool inIsTemplate);
	void OnMapChange(uint32 inFalg);
	void OnNewCurrentLevel();
	void OnPostSaveLevel(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess);
	void OnLevelChange();

public:
	void ExcuteEvent();
	void OnTest();

public:
	void SaveLevel();

public:
	int32 GetCurrentServerLevelId() { return _currentServerLevelId; }
	int32 GetCurrentServerLevelIndex() { return _currentServerLevelIndex; }
	FString GetCurrentServerLevelName() { return _currentServerLevelName; }
	void SetCurrentServerLevelName(const FString& inServerLevel);
};