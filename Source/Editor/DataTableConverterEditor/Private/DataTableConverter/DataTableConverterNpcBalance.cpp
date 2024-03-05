


#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"

#include "DataSchema/NeverCook/Npc/GsSchemaNpcBalanceExcel.h"
#include "DataSchema/NeverCook/Npc/GsSchemaNpcLevelSet.h"
#include "DataSchema/NeverCook/Npc/GsSchemaNpcLevelStatSet.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"


/**
 * 기작업된 복잡도 있는 멀티키 구조를 지원하기 위해 1차 테이블 스키마(엑셀 테이블 2차원 구조)로 Import후에
 * 실제 Tech내에서 활용하는 3차원 구조(예전 구조)로 convert을 합니다.
 */



// 원본 데이터테이블의 Package Name
static const FString ORIGINAL_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/GameObject/Npc/DTGsNpcBalance"));

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterNpcBalance)
	bool FDataTableConverterNpcBalance::Convert()
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
	auto fn_insertBalanceLevelInfo = [](FGsSchemaNpcBalance* modifyed, const FGsSchemaNpcBalanceExcel* imported)-> void
	{
		if (const FGsSchemaNpcLevelSet* levelStatrow = imported->NpcLevelSetId.GetRow())
		{
			FGsSchemaNpcBalanceLevel addsomeLevel;
			addsomeLevel.level = imported->level;			
			addsomeLevel.rewardId = levelStatrow->RewardId;			

			if (const FGsSchemaNpcLevelStatSet* levelStatSet = levelStatrow->levelStatSetId.GetRow())
			{
				FGsSchemaNpcBalanceLevelStat data;
				if (levelStatSet->statType0 != StatType::NONE)
				{
					data.type = levelStatSet->statType0;
					data.value = levelStatSet->statValue0;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType1 != StatType::NONE)
				{
					data.type = levelStatSet->statType1;
					data.value = levelStatSet->statValue1;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType2 != StatType::NONE)
				{
					data.type = levelStatSet->statType2;
					data.value = levelStatSet->statValue2;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType3 != StatType::NONE)
				{
					data.type = levelStatSet->statType3;
					data.value = levelStatSet->statValue3;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType4 != StatType::NONE)
				{
					data.type = levelStatSet->statType4;
					data.value = levelStatSet->statValue4;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType5 != StatType::NONE)
				{
					data.type = levelStatSet->statType5;
					data.value = levelStatSet->statValue5;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType6 != StatType::NONE)
				{
					data.type = levelStatSet->statType6;
					data.value = levelStatSet->statValue6;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType7 != StatType::NONE)
				{
					data.type = levelStatSet->statType7;
					data.value = levelStatSet->statValue7;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType8 != StatType::NONE)
				{
					data.type = levelStatSet->statType8;
					data.value = levelStatSet->statValue8;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType9 != StatType::NONE)
				{
					data.type = levelStatSet->statType9;
					data.value = levelStatSet->statValue9;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType10 != StatType::NONE)
				{
					data.type = levelStatSet->statType10;
					data.value = levelStatSet->statValue10;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType11 != StatType::NONE)
				{
					data.type = levelStatSet->statType11;
					data.value = levelStatSet->statValue11;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType12 != StatType::NONE)
				{
					data.type = levelStatSet->statType12;
					data.value = levelStatSet->statValue12;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType13 != StatType::NONE)
				{
					data.type = levelStatSet->statType13;
					data.value = levelStatSet->statValue13;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType14 != StatType::NONE)
				{
					data.type = levelStatSet->statType14;
					data.value = levelStatSet->statValue14;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType15 != StatType::NONE)
				{
					data.type = levelStatSet->statType15;
					data.value = levelStatSet->statValue15;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType16 != StatType::NONE)
				{
					data.type = levelStatSet->statType16;
					data.value = levelStatSet->statValue16;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType17 != StatType::NONE)
				{
					data.type = levelStatSet->statType17;
					data.value = levelStatSet->statValue17;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType18 != StatType::NONE)
				{
					data.type = levelStatSet->statType18;
					data.value = levelStatSet->statValue18;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType19 != StatType::NONE)
				{
					data.type = levelStatSet->statType19;
					data.value = levelStatSet->statValue19;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType20 != StatType::NONE)
				{
					data.type = levelStatSet->statType20;
					data.value = levelStatSet->statValue20;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType21 != StatType::NONE)
				{
					data.type = levelStatSet->statType21;
					data.value = levelStatSet->statValue21;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType22 != StatType::NONE)
				{
					data.type = levelStatSet->statType22;
					data.value = levelStatSet->statValue22;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType23 != StatType::NONE)
				{
					data.type = levelStatSet->statType23;
					data.value = levelStatSet->statValue23;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType24 != StatType::NONE)
				{
					data.type = levelStatSet->statType24;
					data.value = levelStatSet->statValue24;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType25 != StatType::NONE)
				{
					data.type = levelStatSet->statType25;
					data.value = levelStatSet->statValue25;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType26 != StatType::NONE)
				{
					data.type = levelStatSet->statType26;
					data.value = levelStatSet->statValue26;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType27 != StatType::NONE)
				{
					data.type = levelStatSet->statType27;
					data.value = levelStatSet->statValue27;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType28 != StatType::NONE)
				{
					data.type = levelStatSet->statType28;
					data.value = levelStatSet->statValue28;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType29 != StatType::NONE)
				{
					data.type = levelStatSet->statType29;
					data.value = levelStatSet->statValue29;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType30 != StatType::NONE)
				{
					data.type = levelStatSet->statType30;
					data.value = levelStatSet->statValue30;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType31 != StatType::NONE)
				{
					data.type = levelStatSet->statType31;
					data.value = levelStatSet->statValue31;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType32 != StatType::NONE)
				{
					data.type = levelStatSet->statType32;
					data.value = levelStatSet->statValue32;
					addsomeLevel.statInfo.Emplace(data);
				}				
				if (levelStatSet->statType33 != StatType::NONE)
				{
					data.type = levelStatSet->statType33;
					data.value = levelStatSet->statValue33;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType34 != StatType::NONE)
				{
					data.type = levelStatSet->statType34;
					data.value = levelStatSet->statValue34;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType35 != StatType::NONE)
				{
					data.type = levelStatSet->statType35;
					data.value = levelStatSet->statValue35;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType36 != StatType::NONE)
				{
					data.type = levelStatSet->statType36;
					data.value = levelStatSet->statValue36;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType37 != StatType::NONE)
				{
					data.type = levelStatSet->statType37;
					data.value = levelStatSet->statValue37;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType38 != StatType::NONE)
				{
					data.type = levelStatSet->statType38;
					data.value = levelStatSet->statValue38;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType39 != StatType::NONE)
				{
					data.type = levelStatSet->statType39;
					data.value = levelStatSet->statValue39;
					addsomeLevel.statInfo.Emplace(data);
				}
				if (levelStatSet->statType40 != StatType::NONE)
				{
					data.type = levelStatSet->statType40;
					data.value = levelStatSet->statValue40;
					addsomeLevel.statInfo.Emplace(data);
				}
			}

			modifyed->levelList.Emplace(addsomeLevel);
		}
	};

	const UGsTableNpcBalanceExcel* ExcelData = Cast<UGsTableNpcBalanceExcel>(FGsSchemaNpcBalanceExcel::GetStaticTable());
	TArray< const FGsSchemaNpcBalanceExcel*> ExcelAllRow;
	if (false == ExcelData->GetAllRows(ExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableNpcBalanceExcel empty "));
		return false;;
	}

	const FGsSchemaNpcBalance* findTarget = nullptr;
	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (int i = 0; i < ExcelAllRow.Num(); ++i)
	{
		// UDataTable::AddRow will first remove the row if it already exists so we don't need to do anything special here
		if (const FGsSchemaNpcBalanceExcel* rowTable = ExcelAllRow[i])
		{
			FName rowName = FName(*FString::FromInt(rowTable->id));
			if ((findTarget = OriginalDataTable->FindRow<const FGsSchemaNpcBalance>(rowName, OriginalDataTable->GetName())))
			{
				////rank가 동일하지 않으면 오류(레벨정보이기 때무네
				//if (findTarget->rank != rowTable->rank)
				//{
				//	UE_LOG(LogTemp, Error, TEXT("UGsTableNpcBalance Rank error"));
				//	break;;
				//}

				fn_insertBalanceLevelInfo(const_cast<FGsSchemaNpcBalance*>(findTarget), rowTable);
			}
			else
			{
				FGsSchemaNpcBalance addsome;

				addsome.RowName = rowName;
				addsome.id		= rowTable->id;				
				
				fn_insertBalanceLevelInfo(&addsome, rowTable);
				
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

