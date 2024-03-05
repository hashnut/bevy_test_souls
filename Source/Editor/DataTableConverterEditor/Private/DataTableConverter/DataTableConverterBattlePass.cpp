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

#include "DataSchema/NeverCook/BattlePass/GsSchemaPassLevelGroupExcel.h"
#include "DataSchema/NeverCook/BattlePass/GsSchemaPassMissionDataExcel.h"
#include "DataSchema/NeverCook/BattlePass/GsSchemaBattlePassDataExcel.h"
#include "DataSchema/NeverCook/BattlePass/GsSchemaPassLevelBonusExcel.h"

#include "DataSchema/Icon/GsSchemaBattlePassLevelIcon.h"

#include "DataSchema/BattlePass/GsSchemaBattlePassLevelInfo.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassLevelBonusInfo.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassLevel.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassCommon.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassDetail.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionTab.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionCommon.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassMissionGroup.h"
#include "DataSchema/BattlePass/GsSchemaBattlePassTabDetail.h"

#include "DataSchema/BattlePass/BattlePassCondition/GsSchemaBattlePassMissionConditionETC.h"
#include "DataSchema/BattlePass/BattlePassCondition/GsSchemaBattlePassMissionConditionMonster.h"
#include "DataSchema/BattlePass/BattlePassCondition/GsSchemaBattlePassMissionConditionItem.h"

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
enum class EDataTableIndex : uint8 {
	BattlePassCommon,
	BattlePassMissionGroup,
	BattlePassMissionTab,
	BattlePassMissionCommon,
	BattlePassCondition,
	BattlePassLevel,
	BattlePassConditionSkill,
	BattlePassConditionQuest,
	BattlePassConditionMonster,
	BattlePassConditionItem,
	BattlePassConditionFairy,
	BattlePassConditionETC,
	BattlePassConditionCurrency,
	BattlePassConditionCostume,

	Max,
};

const static TMap<EDataTableIndex, FString> dataTablePath =
{
	{EDataTableIndex::BattlePassCommon,			FString("BattlePass/DTGsBattlePassCommon.DTGsBattlePassCommon")},
	{EDataTableIndex::BattlePassMissionCommon,	FString("BattlePass/DTGsBattlePassMissionCommon.DTGsBattlePassMissionCommon")},
	{EDataTableIndex::BattlePassMissionGroup,	FString("BattlePass/DTGsBattlePassMissionGroup.DTGsBattlePassMissionGroup")},
	{EDataTableIndex::BattlePassMissionTab,		FString("BattlePass/DTGsBattlePassMissionTab.DTGsBattlePassMissionTab")},
	{EDataTableIndex::BattlePassCondition,		FString("BattlePass/DTGsBattlePassCondition.DTGsBattlePassCondition")},
	{EDataTableIndex::BattlePassLevel,			FString("BattlePass/DTGsBattlePassLevel.DTGsBattlePassLevel")}, 

	//{EDataTableIndex::BattlePassConditionSkill,		FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionSkill.DTGsBattlePassMissionConditionSkill")}
	//{EDataTableIndex::BattlePassConditionQuest,		FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionQuest.DTGsBattlePassMissionConditionQuest")}
	//{EDataTableIndex::BattlePassConditionMonster,	FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionMonster.DTGsBattlePassMissionConditionMonster")}
	//{EDataTableIndex::BattlePassConditionItem,		FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionItem.DTGsBattlePassMissionConditionItem")}
	//{EDataTableIndex::BattlePassConditionFairy,		FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionFairy.DTGsBattlePassMissionConditionFairy")}
	//{EDataTableIndex::BattlePassConditionETC,		FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionETC.DTGsBattlePassMissionConditionETC")}
	//{EDataTableIndex::BattlePassConditionCurrency,	FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionCurrency.DTGsBattlePassMissionConditionCurrency")}
	//{EDataTableIndex::BattlePassConditionCostume,	FString("BattlePass/BattlePassCondition/DTGsBattlePassMissionConditionCostume.DTGsBattlePassMissionConditionCostume")}
};
//==========================================================================================================================================

enum class EExcelSheetIndex
{
	BattlePassData,
	PassLevelGroup,
	PassLevelBonus,
	PassMissionData,

	Max,
};

