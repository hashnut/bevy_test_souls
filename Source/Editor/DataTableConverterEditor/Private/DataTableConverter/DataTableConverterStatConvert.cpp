


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"
#include "Item/Data/GsItemStatData.h"
#include "DataSchema/NeverCook/Stat/GsSchemaStatConvertDataExcel.h"

#include "DataSchema/Stat/GsSchemaStatConvertData.h"
#include "GsDataTable.h"
#include "Util/GsText.h"
#include "Helper/RSQLEditorUtil.h"


/**
 * 기작업된 복잡도 있는 멀티키 구조를 지원하기 위해 1차 테이블 스키마(엑셀 테이블 2차원 구조)로 Import후에
 * 실제 Tech내에서 활용하는 3차원 구조(예전 구조)로 convert을 합니다.
 */



// 원본 데이터테이블의 Package Name
static const FString ORIGINAL_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/Stat/DTGsStatConvertData"));

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterStatConvertData)
	bool FDataTableConverterStatConvertData::Convert()
{
	// 원본 데이터테이블 로드
	UGsDataTable* OriginalDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(ORIGINAL_DATATABLE_PACKAGE_NAME, OriginalDataTable))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	OriginalDataTable->EmptyTable();

	// 하위정보들을 입력해주는 함수
	auto fn_insertStatConvertInfo = [](FGsSchemaStatConvertData* modifyed, const FGsSchemaStatConvertDataExcel* imported)-> void
	{
		FGsSchemaStatConvertTable insertInfo;
		insertInfo.baseValue = imported->baseValue;
		FGsItemStatData data;
		if (imported->statType1 != StatType::NONE)
		{
			data.type = imported->statType1;
			data.value = imported->statValue1;
			insertInfo.statList.Emplace(data);
		}
		if (imported->statType2 != StatType::NONE)
		{
			data.type = imported->statType2;
			data.value = imported->statValue2;
			insertInfo.statList.Emplace(data);
		}
		if (imported->statType3 != StatType::NONE)
		{
			data.type = imported->statType3;
			data.value = imported->statValue3;
			insertInfo.statList.Emplace(data);
		}
		if (imported->statType4 != StatType::NONE)
		{
			data.type = imported->statType4;
			data.value = imported->statValue4;
			insertInfo.statList.Emplace(data);
		}
		if (imported->statType5 != StatType::NONE)
		{
			data.type = imported->statType5;
			data.value = imported->statValue5;
			insertInfo.statList.Emplace(data);
		}
		if (imported->statType6 != StatType::NONE)
		{
			data.type = imported->statType6;
			data.value = imported->statValue6;
			insertInfo.statList.Emplace(data);
		}
		modifyed->convertList.Add(insertInfo);
	};

	const UGsTableStatConvertDataExcel* ExcelData = Cast<UGsTableStatConvertDataExcel>(FGsSchemaStatConvertDataExcel::GetStaticTable());
	TArray< const FGsSchemaStatConvertDataExcel*> ExcelAllRow;
	if (false == ExcelData->GetAllRows(ExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableNpcBalanceExcel empty "));
		return false;;
	}

	const FGsSchemaStatConvertData* findTarget = nullptr;
	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (int i = 0; i < ExcelAllRow.Num(); ++i)
	{
		// UDataTable::AddRow will first remove the row if it already exists so we don't need to do anything special here
		if (const FGsSchemaStatConvertDataExcel* rowTable = ExcelAllRow[i])
		{
			const FName RowName = rowTable->baseType.GetName();
			if ((findTarget = OriginalDataTable->FindRow<const FGsSchemaStatConvertData>(RowName, OriginalDataTable->GetName())))
			{
				fn_insertStatConvertInfo(const_cast<FGsSchemaStatConvertData*>(findTarget), rowTable);
			}
			else
			{
				FGsSchemaStatConvertData addsome;

				addsome.RowName = RowName;
				addsome.baseType = rowTable->baseType;
				fn_insertStatConvertInfo(&addsome, rowTable);
				
				OriginalDataTable->AddRow(addsome.RowName, addsome);
			}
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

