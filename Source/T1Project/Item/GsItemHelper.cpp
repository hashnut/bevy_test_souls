// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemHelper.h"

#include "DataSchema/Item/GsSchemaItemEquipEnchantBonusTypeData.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectMagicalForge.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemConsumableBox.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsSchemaItemEffectAbnormalityAndReward.h"

#include "Abnormality/AbnormalityEffect/GsSchemaAbnormalityStatSet.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"

#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Algo/Accumulate.h"
#include "Algo/Transform.h"
#include "Algo/ForEach.h"
#include "MathUtil.h"

#include "T1Project.h"


TArray<TPair<int32, int32>> FGsItemHelper::GetEffectIdTimePairList(const FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		GSLOG(Error, TEXT("nullptr == InItem"));
		return TArray<TPair<int32, int32>>();
	}
	int32 detailId = InItem->GetDetailID();

	const FGsSchemaItemConsumeDetail* consumeDetail = UGsTableUtil::FindRowById<UGsTableItemConsumeDetail, FGsSchemaItemConsumeDetail, int32>(detailId);
	if (nullptr == consumeDetail)
	{
		GSLOG(Error, TEXT("nullptr == consumeDetail"));
		return TArray<TPair<int32, int32>>();
	}
	TArray<int32> preEffectIdList = consumeDetail->effectIDList;

	TArray<TPair<int32, int32>> effectIdTimePairList;

	auto effectIdPairParser = [](const auto& InAbnormalitySet, OUT TArray<TPair<int32, int32>>& OutEffectIdTimePairList) mutable {
		TArray<TPair<int32, int32>> tempEffectIdTimePairList;
		Algo::Transform(InAbnormalitySet->effectIdList, tempEffectIdTimePairList, [&InAbnormalitySet](const auto effectId) {
			return MakeTuple(effectId, InAbnormalitySet->durationMin);
			});
		OutEffectIdTimePairList.Append(tempEffectIdTimePairList);
		};

	
	if (consumeDetail->effectType == ItemEffectType::ADD_ABNORMALITY_STAT)
	{
		Algo::ForEach(preEffectIdList, [&effectIdTimePairList, effectIdPairParser](const int32 InPreEffectId) mutable {
			const FGsSchemaAbnormalitySet* abnormalitySet = UGsTableUtil::FindRowById<UGsTableAbnormalitySet, FGsSchemaAbnormalitySet, int32>(InPreEffectId);
			if (nullptr == abnormalitySet)
			{
				GSLOG(Error, TEXT("nullptr == abnormalitySet "));
				return;
			}

			effectIdPairParser(abnormalitySet, effectIdTimePairList);
			});
	}
	// effectType 에 따라 검색하는 테이블이 다르다 (세계수의 잎의 경우, GET_ABNORMALITY_STAT_AND_REWARD 를 사용하기 때문)
	else if (consumeDetail->effectType == ItemEffectType::GET_ABNORMALITY_STAT_AND_REWARD)
	{
		Algo::ForEach(preEffectIdList, [&effectIdTimePairList, effectIdPairParser](const int32 InPreEffectId) mutable {
			const FGsSchemaItemEffectAbnormalityAndReward* abnormalityAndReward = UGsTableUtil::FindRowById<UGsTableItemEffectAbnormalityAndReward, FGsSchemaItemEffectAbnormalityAndReward, int32>(InPreEffectId);
			if (nullptr == abnormalityAndReward)
			{
				GSLOG(Error, TEXT("nullptr == abnormalityAndReward"));
				return;
			}

			const FGsSchemaAbnormalitySet* abnormalitySet = abnormalityAndReward->abnormalityId.GetRow();
			if (nullptr == abnormalitySet)
			{
				GSLOG(Error, TEXT("nullptr == abnormalitySet "));
				return;
			}

			effectIdPairParser(abnormalitySet, effectIdTimePairList);
			});
	}

	return effectIdTimePairList;
}

