


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"
#include "GsDataTable.h"
#include "Util/GsText.h"
#include "Helper/RSQLEditorUtil.h"
#include "DataSchema/NeverCook/Skill/Cost/GsSchemaSkillUpCostSetExcel.h"
#include "DataSchema/Skill/Cost/GsSchemaSkillUpCostSet.h"

/**
 * 
 */
IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterSkillUpCostSet)
	bool FDataTableConverterSkillUpCostSet::Convert()
{
	// 원본 데이터테이블 로드
	UGsDataTable* OriginalDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(TEXT("/Game/DataCenter/DataTable/Skill/Cost/DTGsSkillUpCostSet"), OriginalDataTable))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	OriginalDataTable->EmptyTable();

	const UGsTableSkillUpCostSetExcel* ExcelData = Cast<UGsTableSkillUpCostSetExcel>(FGsSchemaSkillUpCostSetExcel::GetStaticTable());
	TArray< const FGsSchemaSkillUpCostSetExcel*> ExcelAllRow;
	if (false == ExcelData->GetAllRows(ExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("FGsSchemaSkillUpCostSetExcel empty "));
		return false;;
	}

	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (int i = 0; i < ExcelAllRow.Num(); ++i)
	{
		FGsSchemaSkillUpCostSet newInfo;
		FGsSchemaSkillUpCurrencyInfo newCurrencyInfo;
		FGsSchemaSkillUpItemInfo newItemInfo;
		if (const FGsSchemaSkillUpCostSetExcel* rowTable = ExcelAllRow[i])
		{
			newInfo.id = rowTable->id;
			newInfo.costType = rowTable->costType;

			if (rowTable->currencyType1 != CurrencyType::NONE)
			{
				newCurrencyInfo.currencyType = rowTable->currencyType1;
				newCurrencyInfo.currencyAmount = rowTable->currencyAmount1;
				newInfo.currencyList.Emplace(newCurrencyInfo);
			}
			if (rowTable->currencyType2 != CurrencyType::NONE)
			{
				newCurrencyInfo.currencyType = rowTable->currencyType2;
				newCurrencyInfo.currencyAmount = rowTable->currencyAmount2;
				newInfo.currencyList.Emplace(newCurrencyInfo);
			}
			if (rowTable->currencyType3 != CurrencyType::NONE)
			{
				newCurrencyInfo.currencyType = rowTable->currencyType3;
				newCurrencyInfo.currencyAmount = rowTable->currencyAmount3;
				newInfo.currencyList.Emplace(newCurrencyInfo);
			}
			if (rowTable->currencyType4 != CurrencyType::NONE)
			{
				newCurrencyInfo.currencyType = rowTable->currencyType4;
				newCurrencyInfo.currencyAmount = rowTable->currencyAmount4;
				newInfo.currencyList.Emplace(newCurrencyInfo);
			}
			if (rowTable->currencyType5 != CurrencyType::NONE)
			{
				newCurrencyInfo.currencyType = rowTable->currencyType5;
				newCurrencyInfo.currencyAmount = rowTable->currencyAmount5;
				newInfo.currencyList.Emplace(newCurrencyInfo);
			}

			if (rowTable->itemId1 != 0)
			{
				newItemInfo.itemId = rowTable->itemId1;
				newItemInfo.itemCount = rowTable->itemCount1;
				newInfo.itemList.Emplace(newItemInfo);
			}
			if (rowTable->itemId2 != 0)
			{
				newItemInfo.itemId = rowTable->itemId2;
				newItemInfo.itemCount = rowTable->itemCount2;
				newInfo.itemList.Emplace(newItemInfo);
			}
			if (rowTable->itemId3 != 0)
			{
				newItemInfo.itemId = rowTable->itemId3;
				newItemInfo.itemCount = rowTable->itemCount3;
				newInfo.itemList.Emplace(newItemInfo);
			}
			if (rowTable->itemId4 != 0)
			{
				newItemInfo.itemId = rowTable->itemId4;
				newItemInfo.itemCount = rowTable->itemCount4;
				newInfo.itemList.Emplace(newItemInfo);
			}
			if (rowTable->itemId5 != 0)
			{
				newItemInfo.itemId = rowTable->itemId5;
				newItemInfo.itemCount = rowTable->itemCount5;
				newInfo.itemList.Emplace(newItemInfo);
			}

			OriginalDataTable->AddRow(rowTable->RowName, newInfo);
		}
	}
	// 원본 데이터테이블 저장
	if (!OriginalDataTable->SavePackage())
	{
		UE_LOG(LogTemp, Error, TEXT("!OriginalDataTable->SavePackage(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
		return false;
	}

	return true;
}

