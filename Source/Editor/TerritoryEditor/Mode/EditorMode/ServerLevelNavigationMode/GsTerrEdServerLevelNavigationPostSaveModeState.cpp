#include "GsTerrEdServerLevelNavigationPostSaveModeState.h"
#include "TerritoryEditor.h"
#include "GsTerritoryLevelManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Mode/EditorMode/GsTerrEdEditorModeServerLevelNavigationModeState.h"

FGsTerrEdServerLevelNavigationPostSaveModeState::FGsTerrEdServerLevelNavigationPostSaveModeState()
{

}

FGsTerrEdServerLevelNavigationPostSaveModeState::~FGsTerrEdServerLevelNavigationPostSaveModeState()
{

}

void FGsTerrEdServerLevelNavigationPostSaveModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapOpenModeState::Enter"));
#endif

	int32 currentLevelIndex = _parents->GetCurrentServerLevelId();
	FString currentServerLevelName = _parents->GetCurrentServerLevelName();

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapOpenModeState::Enter - level index : %d\tlevel name : %s"), currentLevelIndex, *currentServerLevelName);

	UWorld* world = GEditor->GetEditorWorldContext().World();		

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapOpenModeState::Enter  - load finish artlevel - level index : %d\tlevel name : %s"), currentLevelIndex, *currentServerLevelName);

	_time = 0;
	_isTick = true;
}

/// <summary>
/// 맵 오픈 후 대기 후 저장 시작 
/// </summary>
void FGsTerrEdServerLevelNavigationPostSaveModeState::Tick(float inTick)
{
	if (false == _isTick)
	{
		return;
	}

	_time += inTick;

	if (_time > 0.2)
	{			
		_isTick = false;
		_parents->FinishNavigationCurrentServerLevel();				
		return;
	}
}

void FGsTerrEdServerLevelNavigationPostSaveModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapOpenModeState::Exit"));
}