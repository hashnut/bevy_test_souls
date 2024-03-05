#include "GsTerrEdEditorModeState.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelCopyModeState.h"
#include "EditorMode/GsTerrEdEditorModeLevelCopyModeState.h"
#include "EditorMode/GsTerrEdEditorModeLevelEdModeState.h"
#include "EditorMode/GsTerrEdEditorModeServerLevelNavigationModeState.h"
#include "EditorMode/GsTerrEdEditorModeLevelNavigationModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdModeDefine.h"
#include "TerritoryEditor.h"
#include "EditorMode/GsTerrEdEditorModeDesignLevelNavigationModeState.h"

FGsTerrEdEditorModeState::FGsTerrEdEditorModeState()
{
	AddState(TERR_ED_EDITOR_MODE_LEVEL_EDIT_MODE, MakeShareable(new FGsTerrEdEditorModeLevelEdModeState()));
	AddState(TERR_ED_EDITOR_MODE_LEVEL_COPY_MODE, MakeShareable(new FGsTerrEdEditorModeLevelCopyModeState()));
	AddState(TERR_ED_EDITOR_MODE_SERVER_LEVEL_NAVIGATION_WORK_MODE, MakeShareable(new FGsTerrEdEditorModeServerLevelNavigationModeState()));
	AddState(TERR_ED_EDITOR_MODE_DESIGN_LEVEL_NAVIGATION_WORK_MODE, MakeShareable(new FGsTerrEdEditorModeDesignLevelNavigationModeState()));
	AddState(TERR_ED_EDITOR_MODE_LEVEL_NAVIGATION_WORK_MODE, MakeShareable(new FGsTerrEdEditorModeLevelNavigationModeState()));	
}

FGsTerrEdEditorModeState::~FGsTerrEdEditorModeState()
{
	
}

void FGsTerrEdEditorModeState::Initialize()
{

}

void FGsTerrEdEditorModeState::Enter()
{
	FTerritoryEditor* editor = FTerritoryEditor::GetInstance();
	if (editor)
	{
		editor->BindMapChange();
		editor->BindMapOpen();
		editor->BindNewCurrentLevel();
	}

	ChangeState(TERR_ED_EDITOR_MODE_LEVEL_EDIT_MODE);
}

void FGsTerrEdEditorModeState::Exit()
{

}

void FGsTerrEdEditorModeState::Tick(float inTick)
{	
	if (_currentState.IsValid())
	{
		_currentState.Pin().Get()->Tick(inTick);
	}
}