bool FGsItemHelper::GetItemGradeText(ItemGrade InGrade, OUT FText& OutText)
{
	FString gradeKey;
	switch (InGrade)
	{
	case ItemGrade::NORMAL:		gradeKey = TEXT("Grade_Normal");	break;
	case ItemGrade::MAGIC:		gradeKey = TEXT("Grade_Magic");		break;
	case ItemGrade::RARE:		gradeKey = TEXT("Grade_Rare");		break;
	case ItemGrade::UNIQUE:		gradeKey = TEXT("Grade_Unique");	break;
	case ItemGrade::EPIC:		gradeKey = TEXT("Grade_Epic");		break;
	case ItemGrade::LEGEND:		gradeKey = TEXT("Grade_Legend");	break;
	case ItemGrade::ETERNAL:	gradeKey = TEXT("Grade_Eternal");	break;
	default: break;
	}

	if (!gradeKey.IsEmpty())
	{
		FText::FindText(TEXT("ItemUIText"), gradeKey, OutText);
		return true;
	}		

	return false;
}

bool FGsItemHelper::GetEnchantBonusText(EGsItemEnchantBonusType InType, OUT FText& OutText)
{
	const UGsTableItemEquipEnchantBonusTypeData* table = Cast<const UGsTableItemEquipEnchantBonusTypeData>(
		FGsSchemaItemEquipEnchantBonusTypeData::GetStaticTable());
	if (table)
	{
		const FGsSchemaItemEquipEnchantBonusTypeData* row = nullptr;
		if (table->FindRowById(InType, row))
		{
			OutText = row->nameText;
			return true;
		}
	}

	return false;
}

int32 FGsItemHelper::GetLocalPlayerMaxWeight()
{
	int32 maxWeight = 0;
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>())
		{
			if (const FGsStatInfo* weight = creatureData->GetCreatureStatInfo(StatType::WEIGHT))
			{
				maxWeight = weight->GetMaxStatValue();
			}
		}
	}
	return maxWeight;
}

float FGsItemHelper::GetWeightPercent(int32 InWeight)
{
	float weightPercent = 0.f;
	if (InWeight > 0)
	{
		int32 maxWeight = GetLocalPlayerMaxWeight();
		if (0 < maxWeight)
		{
			weightPercent = ((float)InWeight * 100.0f / (float)maxWeight);
		}
	}
	return weightPercent;
}

bool FGsItemHelper::GetWeightPercentText(float InWeightPercent, OUT FText& OutPercentText, bool InShowPoint /*= true*/)
{
	FText markText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Percent"), markText);

	FString weightPercentStr;
	if (InShowPoint)
	{
		weightPercentStr = FString::Printf(TEXT("%.2f"), FMath::Floor(InWeightPercent * 100) / 100.0f) + markText.ToString();
	}
	else
	{
		weightPercentStr = FString::Printf(TEXT("%d"), (int32)InWeightPercent) + markText.ToString();
	}
	
	OutPercentText = FText::FromString(weightPercentStr);
	return true;
}

bool FGsItemHelper::IsPickOneTypeRewardItem(const FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		return false;
	}

	int32 rewardId = InItem->GetPickOneTypeRewardID();
	if (0 >= rewardId)
	{
		return false;
	}

	return true;
}

FText FGsItemHelper::GetDurationText(const FGsItem* InItem)
{
	TArray<TPair<int32, int32>> effectIdTimePairList = FGsItemHelper::GetEffectIdTimePairList(InItem);
	if (effectIdTimePairList.Num() == 0)
	{
		return FText::GetEmpty();
	}

	int32 duration = effectIdTimePairList[0].Value;
	return FText::FromString(FGsTimeStringHelper::GetMinuteString(duration / 60));
}

