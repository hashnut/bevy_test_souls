
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "RunTime/Engine/Classes/Engine/CompositeDataTable.h"

#include "DataTableConverter/GsDataTableConvertBase.h"

#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"

#include "DataSchema/NeverCook/Reward/GsSchemaRewardDataExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxDataExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardItemBagDataExcel.h"

#include "DataSchema/NeverCook/Reward/GsSchemaRewardItemBagDataItemExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardItemBagDataQuestExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardItemBagDataEtcExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardItemBagDataBMExcel.h"

#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxDataItemExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxDataQuestExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxDataEtcExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxDataBMExcel.h"

#include "DataSchema/NeverCook/Reward/GsSchemaRewardDataItemExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardDataQuestExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardDataEtcExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardDataBMExcel.h"

#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxItemExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxQuestExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxEtcExcel.h"
#include "DataSchema/NeverCook/Reward/GsSchemaRewardBoxBMExcel.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"


// 원본 데이터테이블의 Package Name
static const FString RWEARD_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/Reward/DTGsRewardData.DTGsRewardData"));
static const FString RWEARD_BOXDATA_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/Reward/DTGsRewardBoxData.DTGsRewardBoxData"));

void SetRewardItemBagData(FGsSchemaRewardItemBagData& InItemBagData, const FGsSchemaRewardItemBagDataExcel* InItemBagDataExcel)
{
	InItemBagData.itemBagPairId = InItemBagDataExcel->id;
	InItemBagData.currencyType = InItemBagDataExcel->currencyType;
	InItemBagData.currencyAmountMin = InItemBagDataExcel->currencyAmountMin;
	InItemBagData.currencyAmountMax = InItemBagDataExcel->currencyAmountMax;
	InItemBagData.itemId = InItemBagDataExcel->itemId;
	InItemBagData.itemCountMin = InItemBagDataExcel->itemCountMin;
	InItemBagData.itemCountMax = InItemBagDataExcel->itemCountMax;
	InItemBagData.probability = InItemBagDataExcel->probability;
	InItemBagData.itemLevel = InItemBagDataExcel->itemLevel;
	InItemBagData.enchantShield = InItemBagDataExcel->enchantShield;
	InItemBagData.enchantBonusIndex = InItemBagDataExcel->enchantBonusIndex;
}

void AddRewardItemBagData(int64 InId, FGsSchemaRewardItemBagData& InRewardItemBagData, UGsDataTable* InRewardBoxDataTable, TMap<int64, FName>& InRewardBoxDataMap)
{
	FName rowName = FName(*FString::FromInt(InId));
	if (const FGsSchemaRewardBoxData* findRewardBoxDataTarget = InRewardBoxDataTable->FindRow<const FGsSchemaRewardBoxData>(rowName, InRewardBoxDataTable->GetName(), false))
	{
		// 동일한 Id 존재
		const_cast<FGsSchemaRewardBoxData*>(findRewardBoxDataTarget)->itemBagList.Emplace(InRewardItemBagData);
	}
	else
	{
		// 동일한 Id 미존재. 신규 추가
		FGsSchemaRewardBoxData addRewardBoxData;
		addRewardBoxData.RowName = rowName;
		addRewardBoxData.id = InId;

		addRewardBoxData.itemBagList.Emplace(InRewardItemBagData);
		InRewardBoxDataTable->AddRow(addRewardBoxData.RowName, addRewardBoxData);

		// FIX: 같은정보 있으면 뻑날것
		InRewardBoxDataMap.Emplace(addRewardBoxData.id, addRewardBoxData.RowName);
	}
}

