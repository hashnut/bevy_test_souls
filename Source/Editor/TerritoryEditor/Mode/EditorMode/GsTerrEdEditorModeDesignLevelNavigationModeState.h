
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateManagerBase.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeDesignLevelNavigationModeState : public FGsTerrEdEditorModeLevelStateBase, public FGsTerrEdModeStateManagerBase<FGsTerrEdModeStateBase>
{
public:
	FGsTerrEdEditorModeDesignLevelNavigationModeState();
	virtual ~FGsTerrEdEditorModeDesignLevelNavigationModeState();

public:
	int32 _currentLevelId = 0;
	int32 _currentLevelIndex = 0;
	FString _currentLevelName;
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
	void FinishNavigationDesignLevel();
	void FinishNavigationCurrentDesignLevel();
	bool OpenLevel(const FString& inLevelName);
	void LoadDesignStreamingLevel();
	void SaveDesignStreamingLevel();

private:
	void SaveDesignLevelInternal();

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
	int32 GetCurrentLevelId() { return _currentLevelId; }
	int32 GetCurrentLevelIndex() { return _currentLevelIndex; }
	FString GetCurrentLevelName() { return _currentLevelName; }
	void SetCurrentLevelName(const FString& inDesignLevel);
};