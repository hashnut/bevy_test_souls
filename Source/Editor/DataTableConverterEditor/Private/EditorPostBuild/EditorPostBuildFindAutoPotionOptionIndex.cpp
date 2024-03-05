
#include "Runtime/AssetRegistry/Public/AssetRegistry/AssetRegistryModule.h"
#include "Runtime/Core/Public/Misc/AutomationTest.h"
#include "Runtime/DataCenter/Public/Option/GsOptionEnumType.h"

#include "../Plugins/RSQL/Source/RSQLEditor/Public/GsTableManagerEditor.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "DataSchema/Option/GsSchemaOptionCategory.h"
#include "DataSchema/Option/GsSchemaOptionDivision.h"
#include "DataSchema/Option/GsSchemaOptionItem.h"
#include "DataSchema/Option/GsSchemaAutoPotionTabIndexInfo.h"
#include "DataTableConverter/GsDataTableConvertBase.h"

#include "DataSchema/GsSchemaEnums.h"


#define GS_UTEST_TRUE(What) UTEST_TRUE(TEXT(#What), What)
#define GS_UTEST_NOT_NULL(What) UTEST_NOT_NULL(TEXT(#What), What)

/**
 * 게임 옵션창에서 자동물약 퍼센트 조정 하는 메인탭 및 서브탭 인덱스 find
 */


IMPLEMENT_SIMPLE_AUTOMATION_TEST(EditorPostBuildFindAutoPotionOptionIndex, TEXT("Raon.EditorPostBuild.7 자동물약 세팅 옵션창 인덱스")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)
bool EditorPostBuildFindAutoPotionOptionIndex::RunTest(const FString& Parameters)
{
	AddInfo(TEXT("자동물약 세팅 옵션창 탭 인덱스"));

	// 필요한 테이블 로드
	const UGsTable* optionCategoryData = FGsSchemaOptionCategory::GetStaticTable();
	const UGsTable* optionDivisionData = FGsSchemaOptionDivision::GetStaticTable();
	const UGsTable* optionItemData = FGsSchemaOptionItem::GetStaticTable();
	
	GS_UTEST_NOT_NULL(optionCategoryData)
	GS_UTEST_NOT_NULL(optionDivisionData)
	GS_UTEST_NOT_NULL(optionItemData)

	int32 childTabIndex = 0;
	int32 parentTabIndex = 0;
	const FGsSchemaOptionItem* findOptionItem = nullptr;
	TArray<const FGsSchemaOptionItem*> optionItemAllRow;
	if (false == optionItemData->GetAllRows(optionItemAllRow))
	{
		AddInfo(FString::Printf(TEXT("FGsSchemaOptionItem table not found !!! ")));
		return false;
	}
	for (const FGsSchemaOptionItem* data : optionItemAllRow)
	{
		if(nullptr == data)
			continue;

		if (data->OptionItemType != EGsOptionItem::AutoPotionUse)
			continue;

		findOptionItem = data;
		break;
	}
	
	if (nullptr == findOptionItem)
	{
		AddInfo(FString::Printf(TEXT("FGsSchemaOptionItem nullptr == findOptionItem !!!!!!")));
		return false;
	}

	const FGsSchemaOptionDivision* findDivisionData = nullptr;
	TArray<const FGsSchemaOptionDivision*> optionDivisionAllRow;
	if (false == optionDivisionData->GetAllRows(optionDivisionAllRow))
	{
		AddInfo(FString::Printf(TEXT("FGsSchemaOptionDivision table not found !!! ")));
		return false;
	}

	for (const FGsSchemaOptionDivision* data : optionDivisionAllRow)
	{
		if(nullptr == data)
			continue;

		bool find = false;
		for (FGsSchemaOptionItemRow iter : data->OptionList)
		{
			if (iter.GetRow() != findOptionItem)
				continue;
			
			findDivisionData = data;
			find = true;
			break;
		}
		if (find)
		{
			break;
		}
	}

	if (nullptr == findDivisionData)
	{
		AddInfo(FString::Printf(TEXT("FGsSchemaOptionDivision nullptr == findDivisionData !!!!!!")));
		return false;
	}
	TArray<const FGsSchemaOptionCategory*> optionCategoryAllRow;
	if (false == optionCategoryData->GetAllRows(optionCategoryAllRow))
	{
		AddInfo(FString::Printf(TEXT("FGsSchemaOptionCategory table not found !!! ")));
		return false;
	}

	for (const FGsSchemaOptionCategory* data : optionCategoryAllRow)
	{
		if (nullptr == data)
			continue;

		childTabIndex = 0;
		bool find = false;
		for (FGsSchemaOptionDivisionRow iter : data->DivisionList)
		{
			if (iter.GetRow() != findDivisionData)
			{
				++childTabIndex;
				continue;
			}
			find = true;
			break;
		}
		if (find)
		{
			break;
		}
		++parentTabIndex;
	}

	UGsDataTable* copyTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(TEXT("/Game/DataCenter/DataTable/Option/DTGsAutoPotionTabIndexInfo.DTGsAutoPotionTabIndexInfo"), copyTable))
	{
		return false;
	}
	copyTable->EmptyTable();
	
	FGsSchemaAutoPotionTabIndexInfo addData;
	addData.RowName = TEXT("none");
	addData.parentTabIndex = parentTabIndex;
	addData.childTabIndex = childTabIndex;
	copyTable->AddRow(TEXT("none"), addData);

	//AddInfo(FString::Printf(TEXT("parentTabIndex : %d "), parentTabIndex));
	//AddInfo(FString::Printf(TEXT("childTabIndex : %d "), childTabIndex));

	if (false == copyTable->SavePackage())
	{
		AddInfo(FString::Printf(TEXT("false == copyTable->SavePackage() !!!!!!!!!")));
		return false;
	}

	AddInfo(TEXT("완료"));
	
	return true;
}