void InsertRewardItemBagDataExcel(int64 InId, const FGsSchemaBase* InItemBagDataExcelRow, UGsDataTable* InRewardBoxDataTable, TMap<int64, FName>& InRewardBoxDataMap)
{
	FGsSchemaRewardItemBagData newItemBagData;
	const FGsSchemaRewardItemBagDataExcel* itemBagDataExcel = static_cast<const FGsSchemaRewardItemBagDataExcel*>(InItemBagDataExcelRow);

	SetRewardItemBagData(newItemBagData, itemBagDataExcel);
	AddRewardItemBagData(InId, newItemBagData, InRewardBoxDataTable, InRewardBoxDataMap);
}

void AddRewardData(FGsSchemaRewardData* InRealData, const FGsSchemaRewardDataExcel* InExcelData, const FGsSchemaRewardBoxExcel* InRewardBoxExcel, UGsDataTable* InRewardBoxDataTable, TMap<int64, FName>& InRewardBoxDataMap)
{
	InRealData->id = InExcelData->id;
	InRealData->expMin = InExcelData->expMin;
	InRealData->expMax = InExcelData->expMax;
	InRealData->currencyType = InExcelData->currencyType;
	InRealData->currencyAmountMin = InExcelData->currencyAmountMin;
	InRealData->currencyAmountMax = InExcelData->currencyAmountMax;
	InRealData->rewardType = InExcelData->rewardType;

	FGsSchemaRewardBox newBox;
	newBox.rewardBoxId = InRewardBoxExcel->rewardBoxId;
	newBox.rate = InRewardBoxExcel->rate;

	// RowPicker 채우기
	newBox.rewardBoxDataRow.DataTable = InRewardBoxDataTable;
	if (FName* findName = InRewardBoxDataMap.Find(InRewardBoxExcel->rewardBoxId))
	{
		newBox.rewardBoxDataRow.RowName = *findName;
	}

	InRealData->rewardBoxList.Emplace(newBox);
}

void InsertRewardDataExcel(const FGsSchemaBase* InRewardDataExcelRow, UGsDataTable* InRewardDataTable, UGsDataTable* InRewardBoxDataTable, TMap<int64, FName>& InRewardBoxDataMap, const FGsSchemaBase* InRewardBoxExcelRow)
{
	const FGsSchemaRewardDataExcel* RewardDataExcelRow = static_cast<const FGsSchemaRewardDataExcel*>(InRewardDataExcelRow);
	if (nullptr == RewardDataExcelRow)
	{
		// 캐스팅 에러
		return;
	}

	const FGsSchemaRewardBoxExcel* RewardBoxExcelRow = static_cast<const FGsSchemaRewardBoxExcel*>(InRewardBoxExcelRow);
	if (nullptr == RewardBoxExcelRow)
	{
		// 캐스팅 에러
		return;
	}

	// bak1210 : 기존이 입력된 로우피커를 살리기위해 로우Name은 엑셀에서 입력된 데이터를 쓰도록 한다.
	TArray<FString> RealName;
	RewardDataExcelRow->RowName.ToString().ParseIntoArray(RealName, TEXT("."));
	FName rowName = FName(*RealName[0]);
	if (const FGsSchemaRewardData* findTarget = InRewardDataTable->FindRow<const FGsSchemaRewardData>(rowName, InRewardDataTable->GetName(), false))
	{
		AddRewardData(const_cast<FGsSchemaRewardData*>(findTarget), RewardDataExcelRow, RewardBoxExcelRow, InRewardBoxDataTable, InRewardBoxDataMap);
	}
	else
	{
		FGsSchemaRewardData addRewardData;
		addRewardData.RowName = rowName;
		AddRewardData(&addRewardData, RewardDataExcelRow, RewardBoxExcelRow, InRewardBoxDataTable, InRewardBoxDataMap);

		InRewardDataTable->AddRow(addRewardData.RowName, addRewardData);
	}
}

