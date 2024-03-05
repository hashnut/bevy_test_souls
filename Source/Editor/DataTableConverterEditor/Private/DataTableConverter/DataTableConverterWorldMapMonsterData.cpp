


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"
#include "Editor/TerritoryEditor/Data/GsTerritoryDataManager.h"
#include "Editor/TerritoryEditor/TerritoryEditor.h"

/**
 * 기작업된 복잡도 있는 멀티키 구조를 지원하기 위해 1차 테이블 스키마(엑셀 테이블 2차원 구조)로 Import후에
 * 실제 Tech내에서 활용하는 3차원 구조(예전 구조)로 convert을 합니다.
 */

// 원본 데이터테이블의 Package Name
const FString WORLDMAPMONSTER_PATH(TEXT("/Game/DataCenter/DataTable/Map/WorldMap/DTGsWorldMapMonsterData"));

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterWorldMapMonsterData)
	bool FDataTableConverterWorldMapMonsterData::Convert()
{
	// 원본 데이터테이블 로드
	UGsDataTable* OriginalDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(WORLDMAPMONSTER_PATH, OriginalDataTable))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	OriginalDataTable->EmptyTable();

	if (FTerritoryEditor* instance = FModuleManager::GetModulePtr<FTerritoryEditor>("TerritoryEditor"))
	{
		FGsTerritoryDataManager* manager = instance->GetDataManager();
		if (manager)
		{
			TWeakPtr<FGsTerritoryDataBase> worldMapMonster = manager->GetWorldMapMonsterData();
			if (worldMapMonster.IsValid())
			{
				worldMapMonster.Pin()->Save(0, nullptr, true, nullptr, "");
			}
		}
	}

	return true;
}

