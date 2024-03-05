#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "DataTableConverter/GsDataTableConvertBase.h"
#include "Event/EventAction/GsSchemaEventActionGroup.h"
#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"

#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h" 

#include "CoreUObject/Public/UObject/UnrealType.h"
#include "Core/Public/HAL/FileManager.h"
#include "Core/Public/GenericPlatform/GenericPlatformFile.h"

#include "DataSchema/NeverCook/Achievement/GsSchemaAchievementDataExcel.h"

#include "DataSchema/Achievement/GsSchemaAchievementCommon.h"
#include "DataSchema/Achievement/GsSchemaAchievementCondition.h"
#include "DataSchema/Achievement/GsSchemaAchievementDetail.h"

#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionCostume.h"
#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionCreature.h"
#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionCurrency.h"
#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionFairy.h"
#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionItem.h"
#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionQuest.h"
#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionRank.h"
#include "DataSchema/Achievement/AchievementCondition/GsSchemaAchievementConditionSkill.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"

#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
UE_LOG(LogTemp, Error, TEXT("[Achievement Converter] %s"), *Message.ToString());\
return false

const static FString rootDataTablePath = FString("/Game/DataCenter/DataTable");

//==========================================================================================================================================
// 변환할 엑셀 데이타
enum class EExcelSheetIndexAchievement
{
	AchievementData,
	Max,
};

const static TMap<EExcelSheetIndexAchievement, FString> achieveExcelSheetPath =
{
	{EExcelSheetIndexAchievement::AchievementData,	FString("NeverCook/Achievement/DTGsAchievementDataExcel.DTGsAchievementDataExcel")},
};

//==========================================================================================================================================
// 변환 대상의 Data Table
enum class EDataTableIndexAchievement : uint8 
{
	AchievementCommon,
	AchievementCondition,
	Max,
};

const static TMap<EDataTableIndexAchievement, FString> achieveDataTablePath =
{
	{EDataTableIndexAchievement::AchievementCommon,		FString("Achievement/DTGsAchievementCommon.DTGsAchievementCommon")},
	{EDataTableIndexAchievement::AchievementCondition,	FString("Achievement/DTGsAchievementCondition.DTGsAchievementCondition")},
};

