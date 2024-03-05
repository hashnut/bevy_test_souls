


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"

#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropItemGroupExcel.h"
#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropItemGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropItemGroupInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropProbGroupExcel.h"
#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropProbGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropProbGroupInfo.h"

/**
 * 기작업된 복잡도 있는 멀티키 구조를 지원하기 위해 1차 테이블 스키마(엑셀 테이블 2차원 구조)로 Import후에
 * 실제 Tech내에서 활용하는 3차원 구조(예전 구조)로 convert을 합니다.
 */

// 원본 데이터테이블의 Package Name
const FString DROP_DATA_TABLE_PATH(TEXT("/Game/DataCenter/DataTable/Drop/DTGsDropData"));
const FString DROP_ITEM_GROUP_TABLE_PATH(TEXT("/Game/DataCenter/DataTable/Drop/DTGsDropItemGroup"));
const FString DROP_PROB_GROUP_TABLE_PATH(TEXT("/Game/DataCenter/DataTable/Drop/DTGsDropProbGroup"));
const FString DROP_ITEM_GROUP_EXCEL_TABLE_PATH(TEXT("/Game/DataCenter/DataTable/Drop/DTGsDropItemGroupExcel"));
const FString DROP_PROB_GROUP_EXCEL_TABLE_PATH(TEXT("/Game/DataCenter/DataTable/Drop/DTGsDropProbGroupExcel"));

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterDropData)
	bool FDataTableConverterDropData::Convert()
{
	// 원본 데이터테이블 로드
	UGsDataTable* dropDataTable = nullptr;
	if (false == FGsDataTableConvertBase::LoadDataTable(DROP_DATA_TABLE_PATH, dropDataTable))
	{
		return false;
	}

	UGsDataTable* dropItemGroupTable = nullptr;
	if (false == FGsDataTableConvertBase::LoadDataTable(DROP_ITEM_GROUP_TABLE_PATH, dropItemGroupTable))
	{
		return false;
	}

	UGsDataTable* dropProbGroupTable = nullptr;
	if (false == FGsDataTableConvertBase::LoadDataTable(DROP_PROB_GROUP_TABLE_PATH, dropProbGroupTable))
	{
		return false;
	}

	UGsDataTable* dropItemGroupExcelTable = nullptr;
	if (false == FGsDataTableConvertBase::LoadDataTable(DROP_ITEM_GROUP_EXCEL_TABLE_PATH, dropItemGroupExcelTable))
	{
		return false;
	}

	UGsDataTable* dropProbGroupExcelTable = nullptr;
	if (false == FGsDataTableConvertBase::LoadDataTable(DROP_PROB_GROUP_EXCEL_TABLE_PATH, dropProbGroupExcelTable))
	{
		return false;
	}	

	// 원본 데이터테이블 비우기	
	dropItemGroupTable->EmptyTable();
	dropProbGroupTable->EmptyTable();

#if true
	//excel 데이터에서 id 그룹핑
	FString context;
	TArray<FGsSchemaDropItemGroupExcel*> itemArray;
	dropItemGroupExcelTable->GetAllRows<FGsSchemaDropItemGroupExcel>(context, itemArray);

	TMap<int32, FGsSchemaDropItemGroup> itemMap;
	FGsSchemaDropItemGroupInfo itemTemp;
	for (FGsSchemaDropItemGroupExcel* item : itemArray)
	{
		if (false == itemMap.Contains(item->id))
		{
			FGsSchemaDropItemGroup itemGroup;
			itemGroup.id = item->id;
			itemMap.Emplace(item->id, itemGroup);
		}
		
		itemTemp.itemId = item->itemId;
		itemTemp.itemCountMin = item->itemCountMin;
		itemTemp.itemCountMax = item->itemCountMax;

		itemMap[item->id].dropItemGroupList.Emplace(itemTemp);
	}

	//get DTGsDropItemGroupExcel
	TArray<FGsSchemaDropProbGroupExcel*> probArray;
	dropProbGroupExcelTable->GetAllRows<FGsSchemaDropProbGroupExcel>(context, probArray);

	TMap<int32, FGsSchemaDropProbGroup> propMap;
	FGsSchemaDropProbGroupInfo propTemp;

	for (FGsSchemaDropProbGroupExcel* prob : probArray)
	{
		if (false == propMap.Contains(prob->id))
		{
			FGsSchemaDropProbGroup probGroup;
			probGroup.id = prob->id;
			propMap.Emplace(prob->id, probGroup);
		}
				
		propTemp.categoryMain = prob->categoryMain;
		propTemp.categorySub = prob->categorySub;
		propTemp.grade = prob->grade;
		propTemp.tier = prob->tier;
		propTemp.prob = prob->prob;

		propMap[prob->id].dropProbGroupList.Emplace(propTemp);
	}

	//그룹핑한 정보 저장
	for (auto itemPair : itemMap)
	{
		dropItemGroupTable->AddRow(FName(FString::FromInt(itemPair.Key)), itemPair.Value);
	}

	for (auto propPair : propMap)
	{
		dropProbGroupTable->AddRow(FName(FString::FromInt(propPair.Key)), propPair.Value);
	}

	dropItemGroupTable->SavePackage();
	dropProbGroupTable->SavePackage();
#endif

	return true;
}

