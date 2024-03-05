#include "GsOptionFunc.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsAIManager.h"

#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "AI/Data/GsAIDefine.h"

#include "Option/GsOptionValueDefine.h"
#include "Option/GsServerOption.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Option/GsSchemaOptionItemGainData.h"
#include "Option/GsOptionData.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

// 아이템 자동 획득 설정
// ui 순서: 끄기, 매직이상, 레어이상, 모두획득
// 서버 순서: 모두, 매직, 레어, off
// 옵션->서버
AutoDropItemGrade FGsOptionFunc::ConvertAutoItemGainOptionEnumToServerEnum(int In_optionEnum)
{
	AutoDropItemGrade resVal = AutoDropItemGrade::MAX;
	switch (In_optionEnum)
	{
	case 0:		
		resVal = AutoDropItemGrade::OFF;
		break;
	case 1:
		resVal = AutoDropItemGrade::MAGIC;
		break;
	case 2:
		resVal = AutoDropItemGrade::RARE;
		break;
	case 3:
		resVal = AutoDropItemGrade::ALL;		
		break;
	}
	return resVal;
}

// 서버->옵션
int FGsOptionFunc::ConvertAutoItemGainServerEnumToOptionEnum(AutoDropItemGrade In_serverEnum)
{
	int resVal = 0;

	switch (In_serverEnum)
	{
	case AutoDropItemGrade::OFF:
		resVal = 0;
		break;
	case AutoDropItemGrade::MAGIC:
		resVal = 1;
		break;
	case AutoDropItemGrade::RARE:
		resVal = 2;
		break;
	case AutoDropItemGrade::ALL:
		resVal = 3;
		break;
	}
	return resVal;
}

// 무게 옵션값에 현재 괜찮은가
bool FGsOptionFunc::IsPossibleLootByWeightOption(
	EGsOptionValueAutoItemLootWeightLimit In_weightOption, int32 In_lootWeight)
{
	// 꺼져있으면 무조건 ok
	if (In_weightOption == EGsOptionValueAutoItemLootWeightLimit::OFF)
	{
		return true;
	}

	int32 maxWeight = FGsItemHelper::GetLocalPlayerMaxWeight();
	int32 limitWeight = 0;
	switch (In_weightOption)
	{
	case EGsOptionValueAutoItemLootWeightLimit::WEIGHT_90_PER_UNDER:
		limitWeight = maxWeight * 90 / 100;
		break;
	case EGsOptionValueAutoItemLootWeightLimit::WEIGHT_70_PER_UNDER:
		limitWeight = maxWeight * 70 / 100;
		break;
	case EGsOptionValueAutoItemLootWeightLimit::WEIGHT_50_PER_UNDER:
		limitWeight = maxWeight * 50 / 100;
		break;
	}
	
	int32 addedItemWeight = GItem()->GetOwnedItemWeight() + In_lootWeight;

	// 체크 값보다 현재값이 크거나 같으면 false 아니면 true
	return (limitWeight <= addedItemWeight) ? false : true;
}

bool FGsOptionFunc::IsPossibleLootByWeightOption(int32 In_lootWeight)
{
	EGsOptionValueAutoItemLootWeightLimit weightLimitOption = EGsOptionValueAutoItemLootWeightLimit::OFF;
	if (FGsServerOption* serverOption = GOption()->GetServerOption())
	{
		weightLimitOption = serverOption->GetAutoDropItemWeight();
	}

	return IsPossibleLootByWeightOption(weightLimitOption, In_lootWeight);
}

bool FGsOptionFunc::IsPossibleLootByGradeOption(ItemType In_itemType, ItemGrade In_itemGrade)
{
	// not equip : pass
	if (In_itemType != ItemType::EQUIP)
	{
		return true;
	}

	EGsAIAutoLootDropItemGradeType autoLootOption =
		GSAI()->GetContentsConditionDefault()->GetAutoLootDropItemGrade();

	// all loot: pass
	if (autoLootOption == EGsAIAutoLootDropItemGradeType::All)
	{
		return true;
	}
	// not loot: not pass
	if (autoLootOption == EGsAIAutoLootDropItemGradeType::NotLoot)
	{
		return false;
	}

	int intItemGrade = (int)In_itemGrade;
	int compareVal = (autoLootOption == EGsAIAutoLootDropItemGradeType::OverMagic) ?
		(int)ItemGrade::MAGIC : (int)ItemGrade::RARE;

	// not enough grade
	if (compareVal > intItemGrade)
	{
		return false;
	}

	return true;
}


