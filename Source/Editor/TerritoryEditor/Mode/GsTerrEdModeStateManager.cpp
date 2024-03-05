#include "GsTerrEdModeStateManager.h"
#include "TerritoryEditor/Mode/GsTerrEdPlayModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdEditorModeState.h"
#include "TerritoryEditor/Mode/GsTerrEdModeDefine.h"

FGsTerrEdModeStateManager::FGsTerrEdModeStateManager()
{
	AddState(TERR_ED_PLAY_MODE, MakeShareable(new FGsTerrEdPlayModeState()));
	AddState(TERR_ED_EDITOR_MODE, MakeShareable(new FGsTerrEdEditorModeState()));
}
