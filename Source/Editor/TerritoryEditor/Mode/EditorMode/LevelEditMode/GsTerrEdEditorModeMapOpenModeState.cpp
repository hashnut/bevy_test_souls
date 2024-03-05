#include "GsTerrEdEditorModeMapOpenModeState.h"
#include "TerritoryEditor.h"

FGsTerrEdEditorModeMapOpenModeState::FGsTerrEdEditorModeMapOpenModeState()
{

}

FGsTerrEdEditorModeMapOpenModeState::~FGsTerrEdEditorModeMapOpenModeState()
{

}

void FGsTerrEdEditorModeMapOpenModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeMapOpenModeState::Enter"));
#endif

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (instance)
	{
		//instance->_levelManager->LoadArtLevelsInEditorTime();
		if (FGsTerritoryUniqueIDManager* uniqueManager = instance->GetUniqueIDManager())
		{
			uniqueManager->Initialize();
		}
		
		instance->BindNewCurrentLevel();
		instance->BindPostSave();
		instance->BindActorAdded();
		instance->BindActorDeleted();
	}
}

void FGsTerrEdEditorModeMapOpenModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeMapOpenModeState::Exit"));
}