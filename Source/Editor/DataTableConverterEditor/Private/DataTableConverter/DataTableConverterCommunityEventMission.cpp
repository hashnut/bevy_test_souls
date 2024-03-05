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

#include "DataSchema/NeverCook/CommunityGirl/Mission/GsSchemaCommunityEventMissionDataExcel.h"
#include "DataSchema/NeverCook/CommunityGirl/Mission/GsSchemaCommunityEventMissionExcel.h"
#include "DataSchema/NeverCook/CommunityGirl/Mission/GsSchemaCommunityEventMissionTabExcel.h"

#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMissionTab.h"
#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMissionCommon.h"
#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMissionCondition.h"
#include "DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMission.h"

#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "Transform.h"


#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
return false

const static FString DataTablePath = FString("/Game/DataCenter/DataTable");

//==========================================================================================================================================
// 변환 대상의 Data Table
enum class ECommunityEventMissionDataTableIndex : uint8 {
	
	CommunityEventMissionCommon,
	CommunityEventMissionTab,
	CommunityEventMissionCondition,
	CommunityEventMissionConditionSkill,
	CommunityEventMissionConditionQuest,
	CommunityEventMissionConditionMonster,
	CommunityEventMissionConditionItem,
	CommunityEventMissionConditionFairy,
	CommunityEventMissionConditionETC,
	CommunityEventMissionConditionCostume,
	CommunityEventMissionConditionCurrency,
	CommunityEventMission,
	Max,
};

const static TMap<ECommunityEventMissionDataTableIndex, FString> eventDataTablePath =
{
	{ECommunityEventMissionDataTableIndex::CommunityEventMissionCommon,		FString("CommunityGirl/Mission/DTGsCommunityEventMissionCommon.DTGsCommunityEventMissionCommon")},
	{ECommunityEventMissionDataTableIndex::CommunityEventMissionTab,		FString("CommunityGirl/Mission/DTGsCommunityEventMissionTab.DTGsCommunityEventMissionTab")},
	{ECommunityEventMissionDataTableIndex::CommunityEventMissionCondition,	FString("CommunityGirl/Mission/DTGsCommunityEventMissionCondition.DTGsCommunityEventMissionCondition")},
	{ECommunityEventMissionDataTableIndex::CommunityEventMission,	FString("CommunityGirl/Mission/DTGsCommunityEventMission.DTGsCommunityEventMission")},
};
//==========================================================================================================================================

enum class EExcelSheetIndexCommunityEvent
{	
	CommunityEventMissionData,
	CommunityEventMissionTab,
	CommunityEventMission,
	Max,
};

const static TMap<EExcelSheetIndexCommunityEvent, FString> excelSheetPath =
{
	{EExcelSheetIndexCommunityEvent::CommunityEventMissionData,	FString("NeverCook/CommunityGirl/Mission/DTGsCommunityEventMissionDataExcel.DTGsCommunityEventMissionDataExcel")},
	{EExcelSheetIndexCommunityEvent::CommunityEventMissionTab,	FString("NeverCook/CommunityGirl/Mission/DTGsCommunityEventMissionTabExcel.DTGsCommunityEventMissionTabExcel")},
	{EExcelSheetIndexCommunityEvent::CommunityEventMission,	FString("NeverCook/CommunityGirl/Mission/DTGsCommunityEventMissionExcel.DTGsCommunityEventMissionExcel")},
};

