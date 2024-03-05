#include "GsTerrEdLevelCopyNewCurrentLevelModeState.h"
#include "TerritoryEditor.h"
#include "TerritoryEditor/GsTerritoryUniqueIDManager.h"
#include "TerritoryEditor/Util/GsTerritoryLevelUtil.h"

FGsTerrEdLevelCopyNewCurrentLevelModeState::FGsTerrEdLevelCopyNewCurrentLevelModeState()
{

}

FGsTerrEdLevelCopyNewCurrentLevelModeState::~FGsTerrEdLevelCopyNewCurrentLevelModeState()
{

}

void FGsTerrEdLevelCopyNewCurrentLevelModeState::Enter()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelCopyNewCurrentLevelModeState::Enter"));
	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
		instance->ChangeLevelEditMode();
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		instance->ChangeLevelEditMode();
		return;
	}

	FGsTerritoryLevelManager* levelManager = FTerritoryEditor::GetInstance()->GetLevelManager();
	if (nullptr == levelManager)
	{
		instance->ChangeLevelEditMode();
		return;
	}

	int32 currentLevelId = levelManager->GetCurrentLevelId();
	ULevel* level = world->GetCurrentLevel();
	FString levelName;
	FString levelKey;	
	if (false == UGsTerritoryLevelUtil::IsValidServerLevel(level, levelName, levelKey)
		&& FCString::Atoi(*levelKey) != currentLevelId)
	{
		instance->ChangeLevelEditMode();
		return;
	}

	//액터 수정을 위해 unique id manager 초기화 
	//이곳에서 unique id manager를 초기화 하는 로직은 자동으로 레벨을 저장할 때만 사용한다. 
	FGsTerritoryUniqueIDManager* uniqueIdManager = FTerritoryEditor::GetInstance()->GetUniqueIDManager();
	if (uniqueIdManager)
	{		
		uniqueIdManager->OnCheckServerLevelLoaded();
	}

	FTerritoryEditor::GetInstance()->BindActorAdded();
	FTerritoryEditor::GetInstance()->BindActorDeleted();

	//액터 복사 시작
	FTerritoryEditor::GetInstance()->GetLevelManager()->ExcuteAdditionalLevelEvent();		

	FTerritoryEditor::GetInstance()->ChangeLevelEditMode();
	FTerritoryEditor::GetInstance()->ChangeLevelEdNewCurrentLevelMode();
}

void FGsTerrEdLevelCopyNewCurrentLevelModeState::Exit()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdLevelCopyNewCurrentLevelModeState::Exit"));
	//FTerritoryEditor::GetInstance()->GetLevelManager()->UnbindAdditionalLevelEvent();	
}