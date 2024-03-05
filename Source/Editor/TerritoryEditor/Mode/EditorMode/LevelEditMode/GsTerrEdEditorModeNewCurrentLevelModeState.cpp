#include "GsTerrEdEditorModeNewCurrentLevelModeState.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/GsTerritoryUniqueIDManager.h"

FGsTerrEdEditorModeNewCurrentLevelModeState::FGsTerrEdEditorModeNewCurrentLevelModeState()
{

}

FGsTerrEdEditorModeNewCurrentLevelModeState::~FGsTerrEdEditorModeNewCurrentLevelModeState()
{

}

void FGsTerrEdEditorModeNewCurrentLevelModeState::Enter()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeNewCurrentLevelModeState::Enter"));
	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (instance)
	{
		instance->_levelEvent.BindNpcTable();
		instance->_levelEvent.BindTerritoryActorTransform();		
		instance->GetUniqueIDManager()->OnCheckServerLevelLoaded();
	}	
}

void FGsTerrEdEditorModeNewCurrentLevelModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEditorModeNewCurrentLevelModeState::Exit"));

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (instance)
	{
		if (FTerritoryEditor::_isActive)
		{
			instance->_levelEvent.UnbindNpcTable();
			instance->_levelEvent.UnbindTerritoryActorTransform();
		}
	}	
}