const static TMap<EExcelSheetIndex, FString> excelSheetPath =
{
	{EExcelSheetIndex::BattlePassData,	FString("NeverCook/BattlePass/DTGsBattlePassDataExcel.DTGsBattlePassDataExcel")},
	{EExcelSheetIndex::PassLevelGroup,	FString("NeverCook/BattlePass/DTGsPassLevelGroupExcel.DTGsPassLevelGroupExcel")},
	{EExcelSheetIndex::PassLevelBonus,	FString("NeverCook/BattlePass/DTGsPassLevelBonusExcel.DTGsPassLevelBonusExcel")},
	{EExcelSheetIndex::PassMissionData,	FString("NeverCook/BattlePass/DTGsPassMissionDataExcel.DTGsPassMissionDataExcel")},
};

auto GetIconPathById = [](const FName& InFindKey, FSoftObjectPath* OutSoftObjectPath) -> bool
{
	(*OutSoftObjectPath) = nullptr;

	const UGsTable* table = FGsSchemaBattlePassLevelIcon::GetStaticTable();
	const FGsSchemaBattlePassLevelIcon* findRow;
	if (table && table->FindRow(InFindKey, findRow))
	{
		(*OutSoftObjectPath) = findRow->iconPath;
	}

	return ((*OutSoftObjectPath).IsNull() == false);
};


bool ConvertPassLevelGroupData(const TMap<EExcelSheetIndex, UGsDataTable*>& excelSheetList, TMap<EDataTableIndex, UGsDataTable*>& dataTableList)
{
	const UGsDataTable* excelLevelData = excelSheetList[EExcelSheetIndex::PassLevelGroup];
	const UGsTablePassLevelBonusExcel* excelLevelBonusTable = Cast<UGsTablePassLevelBonusExcel>(FGsSchemaPassLevelBonusExcel::GetStaticTable());

	const auto itemTable = Cast<const UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	const auto rewardTable = Cast<const UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());

	if (itemTable == nullptr || rewardTable == nullptr) return false;

	TMap<int64, TArray<const FGsSchemaPassLevelGroupExcel*>> templeteList;
	const TMap<FName, uint8*>& rowMap = excelLevelData->GetRowMap();
	for (const auto& e : rowMap)
	{
		if (const auto excelData = reinterpret_cast<const FGsSchemaPassLevelGroupExcel*>(e.Value))
		{
			auto& tempData = templeteList.FindOrAdd(excelData->passLevelGroupId);
			tempData.Add(excelData);
		}
	}

	for (auto& e : templeteList)
	{
		FGsSchemaBattlePassLevel levelData;
		levelData.passLevelGroupId = e.Key;
		levelData.bonus.isPayBonus = false;
		levelData.bonus.exceedPerExp = 0;
		levelData.bonus.maxExp = 0;

		const FGsSchemaPassLevelBonusExcel* bonus{ nullptr };
		if (excelLevelBonusTable->FindRowById(e.Key, bonus))
		{
			levelData.bonus.isPayBonus = bonus->isPayBonus;
			levelData.bonus.exceedPerExp = bonus->exceedPerExp;
			levelData.bonus.maxExp = bonus->maxExp;
			const FGsSchemaRewardData* rewardData{ nullptr };
			if (rewardTable->FindRowById(bonus->bonusRewardId, rewardData))
			{
				levelData.bonus.bonusReward.DataTable = rewardTable->GetCachedDataTable();
				levelData.bonus.bonusReward.RowName = rewardData->RowName;
			}
		}

		for (auto info : e.Value)
		{
			FGsSchemaBattlePassLevelInfo levelInfo;
			levelInfo.exp = info->exp;
			levelInfo.levelPurchaseCurrencyType = info->levelPurchaseCurrencyType;
			levelInfo.levelPurchaseCurrency = info->levelPurchaseCurrency;
			levelInfo.displayLevelFlag = info->displayLevelFlag;

			GetIconPathById(info->icon, &(levelInfo.icon));

			const FGsSchemaItemCommon* itemData{ nullptr };
			if (itemTable->FindRowById(info->levelPurchaseItemId, itemData))
			{
				levelInfo.levelPurchaseItemId.DataTable = itemTable->GetCachedDataTable();
				levelInfo.levelPurchaseItemId.RowName = itemData->RowName;
			}

			levelInfo.levelPurchaseItemCount = info->levelPurchaseItemCount;

			const FGsSchemaRewardData* rewardData{ nullptr };
			if (rewardTable->FindRowById(info->basicRewardId, rewardData))
			{
				levelInfo.levelBasicReward.DataTable = rewardTable->GetCachedDataTable();
				levelInfo.levelBasicReward.RowName = rewardData->RowName;
			}

			if (rewardTable->FindRowById(info->additionalRewardId, rewardData))
			{
				levelInfo.levelAdditionalReward.DataTable = rewardTable->GetCachedDataTable();
				levelInfo.levelAdditionalReward.RowName = rewardData->RowName;
			}

			levelData.level.Emplace(info->level, levelInfo);
		}

		FName name = FName(*FString::FromInt(e.Key));

		dataTableList[EDataTableIndex::BattlePassLevel]->AddRow(name, levelData);
		
	}

	return true;
}