bool ConvertRewardBoxData(UGsDataTable* InRewardBoxDataTable, TMap<int64, FName>& InRewardBoxDataMap)
{
	// Load RewardItemBagData Tables
	const UGsTableRewardItemBagDataItemExcel* itemBagDataItemExcel = Cast<UGsTableRewardItemBagDataItemExcel>(FGsSchemaRewardItemBagDataItemExcel::GetStaticTable());
	if (nullptr == itemBagDataItemExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardItemBagDataItemExcel Error "));
		return false;
	}
	const UGsTableRewardItemBagDataQuestExcel* itemBagDataQuestExcel = Cast<UGsTableRewardItemBagDataQuestExcel>(FGsSchemaRewardItemBagDataQuestExcel::GetStaticTable());
	if (nullptr == itemBagDataQuestExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardItemBagDataQuestExcel Error "));
		return false;
	}
	const UGsTableRewardItemBagDataEtcExcel* itemBagDataEtcExcel = Cast<UGsTableRewardItemBagDataEtcExcel>(FGsSchemaRewardItemBagDataEtcExcel::GetStaticTable());
	if (nullptr == itemBagDataEtcExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardItemBagDataEtcExcel Error "));
		return false;
	}
	const UGsTableRewardItemBagDataBMExcel* itemBagDataBMExcel = Cast<UGsTableRewardItemBagDataBMExcel>(FGsSchemaRewardItemBagDataBMExcel::GetStaticTable());
	if (nullptr == itemBagDataBMExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardItemBagDataBMExcel Error "));
		return false;
	}

	// Load RewardBoxData Tables
	const UGsTableRewardBoxDataItemExcel* RewardBoxDataItemExcel = Cast<UGsTableRewardBoxDataItemExcel>(FGsSchemaRewardBoxDataItemExcel::GetStaticTable());
	TArray<const FGsSchemaRewardBoxDataItemExcel*> RewardBoxDataItemExcelAllRow;
	if (false == RewardBoxDataItemExcel->GetAllRows(RewardBoxDataItemExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxDataItemExcel empty "));
		return false;
	}
	const UGsTableRewardBoxDataQuestExcel* RewardBoxDataQuestExcel = Cast<UGsTableRewardBoxDataQuestExcel>(FGsSchemaRewardBoxDataQuestExcel::GetStaticTable());
	TArray<const FGsSchemaRewardBoxDataQuestExcel*> RewardBoxDataQuestExcelAllRow;
	if (false == RewardBoxDataQuestExcel->GetAllRows(RewardBoxDataQuestExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxDataQuestExcel empty "));
		return false;
	}
	const UGsTableRewardBoxDataEtcExcel* RewardBoxDataEtcExcel = Cast<UGsTableRewardBoxDataEtcExcel>(FGsSchemaRewardBoxDataEtcExcel::GetStaticTable());
	TArray<const FGsSchemaRewardBoxDataEtcExcel*> RewardBoxDataEtcExcelAllRow;
	if (false == RewardBoxDataEtcExcel->GetAllRows(RewardBoxDataEtcExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxDataEtcExcel empty "));
		return false;
	}
	const UGsTableRewardBoxDataBMExcel* RewardBoxDataBMExcel = Cast<UGsTableRewardBoxDataBMExcel>(FGsSchemaRewardBoxDataBMExcel::GetStaticTable());
	TArray<const FGsSchemaRewardBoxDataBMExcel*> RewardBoxDataBMExcelAllRow;
	if (false == RewardBoxDataBMExcel->GetAllRows(RewardBoxDataBMExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxDataBMExcel empty "));
		return false;
	}

	auto fn_FindRow = [itemBagDataItemExcel, itemBagDataQuestExcel, itemBagDataEtcExcel, itemBagDataBMExcel](int64 InItemBagListData)->const FGsSchemaBase*
	{
		const FGsSchemaRewardItemBagDataItemExcel* itemExcelRow = nullptr;
		if (itemBagDataItemExcel->FindRowById(InItemBagListData, itemExcelRow))
		{
			return itemExcelRow;
		}
		const FGsSchemaRewardItemBagDataQuestExcel* questExcelRow = nullptr;
		if (itemBagDataQuestExcel->FindRowById(InItemBagListData, questExcelRow))
		{
			return questExcelRow;
		}
		const FGsSchemaRewardItemBagDataEtcExcel* etcExcelRow = nullptr;
		if (itemBagDataEtcExcel->FindRowById(InItemBagListData, etcExcelRow))
		{
			return etcExcelRow;
		}
		const FGsSchemaRewardItemBagDataBMExcel* BMExcelRow = nullptr;
		if (itemBagDataBMExcel->FindRowById(InItemBagListData, BMExcelRow))
		{
			return BMExcelRow;
		}
		return nullptr;
	};

	for (int i = 0; i < RewardBoxDataItemExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardBoxDataItemExcel* rowExcelTable = RewardBoxDataItemExcelAllRow[i])
		{
			const FGsSchemaBase* itemBagDataExcelRow = fn_FindRow(rowExcelTable->itemBagList);
			if (nullptr == itemBagDataExcelRow)
				continue;

			InsertRewardItemBagDataExcel(rowExcelTable->id, itemBagDataExcelRow, InRewardBoxDataTable, InRewardBoxDataMap);
		}
	}
	for (int i = 0; i < RewardBoxDataQuestExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardBoxDataQuestExcel* rowExcelTable = RewardBoxDataQuestExcelAllRow[i])
		{
			const FGsSchemaBase* itemBagDataExcelRow = fn_FindRow(rowExcelTable->itemBagList);
			if (nullptr == itemBagDataExcelRow)
				continue;

			InsertRewardItemBagDataExcel(rowExcelTable->id, itemBagDataExcelRow, InRewardBoxDataTable, InRewardBoxDataMap);
		}
	}
	for (int i = 0; i < RewardBoxDataEtcExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardBoxDataEtcExcel* rowExcelTable = RewardBoxDataEtcExcelAllRow[i])
		{
			const FGsSchemaBase* itemBagDataExcelRow = fn_FindRow(rowExcelTable->itemBagList);
			if (nullptr == itemBagDataExcelRow)
				continue;

			InsertRewardItemBagDataExcel(rowExcelTable->id, itemBagDataExcelRow, InRewardBoxDataTable, InRewardBoxDataMap);
		}
	}
	for (int i = 0; i < RewardBoxDataBMExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardBoxDataBMExcel* rowExcelTable = RewardBoxDataBMExcelAllRow[i])
		{
			const FGsSchemaBase* itemBagDataExcelRow = fn_FindRow(rowExcelTable->itemBagList);
			if (nullptr == itemBagDataExcelRow)
				continue;

			InsertRewardItemBagDataExcel(rowExcelTable->id, itemBagDataExcelRow, InRewardBoxDataTable, InRewardBoxDataMap);
		}
	}

	return true;
}

