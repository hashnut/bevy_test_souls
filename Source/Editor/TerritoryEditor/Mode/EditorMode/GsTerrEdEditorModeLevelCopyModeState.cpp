#include "GsTerrEdEditorModeLevelCopyModeState.h"
#include "TerritoryEditor/Mode/EditorMode/LevelCopyMode/GsTerrEdLevelCopyMapChangeModeState.h"
#include "TerritoryEditor/Mode/EditorMode/LevelCopyMode/GsTerrEdLevelCopyMapOpenModeState.h"
#include "TerritoryEditor/Mode/EditorMode/LevelCopyMode/GsTerrEdLevelCopyNewCurrentLevelModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdModeDefine.h"

FGsTerrEdEditorModeLevelCopyModeState::FGsTerrEdEditorModeLevelCopyModeState()
{	
	AddState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE, MakeShareable(new FGsTerrEdLevelCopyMapChangeModeState()));
	AddState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE, MakeShareable(new FGsTerrEdLevelCopyMapOpenModeState()));
	AddState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE, MakeShareable(new FGsTerrEdLevelCopyNewCurrentLevelModeState()));
}

FGsTerrEdEditorModeLevelCopyModeState::~FGsTerrEdEditorModeLevelCopyModeState()
{

}

void FGsTerrEdEditorModeLevelCopyModeState::Enter()
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE);
}

void FGsTerrEdEditorModeLevelCopyModeState::Exit()
{
	FGsTerrEdModeStateBase* currentState = GetCurrentState();
	if (currentState)
	{
		currentState->Exit();
	}

	_currentState.Reset();
	_currentStateName = TEXT("");

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (instance)
	{
		instance->_levelEvent.BindNpcTable();
		instance->_levelEvent.BindTerritoryActorTransform();
		//instance->_levelManager->LoadArtLevelsInEditorTime();
		instance->GetUniqueIDManager()->Initialize();
		instance->BindNewCurrentLevel();
		instance->BindPostSave();
		instance->BindActorAdded();
		instance->BindActorDeleted();
	}
}

void FGsTerrEdEditorModeLevelCopyModeState::MapChange()
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_CHANGE_MODE);
}

void FGsTerrEdEditorModeLevelCopyModeState::MapOpened(const FString& inFilename)
{
	ChangeState(TERR_ED_EDITOR_MODE_MAP_OPEN_MODE);
}

void FGsTerrEdEditorModeLevelCopyModeState::NewCurrentLevel()
{
	ChangeState(TERR_ED_EDITOR_MODE_NEW_CURRENT_LEVEL_MODE);
}
