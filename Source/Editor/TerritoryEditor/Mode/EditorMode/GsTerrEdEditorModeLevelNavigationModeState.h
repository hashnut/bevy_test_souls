
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GsTerrEdEditorModeLevelStateBase.h"
#include "Mode/GsTerrEdModeStateBase.h"
#include "Mode/GsTerrEdModeStateManagerBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeLevelNavigationModeState : public FGsTerrEdEditorModeLevelStateBase, public FGsTerrEdModeStateManagerBase<FGsTerrEdModeStateBase>
{
public:
	FGsTerrEdEditorModeLevelNavigationModeState();
	virtual ~FGsTerrEdEditorModeLevelNavigationModeState();

public:
	int32 _currentAreaId;
	FString _currentAreaName;
	int32 _areaDataNum = 0;
	int32 _currentAreaDataIndex = 0;

public:
	FDelegateHandle _mapOpenedHandle;
	FDelegateHandle _mapChangedHandle;
	FDelegateHandle _newLevelLoadedHandle;
	FDelegateHandle _postSaveHandle;
	FDelegateHandle _levelChangeHandle;

public:
	TArray<const FGsSchemaAreaData*> _areaDataArray;
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

public:
	bool StartLoadLevel(const FString& inName);
	void FinishNavigationLevel();
	void FinishNavigationCurrentLevel();
	bool OpenLevelForSaveAllMap(const FString& inLevelName);

public:
	void OnMapOpend(const FString& inFilename, bool inIsTemplate);
	void OnMapChange(uint32 inFalg);
	void OnNewCurrentLevel();
	void OnPostSaveLevel(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess);
	void OnLevelChange();

public:
	void ExcuteEvent();

public:
	void SaveLevel();	

public:
	int32 GetCurrentAreaId() {return _currentAreaId;}
	int32 GetCurrentAreaIndex() {return _currentAreaDataIndex;}
	FString GetCurrentAreaName() {return _currentAreaName;}
	void SetLoadedArtLevel(TArray<TSharedPtr<FLevelModel>>& inLevelList) { _loadLevelList = inLevelList; }
};