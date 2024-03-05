#include "GsTerrEdLevelNavigationMapOpenModeState.h"
#include "TerritoryEditor.h"
#include "GsTerritoryLevelManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "../GsTerrEdEditorModeLevelNavigationModeState.h"

FGsTerrEdLevelNavigationMapOpenModeState::FGsTerrEdLevelNavigationMapOpenModeState()
{

}

FGsTerrEdLevelNavigationMapOpenModeState::~FGsTerrEdLevelNavigationMapOpenModeState()
{

}

/// <summary>
/// 맵 오픈 후 아트 레벨 로드
/// </summary>
void FGsTerrEdLevelNavigationMapOpenModeState::Enter()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapOpenModeState::Enter"));
#endif

	int32 currentLevelIndex = _parents->GetCurrentAreaIndex();
	FString currentAreaName = _parents->GetCurrentAreaName();
	
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapOpenModeState::Enter - load artlevel - level index : %d\tlevel name : %s"), currentLevelIndex, *currentAreaName);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	UGsTerritoryLevelUtil::LoadWorkLevels(world, EGsTerritoryLevelType::Art);

	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> levelList;
	if (false == UGsTerritoryLevelUtil::TryGetWorkLevels(levelCollection, levelList, world, EGsTerritoryLevelType::Art))
	{
		//만약 서버레벨이 하나도 없다면 		
		return;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapOpenModeState::Enter  - load finish artlevel - level index : %d\tlevel name : %s"), currentLevelIndex, *currentAreaName);

	_parents->SetLoadedArtLevel(levelList);	

	_time = 0;
	_isTick = true;
}

/// <summary>
/// 맵 오픈 후 대기 후 저장 시작 
/// </summary>
void FGsTerrEdLevelNavigationMapOpenModeState::Tick(float inTick)
{
	if (false == _isTick)
	{
		return;
	}

	_time += inTick;

	if (_time > 1)
	{
		_parents->ExcuteEvent();
		_parents->SaveLevel();
		_isTick = false;
		return;
	}
}

void FGsTerrEdLevelNavigationMapOpenModeState::Exit()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelNavigationMapOpenModeState::Exit"));
#endif
}