bool ConvertPassMissionGroupData(const TMap<EExcelSheetIndex, UGsDataTable*>& excelSheetList, TMap<EDataTableIndex, UGsDataTable*>& dataTableList)
{
	const UGsDataTable* excelMissionData = excelSheetList[EExcelSheetIndex::PassMissionData];

	static const FString BATTLEPASS_DETAIL_TABLE_NAME(TEXT("BattlePass/DTGsBattlePassDetail.DTGsBattlePassDetail"));

	TMap<FString, UGsDataTable*> conditions = { TPair<FString, UGsDataTable*>{ "NONE", nullptr } };

	const auto detailTable = Cast<const UGsTableBattlePassDetail>(FGsSchemaBattlePassDetail::GetStaticTable());
	UGsDataTable* detailDataTable = nullptr;
	FGsDataTableConvertBase::LoadDataTable(DataTablePath / BATTLEPASS_DETAIL_TABLE_NAME, detailDataTable);

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

	auto SetPassMissionTabInfo = [detailTable](IN const FGsSchemaPassMissionDataExcel* info, OUT FGsSchemaBattlePassMissionTab& tabInfo)
	{	
		tabInfo.tabId = info->passMissionGroupId * 100000 + info->passMissionTabId;
		tabInfo.resetType = info->resetType;

		const auto tabDetailTable = Cast<const UGsTableBattlePassTabDetail>(FGsSchemaBattlePassTabDetail::GetStaticTable());
		const FGsSchemaBattlePassTabDetail* tabDetailData{ nullptr };
		if (tabDetailTable && tabDetailTable->FindRowById(info->tabDetailId, tabDetailData))
		{
			tabInfo.tabDetail.DataTable = tabDetailTable->GetCachedDataTable();
			tabInfo.tabDetail.RowName = tabDetailData->RowName;
		}

		const FGsSchemaBattlePassDetail* detailData{ nullptr };
		if (detailTable && detailTable->FindRowById(info->tabDetailId, detailData))
		{
			tabInfo.detail.DataTable = detailTable->GetCachedDataTable();
			tabInfo.detail.RowName = detailData->RowName;
		}
	};

/*	auto SetPassMissionConditionInfo = [conditions](IN const FGsSchemaPassMissionDataExcel* info, OUT FGsSchemaBattlePassCondition& conditionInfo)
	{
		conditionInfo.passMissionType = info->passMissionType;

		switch (conditionInfo.passMissionType)
		{
			case BattlePassMissionType::CHARACTER_LEVEL:
			case BattlePassMissionType::FRIEND_MAKE:
			case BattlePassMissionType::KILL_PLAYER:
			case BattlePassMissionType::LOGIN_ACCUMULATE:
			case BattlePassMissionType::DAMAGE_ONE_TIME:
			case BattlePassMissionType::MARKET_BUY:
			case BattlePassMissionType::MARKET_SELL:
			case BattlePassMissionType::GUILD_ITEM_COLLECTION_REGISTER:
			case BattlePassMissionType::GUILD_CONTRIBUTE:
			case BattlePassMissionType::DEATH_BY_MONSTER:
			case BattlePassMissionType::DEATH_BY_PLAYER:
			case BattlePassMissionType::SKILL_ENCHANT_TRY:
			case BattlePassMissionType::SKILL_UPGRADE_TRY:
			case BattlePassMissionType::SPACE_CRACK_CLEAR:
			{
				
			}
			break;
			case BattlePassMissionType::KILL_MONSTER:
			case BattlePassMissionType::SPACE_CRACK_KILL_MONSTER:
			{
				if (auto iter = conditions.Find(EDataTableIndex::BattlePassConditionMonster))
				{
					conditionInfo.passMissonConditionData.DataTable = *iter;
					conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
				}

			}
			break;
			case BattlePassMissionType::ITEM_ACCUMULATE_USE:
			case BattlePassMissionType::NPC_SHOP_BUY_ITEM:
			case BattlePassMissionType::ITEM_ENCHANT:
			case BattlePassMissionType::ITEM_REFINE:
			case BattlePassMissionType::ITEM_CRAFT:
			case BattlePassMissionType::ITEM_LOOT:
			{
				if (auto iter = conditions.Find(EDataTableIndex::BattlePassConditionItem))
				{
					conditionInfo.passMissonConditionData.DataTable = *iter;
					conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
				}
			}
			break;
			case BattlePassMissionType::CURRENCY_ACCUMULATE_CONSUME:
			case BattlePassMissionType::CURRENCY_ACCUMULATE_GET:
			{
				if (auto iter = conditions.Find(EDataTableIndex::BattlePassConditionCurrency))
				{
					conditionInfo.passMissonConditionData.DataTable = *iter;
					conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
				}
			}
			break;

			case BattlePassMissionType::COSTUME_GET:
			case BattlePassMissionType::COSTUME_SUMMON:
			case BattlePassMissionType::COSTUME_COMPOSE:
			{
				if (auto iter = conditions.Find(EDataTableIndex::BattlePassConditionCostume))
				{
					conditionInfo.passMissonConditionData.DataTable = *iter;
					conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
				}
			}
			break;
			case BattlePassMissionType::QUEST_COMPLETE:
			{
				if (auto iter = conditions.Find(EDataTableIndex::BattlePassConditionQuest))
				{
					conditionInfo.passMissonConditionData.DataTable = *iter;
					conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
				}
			}
			break;
			case BattlePassMissionType::FAIRY_GET:
			case BattlePassMissionType::FAIRY_SUMMON:
			case BattlePassMissionType::FAIRY_COMPOSE:
			case BattlePassMissionType::FAIRY_GET_LEVEL:
			{
				if (auto iter = conditions.Find(EDataTableIndex::BattlePassConditionFairy))
				{
					conditionInfo.passMissonConditionData.DataTable = *iter;
					conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
				}
			}
			break;
			default:
			{
				if (auto iter = conditions.Find(EDataTableIndex::BattlePassConditionETC))
				{
					conditionInfo.passMissonConditionData.DataTable = *iter;
					conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
				}
			}
		}
	};*/

	TMap<int64, TMap<int64, TArray<const FGsSchemaPassMissionDataExcel*>>> passMissionGroups;
	const TMap<FName, uint8*>& rowMap = excelMissionData->GetRowMap();
	for (const auto& e : rowMap)
	{
		if (const auto excelData = reinterpret_cast<const FGsSchemaPassMissionDataExcel*>(e.Value))
		{
			auto& tabData = passMissionGroups.FindOrAdd(excelData->passMissionGroupId);
			auto& tempData = tabData.FindOrAdd(excelData->passMissionTabId);
			tempData.Add(excelData);
		}
	}

	for (auto& e : passMissionGroups)
	{
		FGsSchemaBattlePassMissionGroup missionGroupData;
		{
			missionGroupData.passMissionGroupId = e.Key;

			for (auto& tab : e.Value) //tabMap
			{
				if (tab.Value.Num() <= 0) continue;

				FGsSchemaBattlePassMissionTab tabInfo;
				{
					SetPassMissionTabInfo(tab.Value[0], tabInfo);

					for (const auto info : tab.Value)
					{	
						FGsSchemaBattlePassMissionCommon commonData;
						{
							commonData.passMissionId = info->passMissionId;
							commonData.passMissionRewardExp = info->passMissionRewardExp;
							commonData.passMissionCount = info->passMissionCount;

							FGsSchemaBattlePassCondition conditionInfo;
							{
								conditionInfo.passMissionType = info->passMissionType;

								if (conditions.Find(info->passConditionTableName) == nullptr)
								{
									FString tablePath = FString::Format(TEXT("BattlePass/BattlePassCondition/{0}.{0}"), {info->passConditionTableName});

									UGsDataTable* table = nullptr;
									if (FGsDataTableConvertBase::LoadDataTable(DataTablePath / tablePath, table))
									{
										conditions.Add(info->passConditionTableName, table);
									}
								}

								if (auto iterTable = conditions.Find(info->passConditionTableName))
								{
									if (auto table = (*iterTable))
									{
										conditionInfo.passMissonConditionData.DataTable = table;
										conditionInfo.passMissonConditionData.RowName = info->passConditionRowName;
									}
								}

								FString conditionRaw = FindEnumName(TEXT("BattlePassMissionType"), static_cast<int64>(conditionInfo.passMissionType)).ToString();
								conditionRaw = conditionRaw + TEXT("_") + info->passConditionRowName.ToString();

								FName conditionRowName = FName(*conditionRaw);
								dataTableList[EDataTableIndex::BattlePassCondition]->AddRow(conditionRowName, conditionInfo);

								commonData.passMissionCondition.DataTable = dataTableList[EDataTableIndex::BattlePassCondition];
								commonData.passMissionCondition.RowName = conditionRowName;

								const FGsSchemaBattlePassDetail* detailData{ nullptr };
								if (detailTable && detailTable->FindRowById(info->detail, detailData))
								{
									commonData.detail.DataTable = detailTable->GetCachedDataTable();
									commonData.detail.RowName = detailData->RowName;
								}
								else if (detailTable && detailDataTable)
								{
									FGsSchemaBattlePassDetail detailNewData;
									detailNewData.RowName = info->RowName;
									detailNewData.id = info->detail;
									detailNewData.name = FText::FromName(info->RowName);
									detailNewData.desc = FText::FromName(info->RowName);
									detailNewData.icon = FSoftObjectPath(TEXT("/Game/UI/Atlas_Source/Item/SPR_ItemConsume_ArcasRedStigma_Sprite.SPR_ItemConsume_ArcasRedStigma_Sprite"));
									detailDataTable->AddRow(detailNewData.RowName, detailNewData);

									commonData.detail.DataTable = detailTable->GetCachedDataTable();
									commonData.detail.RowName = info->RowName;
								}

								const FGsSchemaBattlePassDetail* rewardDetailData{ nullptr };
								if (detailTable && detailTable->FindRowById(info->rewardDetail, rewardDetailData))
								{
									commonData.rewardDetail.DataTable = detailTable->GetCachedDataTable();
									commonData.rewardDetail.RowName = rewardDetailData->RowName;
								}
							}

							dataTableList[EDataTableIndex::BattlePassMissionCommon]->AddRow(info->RowName, commonData);

							FGsSchemaBattlePassMissionCommonRow commonDataRow;
							commonDataRow.DataTable = dataTableList[EDataTableIndex::BattlePassMissionCommon];
							commonDataRow.RowName = info->RowName;

							tabInfo.passMission.Add(commonDataRow);
						}
					}

					FName TabRowName = FName(*FString::FromInt(tabInfo.tabId));
					dataTableList[EDataTableIndex::BattlePassMissionTab]->AddRow(TabRowName, tabInfo);

					FGsSchemaBattlePassMissionTabRow missionTabRow;
					missionTabRow.DataTable = dataTableList[EDataTableIndex::BattlePassMissionTab];
					missionTabRow.RowName = TabRowName;

					missionGroupData.passMissionTab.Add(missionTabRow);
				}
			}

			FName GroupRowName = FName(*FString::FromInt(e.Key));
			dataTableList[EDataTableIndex::BattlePassMissionGroup]->AddRow(GroupRowName, missionGroupData);
		}
	}

	detailDataTable->SavePackage();
	return true;
}

