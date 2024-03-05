#include "GsTerrEdEditorModeLevelEdModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdModeDefine.h"
#include "TerritoryEditor/Mode/EditorMode/LevelEditMode/GsTerrEdEditorModeMapChangeModeState.h"
#include "TerritoryEditor/Mode/EditorMode/LevelEditMode/GsTerrEdEditorModeMapOpenModeState.h"
#include "TerritoryEditor/Mode/EditorMode/LevelEditMode/GsTerrEdEditorModeNewCurrentLevelModeState.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/GsTerritoryLevelManager.h"

FGsTerrEdEditorModeLevelEdModeState::FGsTerrEdEditorModeLevelEdModeState()
{
	AddState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE, MakeShareable(new FGsTerrEdEditorModeMapChangeModeState()));
	AddState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE, MakeShareable(new FGsTerrEdEditorModeMapOpenModeState()));
	AddState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE, MakeShareable(new FGsTerrEdEditorModeNewCurrentLevelModeState()));
}

FGsTerrEdEditorModeLevelEdModeState::~FGsTerrEdEditorModeLevelEdModeState()
{

}

void FGsTerrEdEditorModeLevelEdModeState::Enter()
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE);
}

void FGsTerrEdEditorModeLevelEdModeState::Exit()
{
	FGsTerrEdModeStateBase* currentState = GetCurrentState();
	if (currentState)
	{
		currentState->Exit();
	}

	_currentState.Reset();
	_currentStateName = TEXT("");
}

void FGsTerrEdEditorModeLevelEdModeState::MapChange()
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE);
}

void FGsTerrEdEditorModeLevelEdModeState::MapOpened(const FString& inFilename)
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE);

	FGsTerritoryLevelManager::LoadArtLevelsInEditorTime(inFilename);
}

//Open server level
void FGsTerrEdEditorModeLevelEdModeState::NewCurrentLevel()
{
	ChangeState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE);
}