bool FGsOptionFunc::IsPossibleNoneEquipItemLootByGradeOption(ItemId InItemId)
{
	const FGsSchemaItemCommon* InItemData = GItem()->GetItemTableDataByTID(InItemId);	
	return IsPossibleNoneEquipItemLootByGradeOption(InItemData);
}


bool FGsOptionFunc::IsPossibleNoneEquipItemLootByGradeOption(const FGsSchemaItemCommon* InItemData)
{
	if (nullptr == InItemData || ItemType::EQUIP == InItemData->type)
	{
		return false;
	}

	return IsPossibleNoneEquipItemLootByGradeOption(InItemData->categorySub, InItemData->grade);
}


bool FGsOptionFunc::IsPossibleNoneEquipItemLootByGradeOption(ItemCategorySub InItemCategorySub, ItemGrade InItemGrade)
{
	bool IsPossible = false;

	do
	{
		const FGsServerOption* serverOption = GOption()->GetServerOption();
		if (nullptr == serverOption)
		{
			break;
		}

		const TMap<FName, uint8*>* rowDataMap = nullptr;
		const UGsTableOptionItemGainData* itemGainTable = Cast<UGsTableOptionItemGainData>(FGsSchemaOptionItemGainData::GetStaticTable());
		if (nullptr == itemGainTable || false == itemGainTable->GetRowMap(rowDataMap) || 0 >= rowDataMap->Num())
		{
			// 2023/5/3 PKT - Table이 존재 하지 않으면 무조건 루팅
			IsPossible = true;
			break;
		}

		const OptionSelectTypeInfo optionInfo = serverOption->GetItempickupOption();
		auto IsPossibleChecked = [=](EGsOptionItem InOptionItem, ItemCategorySub InItemCategorySub, ItemGrade InItemGrade) -> bool
		{
			bool isOptionFlag = false;

			switch (InOptionItem)
			{
			case EGsOptionItem::OptionItemGainNoneEqupConsume_Normal:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainCommonConsume;
				break;
			case EGsOptionItem::OptionItemGainNoneEqupConsume_Special:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainSpecialConsume;
				break;
			case EGsOptionItem::OptionItemGainScrollConsume_Normal:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainCommonScroll;
				break;
			case EGsOptionItem::OptionItemGainScrollConsume_Special:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainSpecialScroll;
				break;
			case EGsOptionItem::OptionItemGainScrollConsume_Enchant:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainEnchantScroll;
				break;
			case EGsOptionItem::OptionItemGainOther_Normal:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainOtherNormal;
				break;
			case EGsOptionItem::OptionItemGainOther_Special:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainOtherSpecial;
				break;
			case EGsOptionItem::OptionItemGainOther_Etc:
				isOptionFlag = serverOption->GetItempickupOption().mItemGainOtherEtc;
				break;
			}

			if (false == isOptionFlag)
			{
				return false;
			}

			bool IsPossible = false;
			for (const auto& rowData : (*rowDataMap))
			{
				const FGsSchemaOptionItemGainData* itemGainData = reinterpret_cast<const FGsSchemaOptionItemGainData*>(rowData.Value);
				if (InOptionItem == itemGainData->gainType)
				{
					for (const auto& itemOptionInfoData : itemGainData->itemCategoryInfoList)
					{
						if (InItemCategorySub == itemOptionInfoData.itemCategorySub)
						{
							if (itemOptionInfoData.minGrade <= InItemGrade && itemOptionInfoData.maxGrade >= InItemGrade)
							{
								IsPossible = true;
							}
							break;
						}
					}
				}

				if (IsPossible)
				{
					break;
				}
			};

			return IsPossible;
		};
			

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainNoneEqupConsume_Normal, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainNoneEqupConsume_Special, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainScrollConsume_Normal, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainScrollConsume_Special, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainScrollConsume_Enchant, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainOther_Normal, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainOther_Special, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		if (true == IsPossibleChecked(EGsOptionItem::OptionItemGainOther_Etc, InItemCategorySub, InItemGrade))
		{
			IsPossible = true;
			break;
		}

		IsPossible = false;
			
	} while (0);

	return IsPossible;
}