bool ConvertRewardData(UGsDataTable* InRewardDataTable, UGsDataTable* InRewardBoxDataTable, TMap<int64, FName>& InRewardBoxDataMap)
{
	// Load RewardData Tables
	const UGsTableRewardDataItemExcel* RewardDataItemExcel = Cast<UGsTableRewardDataItemExcel>(FGsSchemaRewardDataItemExcel::GetStaticTable());
	TArray<const FGsSchemaRewardDataItemExcel*> RewardDataItemExcelAllRow;
	if (false == RewardDataItemExcel->GetAllRows(RewardDataItemExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardDataItemExcel empty "));
		return false;
	}
	const UGsTableRewardDataQuestExcel* RewardDataQuestExcel = Cast<UGsTableRewardDataQuestExcel>(FGsSchemaRewardDataQuestExcel::GetStaticTable());
	TArray<const FGsSchemaRewardDataQuestExcel*> RewardDataQuestExcelAllRow;
	if (false == RewardDataQuestExcel->GetAllRows(RewardDataQuestExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardDataQuestExcel empty "));
		return false;
	}
	const UGsTableRewardDataEtcExcel* RewardDataEtcExcel = Cast<UGsTableRewardDataEtcExcel>(FGsSchemaRewardDataEtcExcel::GetStaticTable());
	TArray<const FGsSchemaRewardDataEtcExcel*> RewardDataEtcExcelAllRow;
	if (false == RewardDataEtcExcel->GetAllRows(RewardDataEtcExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardDataEtcExcel empty "));
		return false;
	}
	const UGsTableRewardDataBMExcel* RewardDataBMExcel = Cast<UGsTableRewardDataBMExcel>(FGsSchemaRewardDataBMExcel::GetStaticTable());
	TArray<const FGsSchemaRewardDataBMExcel*> RewardDataBMExcelAllRow;
	if (false == RewardDataBMExcel->GetAllRows(RewardDataBMExcelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardDataBMExcel empty "));
		return false;
	}

	//const UGsTableRewardBoxExcel* RewardBoxExcel = Cast<UGsTableRewardBoxExcel>(FGsSchemaRewardBoxExcel::GetStaticTable());
	//if (nullptr == RewardBoxExcel)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxExcel empty "));
	//	return false;
	//}
	const UGsTableRewardBoxItemExcel* RewardBoxItemExcel = Cast<UGsTableRewardBoxItemExcel>(FGsSchemaRewardBoxItemExcel::GetStaticTable());
	if (nullptr == RewardBoxItemExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxItemExcel empty "));
		return false;
	}
	const UGsTableRewardBoxQuestExcel* RewardBoxQuestExcel = Cast<UGsTableRewardBoxQuestExcel>(FGsSchemaRewardBoxQuestExcel::GetStaticTable());
	if (nullptr == RewardBoxQuestExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxQuestExcel empty "));
		return false;
	}
	const UGsTableRewardBoxEtcExcel* RewardBoxEtcExcel = Cast<UGsTableRewardBoxEtcExcel>(FGsSchemaRewardBoxEtcExcel::GetStaticTable());
	if (nullptr == RewardBoxEtcExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxEtcExcel empty "));
		return false;
	}
	const UGsTableRewardBoxBMExcel* RewardBoxBMExcel = Cast<UGsTableRewardBoxBMExcel>(FGsSchemaRewardBoxBMExcel::GetStaticTable());
	if (nullptr == RewardBoxBMExcel)
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableRewardBoxBMExcel empty "));
		return false;
	}

	auto fn_FindRewardBoxExcelRow = [RewardBoxItemExcel, RewardBoxQuestExcel, RewardBoxEtcExcel, RewardBoxBMExcel](int64 InRewardBoxId)->const FGsSchemaBase*
	{
		const FGsSchemaRewardBoxItemExcel* rewardBoxItemExcelRow = nullptr;
		if (RewardBoxItemExcel->FindRowById(InRewardBoxId, rewardBoxItemExcelRow))
		{
			return rewardBoxItemExcelRow;
		}
		const FGsSchemaRewardBoxQuestExcel* rewardBoxQuestExcelRow = nullptr;
		if (RewardBoxQuestExcel->FindRowById(InRewardBoxId, rewardBoxQuestExcelRow))
		{
			return rewardBoxQuestExcelRow;
		}
		const FGsSchemaRewardBoxEtcExcel* rewardBoxEtcExcelRow = nullptr;
		if (RewardBoxEtcExcel->FindRowById(InRewardBoxId, rewardBoxEtcExcelRow))
		{
			return rewardBoxEtcExcelRow;
		}
		const FGsSchemaRewardBoxBMExcel* rewardBoxBMExcelRow = nullptr;
		if (RewardBoxBMExcel->FindRowById(InRewardBoxId, rewardBoxBMExcelRow))
		{
			return rewardBoxBMExcelRow;
		}
		return nullptr;
	};

	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (int i = 0; i < RewardDataItemExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardDataItemExcel* rowExcelTable = RewardDataItemExcelAllRow[i])
		{
			const FGsSchemaBase* findRewardBoxExcelRow = fn_FindRewardBoxExcelRow(rowExcelTable->rewardBoxPairId);
			if (nullptr == findRewardBoxExcelRow)
				continue;

			InsertRewardDataExcel(rowExcelTable, InRewardDataTable, InRewardBoxDataTable, InRewardBoxDataMap, findRewardBoxExcelRow);
		}
	}
	for (int i = 0; i < RewardDataQuestExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardDataQuestExcel* rowExcelTable = RewardDataQuestExcelAllRow[i])
		{
			const FGsSchemaBase* findRewardBoxExcelRow = fn_FindRewardBoxExcelRow(rowExcelTable->rewardBoxPairId);
			if (nullptr == findRewardBoxExcelRow)
				continue;

			InsertRewardDataExcel(rowExcelTable, InRewardDataTable, InRewardBoxDataTable, InRewardBoxDataMap, findRewardBoxExcelRow);
		}
	}
	for (int i = 0; i < RewardDataEtcExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardDataEtcExcel* rowExcelTable = RewardDataEtcExcelAllRow[i])
		{
			const FGsSchemaBase* findRewardBoxExcelRow = fn_FindRewardBoxExcelRow(rowExcelTable->rewardBoxPairId);
			if (nullptr == findRewardBoxExcelRow)
				continue;

			InsertRewardDataExcel(rowExcelTable, InRewardDataTable, InRewardBoxDataTable, InRewardBoxDataMap, findRewardBoxExcelRow);
		}
	}
	for (int i = 0; i < RewardDataBMExcelAllRow.Num(); ++i)
	{
		if (const FGsSchemaRewardDataBMExcel* rowExcelTable = RewardDataBMExcelAllRow[i])
		{
			const FGsSchemaBase* findRewardBoxExcelRow = fn_FindRewardBoxExcelRow(rowExcelTable->rewardBoxPairId);
			if (nullptr == findRewardBoxExcelRow)
				continue;

			InsertRewardDataExcel(rowExcelTable, InRewardDataTable, InRewardBoxDataTable, InRewardBoxDataMap, findRewardBoxExcelRow);
		}
	}
	return true;
}

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterReward)
bool FDataTableConverterReward::Convert()
{
	//------------------------------------------------------------------------------------------------------------------
	// RewardBoxData 테이블 세팅
	UGsDataTable* OriginalRewardBoxDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(RWEARD_BOXDATA_DATATABLE_PACKAGE_NAME, OriginalRewardBoxDataTable))
	{
		return false;
	}
	OriginalRewardBoxDataTable->EmptyTable();

	// 데이터 변환
	TMap<int64, FName> rewardBoxDataMap;
	if (false == ConvertRewardBoxData(OriginalRewardBoxDataTable, rewardBoxDataMap))
	{
		// 일부 테이블 읽기 중 실패
		return false;
	}
	
	//------------------------------------------------------------------------------------------------------------------
	// RewardData 테이블 세팅
	UGsDataTable* OriginalRewardTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(RWEARD_DATATABLE_PACKAGE_NAME, OriginalRewardTable))
	{
		return false;
	}
	OriginalRewardTable->EmptyTable();

	// 데이터 변환
	if (false == ConvertRewardData(OriginalRewardTable, OriginalRewardBoxDataTable, rewardBoxDataMap))
	{
		// 일부 테이블 읽기 중 실패
		return false;
	}

	// 원본 데이터테이블 저장
	if (!OriginalRewardTable->SavePackage())
	{
		UE_LOG(LogTemp, Error, TEXT("!OriginalRewardTable->SavePackage(), OriginalRewardTable :%s"), *OriginalRewardTable->GetName());
		return false;
	}
	if (!OriginalRewardBoxDataTable->SavePackage())
	{
		UE_LOG(LogTemp, Error, TEXT("!OriginalRewardBoxDataTable->SavePackage(), OriginalRewardBoxDataTable :%s"), *OriginalRewardBoxDataTable->GetName());
		return false;
	}

	return true;
}
