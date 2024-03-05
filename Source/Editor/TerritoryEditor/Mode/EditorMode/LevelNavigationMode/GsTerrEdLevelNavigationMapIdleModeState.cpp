#include "GsTerrEdLevelNavigationMapIdleModeState.h"
#include "TerritoryEditor.h"
#include "../../GsTerrEdModeStateBase.h"
#include "Mode/EditorMode/GsTerrEdEditorModeLevelNavigationModeState.h"

FGsTerrEdLevelNavigationMapIdleModeState::FGsTerrEdLevelNavigationMapIdleModeState()
{

}

FGsTerrEdLevelNavigationMapIdleModeState::~FGsTerrEdLevelNavigationMapIdleModeState()
{

}

//맵 오픈 
void FGsTerrEdLevelNavigationMapIdleModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapIdleModeState::Enter"));
#endif

	if (_parents->_currentAreaDataIndex >= _parents->_areaDataNum)
	{
		_parents->FinishNavigationLevel();
		return;
	}

	if (false == _parents->_areaDataArray.IsValidIndex(_parents->_currentAreaDataIndex))
	{
		_parents->FinishNavigationLevel();
		return;
	}

	const FGsSchemaAreaData* areaData = _parents->_areaDataArray[_parents->_currentAreaDataIndex];
	if (areaData)
	{
		_parents->_currentAreaName = areaData->levelName;
		//맵 Change 호출
		if (false == _parents->StartLoadLevel(areaData->levelName))
		{
			_parents->FinishNavigationLevel();
		}
	}	
}

void FGsTerrEdLevelNavigationMapIdleModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapIdleModeState::Exit"));
#endif
}