FText FGsItemHelper::GetSupplyEffectText(const FGsItem* InItem)
{
	TArray<TPair<int32, int32>> effectIdTimePairList = FGsItemHelper::GetEffectIdTimePairList(InItem);
	if (effectIdTimePairList.Num() == 0)
	{
		return FText::GetEmpty();
	}

	auto abnormalityStatSetToAffectedStatValuePairList = [](const FGsSchemaAbnormalityStatSet* InAbnormalityStatSet) {
		TArray<TPair<FGsStatTypeNameSelector, int32>> outStatValuePairList;
		Algo::Transform(InAbnormalityStatSet->param, outStatValuePairList, [](const auto& elem) {
			return MakeTuple(elem.affectedStat, elem.affectedValue);
			});
		return outStatValuePairList;
	};

	TArray<TPair<FGsStatTypeNameSelector, int32>> affectedStatValuePairList;
	Algo::ForEach(effectIdTimePairList, [=, &affectedStatValuePairList](const auto& statValuePair) mutable {
		const FGsSchemaAbnormalityStatSet* abnormalityStatSet = UGsTableUtil::FindRow<FGsSchemaAbnormalityStatSet>(FName(*(FString::FromInt(statValuePair.Key) + TEXT(".1"))));
		if (nullptr == abnormalityStatSet)
		{
			GSLOG(Error, TEXT("nullptr == abnormalityStatSet"));
			return;
		}

		affectedStatValuePairList.Append(abnormalityStatSetToAffectedStatValuePairList(abnormalityStatSet));
		});

	auto addNewLineToString = [](const FString& A, const FString& B) 
		{
			if (B.IsEmpty())
				return A;
			if (A.IsEmpty())
				return B;
			return A + FString(TEXT("\n")) + B; 
		};
	FString effectStringSum = 
		Algo::TransformAccumulate(affectedStatValuePairList, [](const auto& statValuePair) {
		FString effectString;
		FGsStatHelper::GetDesplayStatNameAndValue(statValuePair.Key, statValuePair.Value, effectString);
		return effectString;
		}, FString(), addNewLineToString);


	// GET_ABNORMALITY_STAT_AND_REWARD 일 경우, RewardId 에 따른 내용물을 RewardData 에서 탐색
	FString rewardDesc;
	if (InItem->IsConsumable())
	{
		int32 detailId = InItem->GetDetailID();

		const FGsSchemaItemConsumeDetail* consumeDetail = UGsTableUtil::FindRowById<UGsTableItemConsumeDetail, FGsSchemaItemConsumeDetail, int32>(detailId);

		if (nullptr == consumeDetail)
		{
			GSLOG(Error, TEXT("nullptr == consumeDetail"));
			return FText::FromString(MoveTemp(effectStringSum));
		}

		if (consumeDetail->effectType == ItemEffectType::GET_ABNORMALITY_STAT_AND_REWARD)
		{
			TArray<int32> effectIdList = consumeDetail->effectIDList;
			rewardDesc = 
				Algo::TransformAccumulate(effectIdList, [](const auto effectId) {
				const FGsSchemaItemEffectAbnormalityAndReward* abnormalityAndReward = UGsTableUtil::FindRowById<UGsTableItemEffectAbnormalityAndReward, FGsSchemaItemEffectAbnormalityAndReward, int32>(effectId);
				if (nullptr == abnormalityAndReward)
				{
					GSLOG(Error, TEXT("nullptr == abnormalityAndReward"));
					return FString();
				}
				const FGsSchemaRewardData* rewardData = abnormalityAndReward->rewardId.GetRow();
				if (nullptr == rewardData)
				{
					GSLOG(Error, TEXT("nullptr == rewardData"));
					return FString();
				}
				return FGsItemConsumableBox::GetRewardDataTextFromId(rewardData->id).ToString();
				}, FString(), addNewLineToString);
			
			if (!rewardDesc.IsEmpty())
				rewardDesc += TEXT("\n\n");

			if (!effectStringSum.IsEmpty())
			{
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_GetAbnormalityAndReward_Abnormality"), findText);
				rewardDesc += findText.ToString();
				rewardDesc += TEXT("\n");
			}
		}
	}

	return FText::FromString(rewardDesc + effectStringSum);
}

void FGsItemHelper::SetPeriodItemCoolTime(const FGsItem* InItem)
{
	FGsCoolTimeManager* coolTimeManager = GCoolTime();
	if (nullptr == coolTimeManager)
		return;

	if (nullptr == InItem)
		return;

	// 기간제가 아닌 아이템
	if (false == InItem->IsPeriodItem())
		return;

	// 이미 만료된 아이템
	if (InItem->IsExpiredTime())
		return;

	// 이미 등록된 아이템
	if (coolTimeManager->IsCoolTime(EGsCoolTime::ItemPeriod, InItem->GetDBID(), 0))
		return;

	FTimespan timeSpan;
	if (false == InItem->GetExpiredRemainTime(timeSpan))
		return;

	coolTimeManager->SetCoolTime(EGsCoolTime::ItemPeriod, InItem->GetDBID(), 0, timeSpan.GetTicks() / ETimespan::TicksPerSecond);
}

