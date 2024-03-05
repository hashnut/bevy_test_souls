#include "DataTableConverter/GsDataTableConvertBase.h"

#include "GsDataTable.h"
#include "NotificationManager.h"
#include "DataSchema/NeverCook/GsSchemaTestApplyStatTypeData.h"
#include "DataSchema/NeverCook/GsSchemaTestApplyStatData.h"
#include "DataSchema/Cheat/GsSchemaCheatSetData.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"


#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
return false


IMPLEMENT_DATATABLE_CONVERTER(DataTableConverterTestStatSet)
bool DataTableConverterTestStatSet::Convert()
{
	// 저장할 데이터 테이블 존재 확인
	UGsDataTable* DTGsCheatSetData = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Cheat/DTGsCheatSetData.DTGsCheatSetData"),
		DTGsCheatSetData))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsCheatSetData)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	
	// 변환할 엑셀 데이터 테이블 존재 확인
	UGsDataTable* DTGsTestApplyStatData = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/NeverCook/DTGsTestApplyStatData.DTGsTestApplyStatData"),
		DTGsTestApplyStatData))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsTestApplyStatData)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	UGsDataTable* DTGsTestApplyStatTypeData = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/NeverCook/DTGsTestApplyStatTypeData.DTGsTestApplyStatTypeData"),
		DTGsTestApplyStatTypeData))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsTestApplyStatTypeData)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	
	const UGsTableTestApplyStatData* TestApplyStatData = Cast<UGsTableTestApplyStatData>(FGsSchemaTestApplyStatData::GetStaticTable());
	if (nullptr == TestApplyStatData)
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(TestApplyStatData)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	const UGsTableTestApplyStatTypeData* TestApplyStatTypeData = Cast<UGsTableTestApplyStatTypeData>(FGsSchemaTestApplyStatTypeData::GetStaticTable());
	if (nullptr == TestApplyStatTypeData)
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(TestApplyStatTypeData)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	

	TArray<const FGsSchemaTestApplyStatData*> statAllRow;
	if (false == TestApplyStatData->GetAllRows(statAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableTestApplyStatData Empty "));
		return false;
	}
	
	// StatType 구분하기위한 컬럼에 해당 하는 값을 얻어옴
	TArray< TArray<FString> > statTypeColums = DTGsTestApplyStatTypeData->GetTableData();
	if (statTypeColums.Num() < 1)
	{
		FText message = FText::FromString(TEXT(" TestApplyStatTypeData에 값이 없습니다."));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	TArray< TArray<FString> > statValueColums = DTGsTestApplyStatData->GetTableData();

	for (int i = 1; i < statValueColums.Num(); ++i)
	{
		// 0번 인덱스는 RowName
		for (int j = 1; j < statTypeColums[1].Num(); ++j)
		{
			// 유니크한 RowName을 만들고 기존 테이블에 존재하는지 확인한다.
			// 구분접두어 . RowName. StatType String
			int32 statTypeInt = FCString::Atoi(*statTypeColums[1][j]);
			FString statTypeKey = StatTypeEnumToString((StatType)statTypeInt);

			FName rowName = FName(*FString::Printf(TEXT("TestApplyStatData.%s.%s"), *statValueColums[i][0], *statTypeKey));
			FGsSchemaCheatSetData* cheatSetData = DTGsCheatSetData->FindRow<FGsSchemaCheatSetData>(rowName, FString());
			if (nullptr == cheatSetData)
			{
				FGsSchemaCheatSetData newInfo;
				DTGsCheatSetData->AddRow(rowName, newInfo);
				cheatSetData = DTGsCheatSetData->FindRow<FGsSchemaCheatSetData>(rowName, FString());
			}

			cheatSetData->Argument.Empty();

			cheatSetData->Command = TEXT("setqastat");
			cheatSetData->Desciption = statTypeKey;

			FGsSchemaCheatArgu newArgInfo1;
			newArgInfo1.Type = EGsCheatArguType::int32;
			newArgInfo1.TypeInt32Value = statTypeInt;
			cheatSetData->Argument.Add(newArgInfo1);

			FGsSchemaCheatArgu newArgInfo2;
			newArgInfo2.Type = EGsCheatArguType::int32;
			newArgInfo2.TypeInt32Value = FCString::Atoi(*statValueColums[i][j]);
			cheatSetData->Argument.Add(newArgInfo2);
		}
	}

	return true;
}