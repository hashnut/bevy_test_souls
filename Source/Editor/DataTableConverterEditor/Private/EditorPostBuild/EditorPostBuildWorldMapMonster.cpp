#include "AutomationTest.h"
#include "AssetRegistryModule.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Engine/Classes/Engine/PrimaryAssetLabel.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"
#include "Runtime/Public/DataTable/GsDataTable.h"
#include "../../../TerritoryEditor/TerritoryEditor.h"
#include "Modules/ModuleManager.h"
#include "../../../TerritoryEditor/Data/GsTerritoryDataManager.h"
#include "RSQLEditor/Public/DataTableConverter/GsDataTableConvertBase.h"

/**
 * 에디터 포스트빌드 테스트 샘플
 * 
 * 포스트빌드에 자동 포함하기 위한 유일한 조건은 테스트 이름이 "Raon.EditorPostBuild" 로 시작하는 것입니다.
 * 아래 코드를 참고하세요.
 * 
 * 포스트빌드는 보통 CI에서 클라이언트 빌드가 성공하면, 본 테스트를 모두 실행합니다.
 * 이후에 변경된 사항들을 SCM 으로 자동 커밋합니다.
 * 
 * 이 테스트를 로컬에서 실행하고자 하면, 아래 스크립트를 참조하세요.
 * UE4Editor-Cmd.exe T1Project.uproject -NoP4 -Run -StdOut -NullRHI -Unattended  -NoPause -UTF8Output -NoXge -ExecCmds="Automation RunTests Raon.EditorPostBuild; Quit"
 * 
 * 또한, 본 포스트빌드는 테스트 프로세스의 일부이므로, Unreal Frontend 에서 원하는 작업을 직접 선택하여 실행할 수 있습니다.
 * 
 * 참고로, 테스트 프로세스는 에디터에만 가능한 것은 아닙니다. 당연히 런타임에도 테스트 프로세스를 실행할 수 있으나, 현재 본 샘플은 에디터 전용 포스트빌드에 한 합니다.
 */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EditorPostBuildWorldMapMonster, TEXT("Raon.EditorPostBuild.3 WorldMapMonster")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)

bool EditorPostBuildWorldMapMonster::RunTest(const FString& Parameters)
{
	const FString worldMapPath = TEXT("/Game/DataCenter/DataTable/Map/WorldMap/DTGsWorldMapMonsterData");
	const FString monsterKnowledgeMapPath = TEXT("GsDataTable'/Game/DataCenter/DataTable/MonsterKnowledgeCollection/DTGsMonsterKnowledgeMapData.DTGsMonsterKnowledgeMapData'");

	// 원본 데이터테이블 로드
	UGsDataTable* worldMapDataTable = nullptr;
	if (false == FGsDataTableConvertBase::LoadDataTable(worldMapPath, worldMapDataTable))
	{
		return false;
	}

	// 원본 데이터테이블 로드
	UGsDataTable* monsterKnowledgeMapDataTable = nullptr;
	if (false == FGsDataTableConvertBase::LoadDataTable(monsterKnowledgeMapPath, monsterKnowledgeMapDataTable))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	worldMapDataTable->EmptyTable();
	if (FTerritoryEditor* instance = FModuleManager::GetModulePtr<FTerritoryEditor>("TerritoryEditor"))
	{
		FGsTerritoryDataManager* manager = instance->GetDataManager();
		if (manager)
		{
			TWeakPtr<FGsTerritoryDataBase> worldMapMonster = manager->GetWorldMapMonsterData();
			if (worldMapMonster.IsValid())
			{
				worldMapMonster.Pin()->Save(0, nullptr, true, nullptr, "", true);
			}
		}
	}

	// 원본 데이터테이블 비우기
	monsterKnowledgeMapDataTable->EmptyTable();
	if (FTerritoryEditor* instance = FModuleManager::GetModulePtr<FTerritoryEditor>("TerritoryEditor"))
	{
		FGsTerritoryDataManager* manager = instance->GetDataManager();
		if (manager)
		{
			TWeakPtr<FGsTerritoryDataBase> monsterKnowledgeMapMonster = manager->GetMonsterKnowledgeMapData();
			if (monsterKnowledgeMapMonster.IsValid())
			{
				monsterKnowledgeMapMonster.Pin()->Save(0, nullptr, true, nullptr, "", true);
			}
		}
	}

	return true;
}