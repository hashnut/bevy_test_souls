#include "GsTerrEdEditorModeMapChangeModeState.h"
#include "TerritoryEditor.h"

FGsTerrEdEditorModeMapChangeModeState::FGsTerrEdEditorModeMapChangeModeState()
{

}

FGsTerrEdEditorModeMapChangeModeState::~FGsTerrEdEditorModeMapChangeModeState()
{

}

void FGsTerrEdEditorModeMapChangeModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeMapChangeModeState::Enter"));
#endif

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (instance)
	{
		if (FTerritoryEditor::_isActive)
		{
			instance->_levelEvent.UnbindNpcTable();
			instance->_levelEvent.UnbindTerritoryActorTransform();
		}		
		
		if (FGsTerritoryUniqueIDManager* uniqueManager = instance->GetUniqueIDManager())
		{
			uniqueManager->Clear();
		}
		
		instance->UnbindNewCurrentLevel();
		instance->UnbindPostSave();
		instance->UnbindActorAdded();
		instance->UnbindActorDeleted();
	}
}

void FGsTerrEdEditorModeMapChangeModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeMapChangeModeState::Exit"));
#endif
}