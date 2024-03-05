#include "GsTerrEdServerLevelNavigationMapIdleModeState.h"
#include "TerritoryEditor.h"
#include "Mode/EditorMode/GsTerrEdEditorModeServerLevelNavigationModeState.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "Util/GsTerritoryLevelUtil.h"

FGsTerrEdServerLevelNavigationMapIdleModeState::FGsTerrEdServerLevelNavigationMapIdleModeState()
{

}

FGsTerrEdServerLevelNavigationMapIdleModeState::~FGsTerrEdServerLevelNavigationMapIdleModeState()
{

}

void FGsTerrEdServerLevelNavigationMapIdleModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapChangeModeState::Enter"));
#endif

	if (_parents->_currentServerLevelIndex >= _parents->_mapDataNum)
	{
		_parents->FinishNavigationServerLevel();
		return;
	}

	if (false == _parents->_mapDataArray.IsValidIndex(_parents->_currentServerLevelIndex))
	{
		_parents->FinishNavigationServerLevel();
		return;
	}

	const FGsSchemaMapData* mapData = _parents->_mapDataArray[_parents->_currentServerLevelIndex];
	if (nullptr == mapData)
	{
		_parents->FinishNavigationServerLevel();
		return;
	}

	FString levelName = UGsTerritoryLevelUtil::GetLevelName(mapData->id);
	if (levelName.Equals(""))
	{
		_parents->FinishNavigationCurrentServerLevel();
		return;
	}
		
	_parents->_currentServerLevelName = levelName;
	_parents->_currentServerLevelId = mapData->id;
	//맵 Change 호출
	if (false == _parents->OpenLevel(levelName))
	{
		_parents->FinishNavigationServerLevel();	
	}
}

void FGsTerrEdServerLevelNavigationMapIdleModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapChangeModeState::Exit"));
#endif
}