#include "GsTerrEdLevelNavigationMapChangeModeState.h"
#include "TerritoryEditor.h"
#include "../../GsTerrEdModeStateBase.h"
#include "Mode/EditorMode/GsTerrEdEditorModeLevelNavigationModeState.h"

FGsTerrEdLevelNavigationMapChangeModeState::FGsTerrEdLevelNavigationMapChangeModeState()
{

}

FGsTerrEdLevelNavigationMapChangeModeState::~FGsTerrEdLevelNavigationMapChangeModeState()
{

}

//맵 오픈 
void FGsTerrEdLevelNavigationMapChangeModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapChangeModeState::Enter"));
#endif

}

void FGsTerrEdLevelNavigationMapChangeModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapChangeModeState::Exit"));
#endif
}