bool ConvertAchievementData(const TMap<EExcelSheetIndexAchievement, UGsDataTable*>& excelSheetList, TMap<EDataTableIndexAchievement, UGsDataTable*>& dataTableList)
{
	const UGsDataTable* excelAchievementData = excelSheetList[EExcelSheetIndexAchievement::AchievementData];

	const auto detailTable = Cast<const UGsTableAchievementDetail>(FGsSchemaAchievementDetail::GetStaticTable());
	const auto rewardTable = Cast<const UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());

	if (detailTable == nullptr || rewardTable == nullptr) return false;

	static const FString ACHIEVEMENT_DETAIL_TABLE_NAME(TEXT("Achievement/DTGsAchievementDetail.DTGsAchievementDetail"));

	TMap<FString, UGsDataTable*> conditions = { TPair<FString, UGsDataTable*>{ "NONE", nullptr } };

	/*
	* 	static const UEnum* EnumType = StaticEnum<ETextCommit::Type>();
	*	FString strCommitMethod = EnumType->GetNameStringByValue(CommitMethod);
	*/

	auto FindEnumValue = [](IN const FString enumName, IN const FName enumValueName) {

		if (const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *enumName, true))
		{
			if (enumPtr->IsValidEnumName(enumValueName))
			{
				return enumPtr->GetValueByName(enumValueName, EGetByNameFlags::None);
			}
		}
		return (int64)-1;
	};

	auto FindEnumName = [](IN const FString enumName, IN const int64 enumValue) {

		if (const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *enumName, true))
		{
			if (enumPtr->IsValidEnumValue(enumValue))
			{
				return enumPtr->GetNameByValue(enumValue);
			}
		}
		return FName("");
	};

	auto IsValidEnum = [](IN const FString enumName, IN const int64 enumValue)->bool {

		if (const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *enumName, true))
		{
			return (enumPtr->IsValidEnumValue(enumValue) &&
					enumPtr->GetNameByValue(enumValue).IsNone() == false);
		}
		return false;
	};

	const auto conditionTable = dataTableList[EDataTableIndexAchievement::AchievementCondition];

	const TMap<FName, uint8*>& rowMap = excelAchievementData->GetRowMap();
	for (const auto& e : rowMap)
	{
		if (const auto excelData = reinterpret_cast<const FGsSchemaAchievementDataExcel*>(e.Value))
		{
			if ( excelData->achievementType == AchievementType::NONE || 
				IsValidEnum(TEXT("AchievementType"), static_cast<int64>(excelData->achievementType)) == false)
			{
				FText failMessage = FText::Format(NSLOCTEXT("FDataTableConverterAchievementExcelData", "InvalidEnum", "{0} : AchievementType이 잘못되었습니다."),
					FText::FromName(excelData->RowName));

				DEF_ERR_POPUP_MESSAGE(failMessage);
			}

			FGsSchemaAchievementCommon commonData;
			{	
				commonData.id = excelData->id;
				//commonData.category = excelData->category;
				commonData.groupId = excelData->groupId;
				commonData.level = excelData->level;
				commonData.isTheme = excelData->isTheme;
				commonData.completePoint = excelData->completePoint;
			
				commonData.detail = excelData->detail;
				commonData.rewardId = excelData->rewardId;

				FString tableFullName = conditions.Contains(excelData->conditionTableName) || excelData->conditionTableName.Contains(TEXT("DTGsAchievementCondition")) ?
					excelData->conditionTableName : FString::Format(TEXT("DTGsAchievementCondition{0}"), {excelData->conditionTableName }) ;

				if (conditions.Find(tableFullName) == nullptr)
				{
					UGsDataTable* table = nullptr;

					FString tablePath = FString::Format(TEXT("Achievement/AchievementCondition/{0}.{0}"), { tableFullName });
					if (FGsDataTableConvertBase::LoadDataTable(rootDataTablePath / tablePath, table))
					{
						conditions.Add(tableFullName, table);
					}
					else
					{
						FText failMessage = FText::Format(NSLOCTEXT("FDataTableConverterAchievementExcelData", "InvalidTable", "{0} : 로드할 수 없는 테이블{1}이 설정되어 있습니다."), 
							FText::FromName(excelData->RowName), FText::FromString(tableFullName));

						DEF_ERR_POPUP_MESSAGE(failMessage);
					}
				}

				if (auto iterTable = conditions.Find(tableFullName))
				{
					FGsSchemaAchievementCondition conditionInfo;
					{
						conditionInfo.conditionType = excelData->achievementType;

						if (auto table = (*iterTable))
						{
							const auto conditionDataRowMap = table->GetRowMap();
							if (conditionDataRowMap.Find(excelData->conditionRowName) == nullptr)
							{	
								FText failedMessage = FText::Format(
									NSLOCTEXT("FDataTableConverterAchievementExcelData", "InvalidRow", "{0} : 설정된 {1}테이블에 {2} Row가 존재하지 않습니다."),
										FText::FromName(excelData->RowName), FText::FromString(tableFullName), FText::FromName(excelData->conditionRowName) );

								DEF_ERR_POPUP_MESSAGE(failedMessage);
							}

							conditionInfo.conditionValue.DataTable = table;
							conditionInfo.conditionValue.RowName = excelData->conditionRowName;
						}

						FString conditionRaw = FindEnumName(TEXT("AchievementType"), static_cast<int64>(conditionInfo.conditionType)).ToString();
						conditionRaw.RemoveFromStart(TEXT("AchievementType::"));

						conditionRaw = conditionRaw + TEXT("::") + excelData->conditionRowName.ToString();

						FName conditionRowName = FName(*conditionRaw);
						const TMap<FName, uint8*>& conditionRowMap = conditionTable->GetRowMap();

						if (conditionRowMap.Contains(conditionRowName) == false)
						{
							conditionTable->AddRow(conditionRowName, conditionInfo);
						}

						commonData.achievementCondition.DataTable = conditionTable;
						commonData.achievementCondition.RowName   = conditionRowName;
					}
				}

				dataTableList[EDataTableIndexAchievement::AchievementCommon]->AddRow(excelData->RowName, commonData);
			}
		}
	}
	return true;
}

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterAchievementExcelData)
bool FDataTableConverterAchievementExcelData::Convert()
{
	TMap<EDataTableIndexAchievement, UGsDataTable*> dataTableList;
	for (const auto& e : achieveDataTablePath)
	{
		if (false == FGsDataTableConvertBase::LoadDataTable(rootDataTablePath / e.Value, dataTableList.FindOrAdd(e.Key)))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found DT TableAsset : [%s]"), *(e.Value)));
			DEF_ERR_POPUP_MESSAGE(message);
		}
	}

	TMap<EExcelSheetIndexAchievement, UGsDataTable*> excelSheetList;
	for (const auto& e : achieveExcelSheetPath)
	{
		if (false == FGsDataTableConvertBase::LoadDataTable(rootDataTablePath / e.Value, excelSheetList.FindOrAdd(e.Key)))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found ExcelSheet TableAsset : [%s]"), *(e.Value)));
			DEF_ERR_POPUP_MESSAGE(message);
		}
	}

	for (auto& pair : dataTableList)
	{
		pair.Value->EmptyTable();
	}
	
	if (ConvertAchievementData(excelSheetList, dataTableList))
	{
		for (auto& pair : dataTableList)
		{
			pair.Value->SavePackage();
		}

		return true;
	}

	return false;
}