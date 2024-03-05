#include "GsTerrEdPlayModeState.h"
#include "TerritoryEditor.h"

void FGsTerrEdPlayModeState::Enter()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdPlayModeState::Enter"));

	FTerritoryEditor* editor = FTerritoryEditor::GetInstance();
	if (editor)
	{
		editor->UnbindPostSave();
		editor->UnbindMapOpen();
		editor->UnbindMapChange();		
		editor->UnbindActorAdded();
		editor->UnbindActorDeleted();
	}	
}

void FGsTerrEdPlayModeState::Exit()
{

}