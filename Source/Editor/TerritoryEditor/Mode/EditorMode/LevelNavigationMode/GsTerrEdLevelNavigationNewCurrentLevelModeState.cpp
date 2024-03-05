#include "GsTerrEdLevelNavigationNewCurrentLevelModeState.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/GsTerritoryUniqueIDManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "../GsTerrEdEditorModeLevelNavigationModeState.h"
#include "../../GsTerrEdModeDefine.h"

FGsTerrEdLevelNavigationNewCurrentLevelModeState::FGsTerrEdLevelNavigationNewCurrentLevelModeState()
{

}

FGsTerrEdLevelNavigationNewCurrentLevelModeState::~FGsTerrEdLevelNavigationNewCurrentLevelModeState()
{

}

/// <summary>
/// 맵 저장 후 레벨 선택하면 레벨 저장이 끝났다는 것임
/// </summary>
void FGsTerrEdLevelNavigationNewCurrentLevelModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationNewCurrentLevelModeState::Enter"));
#endif

	_time = 0;
	_isTick = true;


}

void FGsTerrEdLevelNavigationNewCurrentLevelModeState::Tick(float inTick)
{
	if (false == _isTick)
	{
		return;
	}

	_time += inTick;

	if (_time > 1)
	{
		_parents->ChangeState(TERR_ED_EDITOR_MODE_MAP_IDLE_MODE);
		_isTick = false;
		return;
	}
}

void FGsTerrEdLevelNavigationNewCurrentLevelModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationNewCurrentLevelModeState::Exit"));
#endif
}