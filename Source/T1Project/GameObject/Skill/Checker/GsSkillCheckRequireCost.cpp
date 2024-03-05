// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCheckRequireCost.h"
#include "T1Project.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Item/GsItemManager.h"
#include "Skill/GsSkill.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "DataSchema/Skill/Level/GsSchemaSkillCostItemInfo.h"
#include "DataSchema/Skill/Level/GsSchemaSkillStatInfo.h"
#include "DataSchema/Skill/Level/GsSchemaSkillCostConsumeInfo.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"

EGsSkillCheckResultType FGsSkillCheckRequireCost::GetErrType() const
{
	return EGsSkillCheckResultType::NotEnoughRequireCost;
}

void FGsSkillCheckRequireCost::SetErrorType(RequireCostErrType ErrType)
{
	_currentErrType = ErrType;
}

bool FGsSkillCheckRequireCost::ProcessCheck(const FGsSkill* NextSkill)
{
	SetErrorType(RequireCostErrType::None);

	if (nullptr == NextSkill)
	{
		return false;
	}

	UGsGameObjectBase* findObject = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == findObject)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* local = findObject->CastGameObject<UGsGameObjectLocalPlayer>();
	
	// 인벤토리 무게가 2번째(90.f) 패널티 타입이라면 경고
	if (WeightPenaltyGrade::SECOND == GItem()->GetWeightPenaltyGradeType())
	{
		SetErrorType(RequireCostErrType::WeightFull);
		return false;
	}
	
	// 우선 주석처리
	/*
	* SkillCategorySet category = NextSkill->GetSkillCateogrySet();
	if (SkillCategorySet::BUFF != category)
	{
		if (true == GItem()->IsInvenWeightFull(90.f))
		{
			SetErrorType(RequireCostErrType::WeightFull);	
			return false;
		}
	}
	*/

	// 데이터에 설정되어있는 전투 소모품 보유 갯수 체크
	const FGsSchemaSkillCostConsumeInfo* consumeInfoData = NextSkill->GetCurrLevelCostConsumeInfoData(NextSkill->GetTotalLevel());
	if (nullptr == consumeInfoData)
		return false;
	
	TArray<FGsSchemaSkillCostItemInfo> costItemList = consumeInfoData->requireItemList;
	for (uint8 i = 0; i < costItemList.Num(); ++i)
	{
		int32 itemID = costItemList[i].itemId;
		ItemAmount amount = costItemList[i].itemAmount;
		if (false == GItem()->IsEnoughItemAmountByTID(itemID, amount))
		{
			_currentErrItemId = itemID;
			SetErrorType(RequireCostErrType::RequireItem);
			// 메세지는 확인 해야된다.
			// GItem()->PlayNotEnoughAquireCostTicker(itemID);
			return false;
		}
	}

	// 데이터에 설정되어있는 stat 설정량 체크
	TArray<FGsSchemaSkillStatInfo> statInfoList = consumeInfoData->statInfoList;
	for (uint8 i = 0; i < statInfoList.Num(); ++i)
	{
		StatType type = statInfoList[i].type;
		int32 value = statInfoList[i].value;

		// 살기 예외처리
		if (type == StatType::SENSITIVE)
		{
			FGsAbnormalityHandlerBase* abnormalitySet = local->GetAbnormalityHandler();
			if (abnormalitySet->IsAbnormalityEffectType(AbnormalityEffectType::STAT_VENT))
			{
				continue;
			}
		}

		const FGsStatInfo* statInfo = local->GetCreatureStatInfoByType(type);
		if (nullptr == statInfo)
		{
#if WITH_EDITOR
			GSLOG(Warning, TEXT("[FGsSkillCheckRequireCost::ProcessCheck] - _local GetStatInfo() is nullptr - StatType : %d"),(uint16)type);
#endif
			continue;
		}

		int32 curValue = statInfo->GetStatValue();
		if (value > curValue)
		{
			FText findText;
			switch (statInfo->GetStatType())
			{
			case StatType::HP:
				SetErrorType(RequireCostErrType::NotEnoughHP);
				break;
			case StatType::MP:
				SetErrorType(RequireCostErrType::NotEnoughMP);
				break;
			}

			return false;
		}
	}
	
	return true;
}

FText FGsSkillCheckRequireCost::GetErrMessage() const
{
	FText findText;
	switch (_currentErrType)
	{
	case RequireCostErrType::WeightFull:
	{
		if (UGsItemManager* itemMgr = GItem())
		{
			if (WeightPenaltyGrade::NONE != itemMgr->GetWeightPenaltyGradeType())
			{
				FTextKey textKey = (itemMgr->GetWeightPenaltyGradeType() == WeightPenaltyGrade::FIRST) ?
					TEXT("ItemUi_Weight_Penalty_1stGrade") : TEXT("ItemUi_Weight_Penalty_2ndGrade");

				int32 value = GSInventory()->GetOwnedItemPenaltyValueByType(itemMgr->GetWeightPenaltyGradeType());
				if (FText::FindText(TEXT("ItemUIText"), textKey, findText))
				{
					findText = FText::Format(findText, value);
				}
			}
		}
	}
		break;
	case RequireCostErrType::NotEnoughHP:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_NotEnoughHP"), findText);
		break;
	case RequireCostErrType::NotEnoughMP:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_NotEnoughMP"), findText);
		break;
	case RequireCostErrType::RequireItem:
	{
		if (const FGsSchemaItemCommon* errItem = FGsItem::GetTableDataByID(_currentErrItemId))
		{
			FText textFormat;
			if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_NotEnoughRequireItem"), textFormat))
			{

				GItem()->FindByTID(_currentErrItemId);
				findText = FText::Format(textFormat, errItem->name);
			}
		}
	}
		break;
// 	default:
// 		FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_NotEnoughRequireCost"), findText);
// 		break;
	}
	
	return findText;
}

