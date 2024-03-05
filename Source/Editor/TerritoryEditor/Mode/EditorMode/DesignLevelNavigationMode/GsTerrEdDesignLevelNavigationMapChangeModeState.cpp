#include "GsTerrEdDesignLevelNavigationMapChangeModeState.h"
#include "TerritoryEditor.h"

FGsTerrEdDesignLevelNavigationMapChangeModeState::FGsTerrEdDesignLevelNavigationMapChangeModeState()
{

}

FGsTerrEdDesignLevelNavigationMapChangeModeState::~FGsTerrEdDesignLevelNavigationMapChangeModeState()
{

}

void FGsTerrEdDesignLevelNavigationMapChangeModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapChangeModeState::Enter"));
#endif
}

void FGsTerrEdDesignLevelNavigationMapChangeModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdDesignLevelNavigationMapChangeModeState::Exit"));
#endif
}