const FGsSchemaPassivitySet* FGsItemHelper::GetMagicalForgePassivitySetAndGrade(MagicalForgeData InMagicalForgeData, OUT ItemGrade& OutGrade)
{
	const FGsSchemaItemEffectMagicalForge* itemEffectMagicalForgeData = UGsTableUtil::FindRowById<UGsTableItemEffectMagicalForge, FGsSchemaItemEffectMagicalForge, int32>(InMagicalForgeData.mMagicalForgeId);
	if (nullptr == itemEffectMagicalForgeData)
		return nullptr;

	OutGrade = itemEffectMagicalForgeData->grade;

	for (const FGsSchemaItemMagicalForgeData& magicalForgeLevelData : itemEffectMagicalForgeData->levelGroupList)
	{
		if (magicalForgeLevelData.level == InMagicalForgeData.mLevel)
		{
			return magicalForgeLevelData.passivityId.GetRow();
		}
	}

	return nullptr;
}

bool FGsItemHelper::IsMatchedMagicalForge(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	TWeakPtr<FGsItem> findTargetItem = itemManager->FindItem(InTargetItemDBId, ItemType::EQUIP);
	if (false == findTargetItem.IsValid())
		return false;

	TWeakPtr<FGsItem> findIngredientItem = itemManager->FindItem(InIngredientItemDBId, ItemType::INGREDIENT);
	if (false == findIngredientItem.IsValid())
		return false;

	const FGsItemIngredient* ingredientData = findIngredientItem.Pin()->GetCastItem<FGsItemIngredient>();
	if (nullptr == ingredientData)
		return false;

	return ingredientData->IsMatchedMagicalForgeTargetItem(findTargetItem.Pin()->GetSubCategory());
}

bool FGsItemHelper::CanMagicalForge(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId, bool InIsShowTicker, OUT bool& OutIsSameEffect)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	TWeakPtr<FGsItem> findEqupiItem = itemManager->FindItem(InTargetItemDBId, ItemType::EQUIP);
	TWeakPtr<FGsItem> findIngredientItem = itemManager->FindItem(InIngredientItemDBId, ItemType::INGREDIENT);
	if (false == findEqupiItem.IsValid() || false == findIngredientItem.IsValid())
		return false;

	FGsItemEquipment* equipData = findEqupiItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipData)
		return false;

	FGsItemIngredient* ingredientData = findIngredientItem.Pin()->GetCastItem<FGsItemIngredient>();
	if (nullptr == ingredientData)
		return false;

	if (true == equipData->GetLock() || true == ingredientData->GetLock())
	{
		if (InIsShowTicker)
		{
			// 잠긴 아이템은 강화할수없습니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_Notice_Locked"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		return false;
	}
	if (true == ingredientData->IsExpiredTime() || true == equipData->IsExpiredTime())
	{
		if (InIsShowTicker)
		{
			// 사용할 수 없습니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CannotUse"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		return false;
	}
	// 마법 부여 재료 수량 확인
	if (0 >= ingredientData->GetAmount())
		return false;

	// 마법 부여 재료 확인
	if (ItemEffectType::MAGICAL_FORGE != ingredientData->GetEffectType())
		return false;

	// 유효하지 않은 재료의 EffectId
	if (0 == ingredientData->FindFirstEffectId())
		return false;

	// 신규 마법 부여인가?
	if (0 == equipData->GetMagicalForgeData().mMagicalForgeId)
	{
		OutIsSameEffect = true;
		return true;
	}

	// 동일한 효과에 대한 마법 부여 : 강화
	if (equipData->GetMagicalForgeData().mMagicalForgeId == ingredientData->FindFirstEffectId())
	{
		const FGsSchemaItemEffectMagicalForge* expectMagicalForge = UGsTableUtil::FindRowById<UGsTableItemEffectMagicalForge, FGsSchemaItemEffectMagicalForge, int32>(ingredientData->FindFirstEffectId());
		Level expectLevel = equipData->GetMagicalForgeData().mLevel + 1;

		bool isExistNextLevel = false;
		for (const FGsSchemaItemMagicalForgeData& magicalForgeLevelData : expectMagicalForge->levelGroupList)
		{
			if (magicalForgeLevelData.level == expectLevel)
			{
				isExistNextLevel = true;
				break;
			}
		}

		// 다음 레벨의 효과정보가 없는 것을 더 이상 강화할 수 없는 상태로 판단
		if (false == isExistNextLevel)
		{
			if (InIsShowTicker)
			{
				// 더 이상 마법 부여를 할 수 없습니다.
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_Notice_MagicalGradeMax"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}
			return false;
		}

		OutIsSameEffect = true;
	}
	// 다른 효과에 대한 마법 부여 : 변경
	else
	{
		// 변경

		OutIsSameEffect = false;
	}
	return true;
}
