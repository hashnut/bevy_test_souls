#include "GsTerrEdDesignLevelNavigationMapIdleModeState.h"
#include "TerritoryEditor.h"
#include "Mode/EditorMode/GsTerrEdEditorModeDesignLevelNavigationModeState.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "Util/GsTerritoryLevelUtil.h"

FGsTerrEdDesignLevelNavigationMapIdleModeState::FGsTerrEdDesignLevelNavigationMapIdleModeState()
{

}

FGsTerrEdDesignLevelNavigationMapIdleModeState::~FGsTerrEdDesignLevelNavigationMapIdleModeState()
{

}

void FGsTerrEdDesignLevelNavigationMapIdleModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapIdleModeState::Enter"));
#endif

	if (_parents->_currentLevelIndex >= _parents->_mapDataNum)
	{
		_parents->FinishNavigationDesignLevel();
		return;
	}

	if (false == _parents->_mapDataArray.IsValidIndex(_parents->_currentLevelIndex))
	{
		_parents->FinishNavigationDesignLevel();
		return;
	}

	const FGsSchemaMapData* mapData = _parents->_mapDataArray[_parents->_currentLevelIndex];
	if (nullptr == mapData)
	{
		_parents->FinishNavigationDesignLevel();
		return;
	}

	FString levelName = UGsTerritoryLevelUtil::GetLevelName(mapData->id);
	if (levelName.Equals(""))
	{
		_parents->FinishNavigationCurrentDesignLevel();
		return;
	}
		
	_parents->_currentLevelName = levelName;
	_parents->_currentLevelId = mapData->id;
	//맵 Change 호출
	if (false == _parents->OpenLevel(levelName))
	{
		_parents->FinishNavigationDesignLevel();
	}
}

void FGsTerrEdDesignLevelNavigationMapIdleModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapIdleModeState::Exit"));
#endif
}