#include "GsTerrEdDesignLevelNavigationNewCurrentLevelModeState.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/GsTerritoryUniqueIDManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Mode/GsTerrEdModeDefine.h"
#include "Mode/EditorMode/GsTerrEdEditorModeDesignLevelNavigationModeState.h"

FGsTerrEdDesignLevelNavigationNewCurrentLevelModeState::FGsTerrEdDesignLevelNavigationNewCurrentLevelModeState()
{

}

FGsTerrEdDesignLevelNavigationNewCurrentLevelModeState::~FGsTerrEdDesignLevelNavigationNewCurrentLevelModeState()
{

}

void FGsTerrEdDesignLevelNavigationNewCurrentLevelModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationNewCurrentLevelModeState::Enter"));
#endif

	_time = 0;
	_isTick = true;
}

void FGsTerrEdDesignLevelNavigationNewCurrentLevelModeState::Tick(float inTick)
{
	if (false == _isTick)
	{
		return;
	}

	_time += inTick;

	if (_time > 0.2)
	{
		_isTick = false;
		_parents->SaveDesignStreamingLevel();
		return;
	}
}

void FGsTerrEdDesignLevelNavigationNewCurrentLevelModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationNewCurrentLevelModeState::Exit"));
}