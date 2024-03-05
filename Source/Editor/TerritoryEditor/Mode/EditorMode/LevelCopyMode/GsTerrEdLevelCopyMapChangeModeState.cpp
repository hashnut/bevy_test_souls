#include "GsTerrEdLevelCopyMapChangeModeState.h"
#include "TerritoryEditor.h"

FGsTerrEdLevelCopyMapChangeModeState::FGsTerrEdLevelCopyMapChangeModeState()
{

}

FGsTerrEdLevelCopyMapChangeModeState::~FGsTerrEdLevelCopyMapChangeModeState()
{

}

void FGsTerrEdLevelCopyMapChangeModeState::Enter()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelCopyMapChangeModeState::Enter"));

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (instance)
	{
		instance->_levelEvent.UnbindNpcTable();
		instance->_levelEvent.UnbindTerritoryActorTransform();
		//instance->_levelManager->LoadArtLevelsInEditorTime();
		instance->GetUniqueIDManager()->Clear();
		instance->UnbindNewCurrentLevel();
		instance->UnbindPostSave();
		instance->UnbindActorAdded();
		instance->UnbindActorDeleted();
	}
}

void FGsTerrEdLevelCopyMapChangeModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelCopyMapChangeModeState::Exit"));
}