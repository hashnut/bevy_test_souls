#include "GsTerrEdServerLevelNavigationMapChangeModeState.h"
#include "TerritoryEditor.h"

FGsTerrEdServerLevelNavigationMapChangeModeState::FGsTerrEdServerLevelNavigationMapChangeModeState()
{

}

FGsTerrEdServerLevelNavigationMapChangeModeState::~FGsTerrEdServerLevelNavigationMapChangeModeState()
{

}

void FGsTerrEdServerLevelNavigationMapChangeModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapChangeModeState::Enter"));
#endif
}

void FGsTerrEdServerLevelNavigationMapChangeModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdServerLevelNavigationMapChangeModeState::Exit"));
#endif
}