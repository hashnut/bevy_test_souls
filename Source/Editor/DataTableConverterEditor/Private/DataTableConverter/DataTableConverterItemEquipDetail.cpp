


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"
#include "Item/Data/GsItemStatData.h"
#include "DataSchema/NeverCook/Item/GsSchemaItemEquipDetailExcel.h"

#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "DataSchema/Item/GsSchemaItemEquipFuse.h"
#include "GsDataTable.h"
#include "Util/GsText.h"
#include "Helper/RSQLEditorUtil.h"



/**
 * 기작업된 복잡도 있는 멀티키 구조를 지원하기 위해 1차 테이블 스키마(엑셀 테이블 2차원 구조)로 Import후에
 * 실제 Tech내에서 활용하는 3차원 구조(예전 구조)로 convert을 합니다.
 */



// 원본 데이터테이블의 Package Name
static const FString ORIGINAL_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/Item/DTGsItemEquipDetail"));

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterItemEquipDetail)
	bool FDataTableConverterItemEquipDetail::Convert()
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
	auto fn_insertStatConvertInfo = [](FGsSchemaItemEquipDetail* modifyed, const FGsSchemaItemEquipDetailExcel* imported)-> void
	{
		FGsItemStatData data;
		if (imported->statType1 != StatType::NONE)
		{
			data.type = imported->statType1;
			data.value = imported->statValue1;
			modifyed->statList.Emplace(data);
		}
		if (imported->statType2 != StatType::NONE)
		{
			data.type = imported->statType2;
			data.value = imported->statValue2;
			modifyed->statList.Emplace(data);
		}
		if (imported->statType3 != StatType::NONE)
		{
			data.type = imported->statType3;
			data.value = imported->statValue3;
			modifyed->statList.Emplace(data);
		}
		if (imported->statType4 != StatType::NONE)
		{
			data.type = imported->statType4;
			data.value = imported->statValue4;
			modifyed->statList.Emplace(data);
		}
		if (imported->statType5 != StatType::NONE)
		{
			data.type = imported->statType5;
			data.value = imported->statValue5;
			modifyed->statList.Emplace(data);
		}
		if (imported->statType6 != StatType::NONE)
		{
			data.type = imported->statType6;
			data.value = imported->statValue6;
			modifyed->statList.Emplace(data);
		}
		if (imported->statType7 != StatType::NONE)
		{
			data.type = imported->statType7;
			data.value = imported->statValue7;
			modifyed->statList.Emplace(data);
		}
		if (imported->statType8 != StatType::NONE)
		{
			data.type = imported->statType8;
			data.value = imported->statValue8;
			modifyed->statList.Emplace(data);
		}
	};

	const UGsTableItemEquipDetailExcel* ExcelData = Cast<UGsTableItemEquipDetailExcel>(FGsSchemaItemEquipDetailExcel::GetStaticTable());
	TArray< const FGsSchemaItemEquipDetailExcel*> ExcelAllRow;
	if (false == ExcelData->GetAllRows(ExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableNpcBalanceExcel empty "));
		return false;;
	}

	UGsDataTable* itemFusionDT = nullptr;
	FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Item/DTGsItemEquipFuse.DTGsItemEquipFuse"), itemFusionDT);

	auto FillRowDataItemEquipFuse = [](UGsDataTable* InTable, const FName& InRowName, FGsSchemaRowBase* OutRowData)
	{
		OutRowData->DataTable = nullptr;
		OutRowData->RowName = NAME_None;

		const TMap<FName, uint8*>& rowMap = InTable->GetRowMap();
		for (const TPair<FName, uint8*>& itPair : rowMap)
		{
			const FGsSchemaItemEquipFuse* data = reinterpret_cast<const FGsSchemaItemEquipFuse*>(itPair.Value);
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
	};

	const FGsSchemaItemEquipDetail* findTarget = nullptr;
	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (int i = 0; i < ExcelAllRow.Num(); ++i)
	{
		// UDataTable::AddRow will first remove the row if it already exists so we don't need to do anything special here
		if (const FGsSchemaItemEquipDetailExcel* rowTable = ExcelAllRow[i])
		{
			FName rowName = FName(*FString::FromInt(rowTable->id));
			if ((findTarget = OriginalDataTable->FindRow<const FGsSchemaItemEquipDetail>(rowName, OriginalDataTable->GetName())))
			{
				fn_insertStatConvertInfo(const_cast<FGsSchemaItemEquipDetail*>(findTarget), rowTable);
			}
			else
			{
				FGsSchemaItemEquipDetail addsome;

				addsome.RowName = rowName;
				addsome.id				= rowTable->id;
				addsome.tier			= rowTable->tier;
				addsome.modelPath		= rowTable->modelPath;
				addsome.materialType	= rowTable->materialType;
				addsome.weaponType		= rowTable->weaponType;

				addsome.safeEnchantLevel= rowTable->safeEnchantLevel;

				addsome.enchantShieldId		= rowTable->enchantShieldId;
				addsome.enchantRateId		= rowTable->enchantRateId;
				addsome.enchantBonusId		= rowTable->enchantBonusId;
				addsome.enchantBalanceId	= rowTable->enchantBalanceId;
				addsome.enchantCost			= rowTable->enchantCost;

				addsome.passivityIdList = rowTable->passivityIdList;

				addsome.refineOptionCostCurrencyType	= rowTable->refineOptionCostCurrencyType;
				addsome.refineOptionCost				= rowTable->refineOptionCost;
				addsome.refineOptionId					= rowTable->refineOptionId;				
				addsome.refineBonusOptionIds			= rowTable->refineBonusOptionIds;
				addsome.refineBonusOptionProbs			= rowTable->refineBonusOptionProbs;

				addsome.goldRecoveryCost	= rowTable->goldRecoveryCost;
				addsome.diaRecoveryCost		= rowTable->diaRecoveryCost;

				addsome.setId				= rowTable->setId;

				addsome.itemOptionSkillGroupId = rowTable->itemOptionSkillGroupId;

				addsome.equipGroupId = rowTable->equipGroupId;

				fn_insertStatConvertInfo(&addsome, rowTable);

				FillRowDataItemEquipFuse(itemFusionDT, rowTable->itemEquipFuseRowName, &addsome.itemFusionData);
				
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

