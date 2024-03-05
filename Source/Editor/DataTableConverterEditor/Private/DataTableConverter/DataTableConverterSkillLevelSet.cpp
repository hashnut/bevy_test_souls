


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"

#include "GsDataTable.h"
#include "Util/GsText.h"
#include "Helper/RSQLEditorUtil.h"
#include "DataSchema/NeverCook/Skill/Level/GsSchemaSkillLevelSetExcel.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelInfo.h"
#include "DataSchema/Skill/Upgrade/GsSchemaSkillUpgradeSet.h"
#include "DataSchema/NeverCook/Skill/Level/GsSchemaSkillLevelSetInfoExcel.h"
#include "../../../../T1Project/UTIL/GsTableUtil.h"

template <typename T = UGsDataTable, typename TRow = FGsSchemaRowBase>
void FillRowDataSkillLevelSet(UGsDataTable* InTable, const FName& InRowName, TRow* OutRowData)
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
//-------------------------------------------------------

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterSkillLevelSet)
	bool FDataTableConverterSkillLevelSet::Convert()
{
	// 원본 데이터테이블 로드
	UGsDataTable* OriginalDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(TEXT("/Game/DataCenter/DataTable/Skill/Level/DTGsSkillLevelSet"), OriginalDataTable))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	OriginalDataTable->EmptyTable();

	// LevelSet 컨버팅
	const UGsTableSkillLevelSetExcel* levelSetExcelData = 
		Cast<UGsTableSkillLevelSetExcel>(FGsSchemaSkillLevelSetExcel::GetStaticTable());
	
	TArray< const FGsSchemaSkillLevelSetExcel*> ExcelAllRow;
	if (false == levelSetExcelData->GetAllRows(ExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableSkillLevelSetExcel empty "));
		return false;;
	}

	UGsDataTable* skillUpgradeDT = nullptr;
	FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Skill/Upgrade/DTGsSkillUpgradeSet.DTGsSkillUpgradeSet"), skillUpgradeDT);

	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (int i = 0; i < ExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaSkillLevelSetExcel* rowTable = ExcelAllRow[i])
		{
			FGsSchemaSkillLevelSet newLevelSet;
			newLevelSet.Desc = rowTable->Desc;
			newLevelSet.skillId = rowTable->SkillId;
			newLevelSet.maxUpgradeStep = rowTable->maxUpgradeStep;
			newLevelSet.maxEnchantLevel = rowTable->maxEnchantLevel;
			newLevelSet.spiritShotConsumption = rowTable->spiritShotConsumption;			

			newLevelSet.upgradeInfo.currentStep = rowTable->currentStep;
			FillRowDataSkillLevelSet<FGsSchemaSkillUpgradeSet, FGsSchemaSkillUpgradeSetRow>(skillUpgradeDT,
				rowTable->skillUpgradeId, &newLevelSet.upgradeInfo.skillUpgradeId);

			FName rowName = FName(*FString::Printf(TEXT("%d"), newLevelSet.skillId));
			OriginalDataTable->AddRow(rowName, newLevelSet);
		}
	}

	// LevelSet 하위 levelInfoList 채우기
	const UGsTableSkillLevelSetInfoExcel* levelSetinfoExcelData =
		Cast<UGsTableSkillLevelSetInfoExcel>(FGsSchemaSkillLevelSetInfoExcel::GetStaticTable());

	TArray< const FGsSchemaSkillLevelSetInfoExcel*> InfoExcelAllRow;
	if (false == levelSetinfoExcelData->GetAllRows(InfoExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableSkillLevelSetInfoExcel empty "));
		return false;;
	}
	
	for (int i = 0; i < InfoExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaSkillLevelSetInfoExcel* rowTable = InfoExcelAllRow[i])
		{
			// levelInfoPairId를 통해 LevelSet 원본 데이터를 찾음
			const FGsSchemaSkillLevelSet* findOriginLevelSet = 
				UGsTableUtil::FindRowById<UGsTableSkillLevelSet, FGsSchemaSkillLevelSet>(rowTable->skillId);

			if (nullptr == findOriginLevelSet)
			{
				continue;
			}
			
			if (findOriginLevelSet)
			{				
				FGsSchemaSkillLevelInfo newSkillLevelInfo;
				newSkillLevelInfo.level = rowTable->level;
				newSkillLevelInfo.cooldownTime = rowTable->cooldownTime;
				newSkillLevelInfo.enchantSkillUpCostId = rowTable->enchantSkillUpCostId;
				newSkillLevelInfo.notifyInfoList = rowTable->notifyInfoList;
				newSkillLevelInfo.effectText = rowTable->EffectText;

				// CostConsumeInfo
				if (rowTable->requireItemId1 != 0)
				{
					FGsSchemaSkillCostItemInfo newInfo;
					newInfo.itemId = rowTable->requireItemId1;
					newInfo.itemAmount = rowTable->requireAmount1;
					newSkillLevelInfo.costConsumeInfo.requireItemList.Emplace(newInfo);
				}
				if (rowTable->requireItemId2 != 0)
				{
					FGsSchemaSkillCostItemInfo newInfo;
					newInfo.itemId = rowTable->requireItemId2;
					newInfo.itemAmount = rowTable->requireAmount2;
					newSkillLevelInfo.costConsumeInfo.requireItemList.Emplace(newInfo);
				}

				// StatInfo
				if (rowTable->statType1 != StatType::NONE)
				{
					FGsSchemaSkillStatInfo newInfo;
					newInfo.type = rowTable->statType1;
					newInfo.value = rowTable->statValue1;
					newSkillLevelInfo.costConsumeInfo.statInfoList.Emplace(newInfo);
				}
				if (rowTable->statType2 != StatType::NONE)
				{
					FGsSchemaSkillStatInfo newInfo;
					newInfo.type = rowTable->statType2;
					newInfo.value = rowTable->statValue2;
					newSkillLevelInfo.costConsumeInfo.statInfoList.Emplace(newInfo);
				}

				// const 속성 제거
				FGsSchemaSkillLevelSet* constCastLevelSet = const_cast<FGsSchemaSkillLevelSet*>(findOriginLevelSet);
				constCastLevelSet->levelInfoList.Emplace(newSkillLevelInfo);
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

