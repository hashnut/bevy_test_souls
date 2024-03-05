#include "GsTerrEdServerLevelNavigationNewCurrentLevelModeState.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/GsTerritoryUniqueIDManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Mode/GsTerrEdModeDefine.h"
#include "Mode/EditorMode/GsTerrEdEditorModeServerLevelNavigationModeState.h"

FGsTerrEdServerLevelNavigationNewCurrentLevelModeState::FGsTerrEdServerLevelNavigationNewCurrentLevelModeState()
{

}

FGsTerrEdServerLevelNavigationNewCurrentLevelModeState::~FGsTerrEdServerLevelNavigationNewCurrentLevelModeState()
{

}

void FGsTerrEdServerLevelNavigationNewCurrentLevelModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationNewCurrentLevelModeState::Enter"));
#endif

	_time = 0;
	_isTick = true;
}

void FGsTerrEdServerLevelNavigationNewCurrentLevelModeState::Tick(float inTick)
{
	if (false == _isTick)
	{
		return;
	}

	_time += inTick;

	if (_time > 0.2)
	{
		_isTick = false;
		_parents->SaveServerStreamingLevel();
		return;
	}
}

void FGsTerrEdServerLevelNavigationNewCurrentLevelModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationNewCurrentLevelModeState::Exit"));
}