bool ConvertPassCommonData(const TMap<EExcelSheetIndex, UGsDataTable*>& excelSheetList, TMap<EDataTableIndex, UGsDataTable*>& dataTableList)
{
	const UGsDataTable* excePassData = excelSheetList[EExcelSheetIndex::BattlePassData];

	const auto itemTable = Cast<const UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	const auto levelGroupTable = Cast<const UGsTableBattlePassLevel>(FGsSchemaBattlePassLevel::GetStaticTable());
	const auto missionGroupTable = Cast<const UGsTableBattlePassMissionGroup>(FGsSchemaBattlePassMissionGroup::GetStaticTable());
	const auto detailTable = Cast<const UGsTableBattlePassDetail>(FGsSchemaBattlePassDetail::GetStaticTable());
	const auto tabDetailTable = Cast<const UGsTableBattlePassTabDetail>(FGsSchemaBattlePassTabDetail::GetStaticTable());
	
	const TMap<FName, uint8*>& rowMap = excePassData->GetRowMap();
	for (const auto& e : rowMap)
	{
		if (const auto excelData = reinterpret_cast<const FGsSchemaBattlePassDataExcel*>(e.Value))
		{
			FGsSchemaBattlePassCommon passData;
			passData.id = excelData->id;
			passData.passPeriodStart = excelData->passPerriodStart;
			passData.passPeriodEnd = excelData->passPerriodEnd;
			passData.passExposureConditionType = excelData->passExposureConditionType;
			passData.passExposureConditionCountMin = excelData->passExposureConditionCountMin;
			passData.passExposureConditionCountMax = excelData->passExposureConditionCountMax;
			passData.additionalRewardOpenCurrencyType = excelData->additionalRewardOpenCurrencyType;
			passData.additionalRewardOpenCurrency = excelData->additionalRewardOpenCurrency;
			passData.additionalRewardOpenItemAmount = excelData->additionalRewardOpenItemAmount;
			passData.passBanner = excelData->passBanner;

			const FGsSchemaItemCommon* itemData{ nullptr };
			if (excelData->additionalRewardOpenItemId > 0 && 
				itemTable->FindRowById(excelData->additionalRewardOpenItemId, itemData))
			{
				passData.additionalRewardOpenItemId.DataTable = itemTable->GetCachedDataTable();
				passData.additionalRewardOpenItemId.RowName = itemData->RowName;
			}

			const FGsSchemaBattlePassLevel* passLevelGroup{ nullptr };
			if (levelGroupTable && levelGroupTable->FindRowById(excelData->passLevelGroupId, passLevelGroup))
			{
				passData.passLevelGroupId.DataTable = levelGroupTable->GetCachedDataTable();
				passData.passLevelGroupId.RowName = passLevelGroup->RowName;
			}

			const FGsSchemaBattlePassMissionGroup* passMissionGroup{ nullptr };
			if (missionGroupTable && missionGroupTable->FindRowById(excelData->passMissionGroupId, passMissionGroup))
			{
				passData.passMissionGroupId.DataTable = missionGroupTable->GetCachedDataTable();
				passData.passMissionGroupId.RowName = passMissionGroup->RowName;
			}

			const FGsSchemaBattlePassTabDetail* tabDetailData{ nullptr };
			if (tabDetailTable && tabDetailTable->FindRowById(excelData->tabDetailId, tabDetailData))
			{
				passData.tabDetailId.DataTable = tabDetailTable->GetCachedDataTable();
				passData.tabDetailId.RowName = tabDetailData->RowName;
			}

			const FGsSchemaBattlePassDetail* detailData{ nullptr };
			if (detailTable && detailTable->FindRowById(excelData->detail, detailData))
			{
				passData.detail.DataTable = detailTable->GetCachedDataTable();
				passData.detail.RowName = detailData->RowName;
			}

			dataTableList[EDataTableIndex::BattlePassCommon]->AddRow(excelData->RowName, passData);
		}
	}
	return true;
}

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterBattlePassExcelData)
bool FDataTableConverterBattlePassExcelData::Convert()
{
	TMap<EDataTableIndex, UGsDataTable*> dataTableList;
	for (const auto& e : dataTablePath)
	{
		if (false == FGsDataTableConvertBase::LoadDataTable(DataTablePath / e.Value, dataTableList.FindOrAdd(e.Key)))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found DT TableAsset : [%s]"), *(e.Value)));
			DEF_ERR_POPUP_MESSAGE(message);
		}
	}

	TMap<EExcelSheetIndex, UGsDataTable*> excelSheetList;
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

	ConvertPassLevelGroupData(excelSheetList, dataTableList);
	ConvertPassMissionGroupData(excelSheetList, dataTableList);

	for (auto& pair : dataTableList)
	{
		if(pair.Key != EDataTableIndex::BattlePassCommon)
			pair.Value->SavePackage();
	}

	ConvertPassCommonData(excelSheetList, dataTableList);
	dataTableList[EDataTableIndex::BattlePassCommon]->SavePackage();
	return true;
}