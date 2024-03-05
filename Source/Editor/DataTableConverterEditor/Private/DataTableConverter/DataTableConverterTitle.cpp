#include "DataTableConverter/GsDataTableConvertBase.h"

#include "GsDataTable.h"

#include "DataSchema/Title/GsSchemaTitle.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataSkill.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataRank.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataQuest.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataItem.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataGuild.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataFairy.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataEtc.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataCurrency.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataCreature.h"
#include "DataSchema/Title/GsSchemaTitleConditionDataCostume.h"

#include "DataSchema/NeverCook/Title/GsSchemaTitleExcel.h"

#include "Slate/Public/Widgets/Notifications/SNotificationList.h"
#include "Slate/Public/Framework/Notifications/NotificationManager.h"

#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
return false


IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterTitleExcelData)
bool FDataTableConverterTitleExcelData::Convert()
{
	// 저장할 데이터 테이블 존재 확인
	UGsDataTable* DTGsTitle = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Title/DTGsTitle.DTGsTitle"),
		DTGsTitle))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsTitle)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	
	// 변환할 엑셀 데이터 테이블 존재 확인
	UGsDataTable* DTGsTitleExcel = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/NeverCook/Title/DTGsTitleExcel.DTGsTitleExcel"),
		DTGsTitleExcel))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsTitleExcel)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	const UGsTableTitleExcel* titleExcel = Cast<UGsTableTitleExcel>(FGsSchemaTitleExcel::GetStaticTable());
	if (nullptr == titleExcel)
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsTitle)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	TArray<const FGsSchemaTitleExcel*> excelAllRow;
	if (false == titleExcel->GetAllRows(excelAllRow))
	{
		UE_LOG(LogTemp, Error, TEXT("UGsTableTitleExcel Empty "));
		return false;
	}

	// 타이틀 조건 데이터 테이블 기본 경로 및 이름
	const FString defaultPath = FString("/Game/DataCenter/DataTable/Title/");
	const FString defaultName = FString("DTGsTitleConditionData");

	// 저장할 테이블 비우기
	DTGsTitle->EmptyTable();
	
	// 엑셀 데이터 테이블에서 게임용 데이터 테이블로 변환
	for (const FGsSchemaTitleExcel* row : excelAllRow)
	{
		if (row)
		{
			FGsSchemaTitle newTitleData;
			newTitleData.id = row->id;
			newTitleData.subCategoryRowId = row->subCategoryRowId;
			newTitleData.name = row->name;
			newTitleData.grade = row->grade;
			newTitleData.equipSlotType = row->equipSlotType;
			newTitleData.passivitySetId = row->passivitySetId;
			newTitleData.passivityDesc = row->passivityDesc;
			newTitleData.desc = row->desc;
			newTitleData.isProgressDisplayFlag = row->isProgressDisplayFlag;
			newTitleData.retainType = row->retainType;
			newTitleData.retainCheckType = row->retainCheckType;
			newTitleData.conditionType = row->conditionType;
			newTitleData.conditionDesc = row->conditionDesc;
			newTitleData.conditionalCount = row->conditionalCount;

			// XOR 체크
			if ((!row->conditionTableType.IsEmpty() && row->conditionTableRowName.IsEmpty()) ||
				(row->conditionTableType.IsEmpty() && !row->conditionTableRowName.IsEmpty()))
			{
				FText message = FText::FromString(FString::Printf(TEXT("(Title.xlsx) Data ID : %d, Path Column Error"), row->id));
				DEF_ERR_POPUP_MESSAGE(message);
			}
			
			// (기본 경로)(데이터 테이블 이름).(데이터 테이블 이름)
			FString dataTableName = defaultName + row->conditionTableType;
			FString conditionDataTablePath = FString::Printf(TEXT("%s%s.%s"), *defaultPath, *dataTableName, *dataTableName);

			UGsDataTable* conditionDataTable = nullptr;
			if (FGsDataTableConvertBase::LoadDataTable(conditionDataTablePath, conditionDataTable))
			{
				const FGsSchemaBase* dataRow = conditionDataTable->FindRow<FGsSchemaBase>(FName(*row->conditionTableRowName), conditionDataTable->GetName(), false);
				if (nullptr == dataRow)
				{
					FText message = FText::FromString(FString::Printf(TEXT("%s Row Cannot Found In (%s)"), *row->conditionTableRowName, *dataTableName));
					DEF_ERR_POPUP_MESSAGE(message);
				}
			}
			newTitleData.titleConditionData.DataTable = conditionDataTable;
			newTitleData.titleConditionData.RowName = FName(*row->conditionTableRowName);

			FName rowName = FName(*FString::Printf(TEXT("%d"), newTitleData.id));
			DTGsTitle->AddRow(rowName, newTitleData);
		}
	}

	// Save Table
	if (!DTGsTitle->SavePackage())
	{
		UE_LOG(LogTemp, Error, TEXT("!DTGsTitle->SavePackage(), DTGsTitle:%s"), *DTGsTitle->GetName());
		return false;
	}

	return true;
}