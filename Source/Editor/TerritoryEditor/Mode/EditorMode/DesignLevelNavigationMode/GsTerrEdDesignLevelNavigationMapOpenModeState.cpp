#include "GsTerrEdDesignLevelNavigationMapOpenModeState.h"
#include "TerritoryEditor.h"
#include "GsTerritoryLevelManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Mode/EditorMode/GsTerrEdEditorModeDesignLevelNavigationModeState.h"

FGsTerrEdDesignLevelNavigationMapOpenModeState::FGsTerrEdDesignLevelNavigationMapOpenModeState()
{

}

FGsTerrEdDesignLevelNavigationMapOpenModeState::~FGsTerrEdDesignLevelNavigationMapOpenModeState()
{

}

void FGsTerrEdDesignLevelNavigationMapOpenModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapOpenModeState::Enter"));
#endif

	int32 currentLevelIndex = _parents->GetCurrentLevelId();
	FString currentDesignLevelName = _parents->GetCurrentLevelName();

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapOpenModeState::Enter - level index : %d\tlevel name : %s"), currentLevelIndex, *currentDesignLevelName);

	UWorld* world = GEditor->GetEditorWorldContext().World();		

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapOpenModeState::Enter  - load finish artlevel - level index : %d\tlevel name : %s"), currentLevelIndex, *currentDesignLevelName);

	_time = 0;
	_isTick = true;
}

/// <summary>
/// 맵 오픈 후 대기 후 저장 시작 
/// </summary>
void FGsTerrEdDesignLevelNavigationMapOpenModeState::Tick(float inTick)
{
	if (false == _isTick)
	{
		return;
	}

	_time += inTick;

	if (_time > 0.2)
	{	
		_isTick = false;
		_parents->LoadDesignStreamingLevel();		
		return;
	}
}

void FGsTerrEdDesignLevelNavigationMapOpenModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapOpenModeState::Exit"));
}