#include "GsTerrEdLevelCopyMapOpenModeState.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/Util/GsTerritoryLevelUtil.h"
#include "Mode/GsTerrEdModeDefine.h"
#include "Mode/GsTerrEdModeStateManagerBase.h"
#include "Mode/GsTerrEdEditorModeState.h"
#include "Mode/GsTerrEdModeStateManager.h"

FGsTerrEdLevelCopyMapOpenModeState::FGsTerrEdLevelCopyMapOpenModeState()
{

}

FGsTerrEdLevelCopyMapOpenModeState::~FGsTerrEdLevelCopyMapOpenModeState()
{

}

void FGsTerrEdLevelCopyMapOpenModeState::Enter()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelCopyMapOpenModeState::Enter"));

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
		//Copy fail, chagne level work mode
		instance->ChangeLevelEditMode();
		return;
	}

	if (instance)
	{
		instance->_levelEvent.BindNpcTable();
		instance->_levelEvent.BindTerritoryActorTransform();
		//instance->_levelManager->LoadArtLevelsInEditorTime();
		instance->GetUniqueIDManager()->Initialize();
		instance->BindNewCurrentLevel();
		instance->BindPostSave();
		instance->BindActorAdded();
		instance->BindActorDeleted();
	}	

	//load art level
	UWorld* world = GEditor->GetEditorWorldContext().World();
	TWeakPtr<FLevelCollectionModel> levelCollection;
	TArray<TSharedPtr<FLevelModel>> levelList;
	if (false == UGsTerritoryLevelUtil::TryGetWorkLevels(levelCollection, levelList, world, EGsTerritoryLevelType::Server))
	{
		//Copy fail, chagne level work mode	
		instance->ChangeLevelEditMode();
		return;
	}

	FGsTerritoryLevelManager* levelManager = FTerritoryEditor::GetInstance()->GetLevelManager();
	if (nullptr == levelManager)
	{
		instance->ChangeLevelEditMode();
		return;
	}

	int32 currentLevelId = FTerritoryEditor::GetInstance()->GetLevelManager()->GetCurrentLevelId();
	bool isFind = false;
	for (TSharedPtr<FLevelModel> level : levelList)
	{
		const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(level->GetPackageFileName()));
		FString persistantLevel;
		FString levelKey;
		if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevel, levelKey))
		{
			continue;
		}

		if (FCString::Atoi(*levelKey) == currentLevelId)
		{
			isFind = true;

			UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelManager::OnOpenLevel - load streaming level start - level key : %d"), currentLevelId);

			TArray<TSharedPtr<FLevelModel>> loadLevelList;
			loadLevelList.Add(level);
			levelManager->SetLoadLevelList(loadLevelList);
			level->MakeLevelCurrent();

			break;
		}
	}

	if (false == isFind)
	{
		//Copy fail, chagne level work mode
		instance->ChangeLevelEditMode();
	}
}

void FGsTerrEdLevelCopyMapOpenModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelCopyMapOpenModeState::Exit"));
}

