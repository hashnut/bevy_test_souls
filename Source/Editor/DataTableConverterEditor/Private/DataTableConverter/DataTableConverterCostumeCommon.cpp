


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"
#include "Item/Data/GsItemStatData.h"

#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/GameObject/GsSchemaUserLookInfo.h"
#include "GsDataTable.h"
#include "Util/GsTableUtil.h"
#include "Helper/RSQLEditorUtil.h"


/**
 * 코스튬 데이터의 외형데이터 분리 (외형변경 시스템 기반작업을 위한 컨버팅)
 */


// 원본 데이터테이블의 Package Name
static const FString ORIGINAL_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/GameObject/DTGsUserLookInfo"));
static const FString ORIGINAL_DATATABLE_PACKAGE_CostumeCommon(TEXT("/Game/DataCenter/DataTable/Costume/DTGsCostumeCommon"));


template <typename T = UGsDataTable, typename TRow = FGsSchemaRowBase>
void FillRow(UGsDataTable* InTable, const FName& InRowName, TRow* OutRowData)
{
	OutRowData->DataTable = nullptr;
	OutRowData->RowName = NAME_None;

	if (InRowName.IsNone())
	{
		return;
	}

	const TMap<FName, uint8*>& rowMap = InTable->GetRowMap();
	for (const TPair<FName, uint8*>& itPair : rowMap)
	{
		const T* data = reinterpret_cast<const T*>(itPair.Value);
		if (nullptr == data)
		{
			continue;
		}

		if (0 == data->GetRowName().Compare(InRowName))
		{
			OutRowData->DataTable = InTable;
			OutRowData->RowName = data->GetRowName();
		}
	}
}

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterCostumeCommon)
	bool FDataTableConverterCostumeCommon::Convert()
{
	// 원본 데이터테이블 로드
	UGsDataTable* OriginalDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(ORIGINAL_DATATABLE_PACKAGE_NAME, OriginalDataTable))
	{
		return false;
	}

	UGsDataTable* CostumeOriginalDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(ORIGINAL_DATATABLE_PACKAGE_CostumeCommon, CostumeOriginalDataTable))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	OriginalDataTable->EmptyTable();

	

	TArray<FGsSchemaCostumeCommon*> AllRow;
	CostumeOriginalDataTable->GetAllRows(CostumeOriginalDataTable->GetName(), AllRow);

#if 0
	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (int i = 0; i < AllRow.Num(); ++i)
	{
		// UDataTable::AddRow will first remove the row if it already exists so we don't need to do anything special here
		if (const FGsSchemaCostumeCommon* rowTable = AllRow[i])
		{
			FGsSchemaUserLookInfo addsome;

			addsome.RowName = rowTable->GetRowName();
			addsome.id = rowTable->id;
			addsome.PartsHair = rowTable->detail.hairPath;
			addsome.PartsBody = rowTable->detail.bodyPath;
			OriginalDataTable->AddRow(addsome.RowName, addsome);
			OriginalDataTable->Modify();
		}
	}
#endif
	// 원본 데이터테이블 저장
	if (!OriginalDataTable->SavePackage())
	{
		UE_LOG(LogTemp, Error, TEXT("!OriginalDataTable->SavePackage(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
		return false;
	}

	// 변경된 코스튬 Common에 적용
	TArray<FGsSchemaUserLookInfo*> AllLookInfoRow;
	OriginalDataTable->GetAllRows(OriginalDataTable->GetName(), AllLookInfoRow);

	for (int i = 0; i < AllRow.Num(); ++i)
	{
		if (FGsSchemaCostumeCommon* rowTable = AllRow[i])
		{
			for (int j = 0; j < AllLookInfoRow.Num(); ++j)
			{
				if (AllLookInfoRow[j]->id == rowTable->id)
				{
					FGsSchemaUserLookInfoRow Row;
					FillRow<FGsSchemaUserLookInfo, FGsSchemaUserLookInfoRow>(OriginalDataTable, AllLookInfoRow[j]->RowName, &Row);
					rowTable->detail.shapeInfo = Row;
					CostumeOriginalDataTable->Modify();
					break;
				}
			}
		}
	}

	CostumeOriginalDataTable->SavePackage();
	return true;
}