bool ConvertCommunityEventMissionData(const TMap<EExcelSheetIndexCommunityEvent, UGsDataTable*>& excelSheetList, TMap<ECommunityEventMissionDataTableIndex, UGsDataTable*>& dataTableList)
{
	const auto rewardTable = Cast<const UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());

	const UGsDataTable* excelMissionData = excelSheetList[EExcelSheetIndexCommunityEvent::CommunityEventMissionData];
	const UGsDataTable* excelMissionTabData = excelSheetList[EExcelSheetIndexCommunityEvent::CommunityEventMissionTab];
	const UGsDataTable* excelMissionInfoData = excelSheetList[EExcelSheetIndexCommunityEvent::CommunityEventMission];

	TMap<FString, UGsDataTable*> conditions = { TPair<FString, UGsDataTable*>{ "NONE", nullptr } };

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

	TMap<int, FName> missionIdToRow;
	const auto& missaionData = excelMissionData->GetRowMap();
	for (const auto& e : missaionData)
	{
		if (const auto mission = reinterpret_cast<const FGsSchemaCommunityEventMissionDataExcel*>(e.Value))
		{
			FGsSchemaCommunityEventMissionCommon commonData;
			{
				commonData.id = mission->id;
				commonData.eventMissionCount = mission->eventMissionCount;
				commonData.eventMissionText = mission->eventMissionText;

				const FGsSchemaRewardData* rewardData{ nullptr };
				if (rewardTable->FindRowById(mission->rewardId, rewardData))
				{
					commonData.rewardId.DataTable = rewardTable->GetCachedDataTable();
					commonData.rewardId.RowName = rewardData->RowName;
				}

				FGsSchemaCommunityEventMissionCondition conditionInfo;
				{
					conditionInfo.communtityEventMissionType = mission->missionType;

					if (conditions.Find(mission->missionConditionTableName) == nullptr)
					{
						FString tablePath = FString::Format(TEXT("CommunityGirl/Mission/Condition/{0}.{0}"), { mission->missionConditionTableName });

						UGsDataTable* table = nullptr;
						if (FGsDataTableConvertBase::LoadDataTable(DataTablePath / tablePath, table))
						{
							conditions.Add(mission->missionConditionTableName, table);
						}
					}

					if (auto iterTable = conditions.Find(mission->missionConditionTableName))
					{
						if (auto table = (*iterTable))
						{
							conditionInfo.communityEventMissionConditionData.DataTable = table;
							conditionInfo.communityEventMissionConditionData.RowName = mission->missionConditionRowName;
						}
					}
				}

				FString conditionRaw = FindEnumName(TEXT("CommunityEventMissionType"), static_cast<int64>(conditionInfo.communtityEventMissionType)).ToString();
				conditionRaw = conditionRaw + TEXT("_") + mission->missionConditionRowName.ToString();

				FName conditionRowName = FName(*conditionRaw);
				dataTableList[ECommunityEventMissionDataTableIndex::CommunityEventMissionCondition]->AddRow(conditionRowName, conditionInfo);

				commonData.eventMissionConditionId.DataTable = dataTableList[ECommunityEventMissionDataTableIndex::CommunityEventMissionCondition];
				commonData.eventMissionConditionId.RowName = conditionRowName;
			}

			dataTableList[ECommunityEventMissionDataTableIndex::CommunityEventMissionCommon]->AddRow(mission->RowName, commonData);
			missionIdToRow.Add(mission->id, e.Key);
		}
	}

	const auto missionTable = dataTableList[ECommunityEventMissionDataTableIndex::CommunityEventMissionCommon];
	TMap<int, FName> tabIdToRow;
	auto missaionTabData = excelMissionTabData->GetRowMap();
	for (const auto& e : missaionTabData)
	{
		if (const auto tabData = reinterpret_cast<const FGsSchemaCommunityEventMissionTabExcel*>(e.Value))
		{
			FGsSchemaCommunityEventMissionTab tabInfo;
			{
				tabInfo.id = tabData->tabId;
				tabInfo.tabOpenTime = tabData->tabOpenTime;
				tabInfo.tabNameText = tabData->tabNameText;

				for (auto missionId : tabData->eventMissionList)
				{
					if (auto missionNameIter = missionIdToRow.Find(missionId))
					{
						FGsSchemaCommunityEventMissionCommonRow missionRow;
						missionRow.DataTable = missionTable;
						missionRow.RowName = *missionNameIter;
						tabInfo.eventMissionList.Add(missionRow);
					}
				}

				dataTableList[ECommunityEventMissionDataTableIndex::CommunityEventMissionTab]->AddRow(e.Key, tabInfo);
				tabIdToRow.Add(tabData->tabId, e.Key);
			}
		}
	}

	const auto missionTabTable = dataTableList[ECommunityEventMissionDataTableIndex::CommunityEventMissionTab];
	auto missionInfoData = excelMissionInfoData->GetRowMap();

	for (const auto& e : missionInfoData)
	{
		if (const auto excelData = reinterpret_cast<const FGsSchemaCommunityEventMissionExcel*>(e.Value))
		{
			FGsSchemaCommunityEventMission eventmissionInfo;
			eventmissionInfo.id = excelData->id;
			eventmissionInfo.isBonus = excelData->isBonus;
			eventmissionInfo.bonusRewardId.DataTable = excelData->bonusRewardId.DataTable;
			eventmissionInfo.bonusRewardId.RowName = excelData->bonusRewardId.RowName;

			for (auto tabId : excelData->eventMissionTabList)
			{
				if (auto tabNameIter = tabIdToRow.Find(tabId))
				{
					FGsSchemaCommunityEventMissionTabRow missionRow;
					missionRow.DataTable = missionTabTable;
					missionRow.RowName = *tabNameIter;
					eventmissionInfo.eventMissionTablist.Add(missionRow);
				}
			}
			dataTableList[ECommunityEventMissionDataTableIndex::CommunityEventMission]->AddRow(e.Key, eventmissionInfo);
		}
	}

	return true;
}

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterCommunityEventExcelData)
bool FDataTableConverterCommunityEventExcelData::Convert()
{
	TMap<ECommunityEventMissionDataTableIndex, UGsDataTable*> dataTableList;
	for (const auto& e : eventDataTablePath)
	{
		if (false == FGsDataTableConvertBase::LoadDataTable(DataTablePath / e.Value, dataTableList.FindOrAdd(e.Key)))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found DT TableAsset : [%s]"), *(e.Value)));
			DEF_ERR_POPUP_MESSAGE(message);
		}
	}

	TMap<EExcelSheetIndexCommunityEvent, UGsDataTable*> excelSheetList;
	for (const auto& e : excelSheetPath)
	{
		if (false == FGsDataTableConvertBase::LoadDataTable(DataTablePath / e.Value, excelSheetList.FindOrAdd(e.Key)))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found ExcelSheet TableAsset : [%s]"), *(e.Value)));
			DEF_ERR_POPUP_MESSAGE(message);
		}
	}

	for (auto& pair : dataTableList)
	{	
		pair.Value->EmptyTable();
	}

	ConvertCommunityEventMissionData(excelSheetList, dataTableList);

	for (auto& pair : dataTableList)
	{
		pair.Value->SavePackage();
	}

	return true;
}