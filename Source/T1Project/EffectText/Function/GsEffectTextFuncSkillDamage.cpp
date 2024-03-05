// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextFuncSkillDamage.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"


bool FGsEffectTextFuncSkillDamage::IsAdditionalDamage(const FGsSchemaSkillNotifyHitSet* InData)
{
	// 무속성 피해, 속성 피해, 방어무시 피해 값이 있는지 검사
	if (0 < InData->noneDamage ||
		DamageElementalType::NONE != InData->elementalType ||
		0 < InData->ignoreDefenseDamage)
	{
		return true;
	}

	return false;
}

bool FGsEffectTextFuncSkillDamage::SetData(EGsEffectTextParseType InParseType,
	const FGsSchemaSkillNotifySet* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (nullptr == InData ||
		0 == InData->notifyElementList.Num())
	{
		return false;
	}

	for (int i = 0; i < InData->notifyElementList.Num(); ++i)
	{
		// 없으면 안되는 정보이므로 바로 false 리턴
		const FGsSchemaSkillNotifyElement* element = InData->notifyElementList[i].GetRow();
		if (nullptr == element)
		{
			return false;
		}

		//// [B1] executeConditionId가 0인 정보들만 사용함
		//if (0 != element->executeConditionId)
		//{
		//	continue;
		//}

		// 없으면 안되는 정보이므로 바로 false 리턴
		const FGsSchemaSkillNotifyHitSet* hitSet = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
		if (nullptr == hitSet)
		{
			return false;
		}

		// 데미지 없는 타입이면 스킵
		if (hitSet->noDamage)
		{
			continue;
		}

		// true일 경우, 카운트만 증가한 것
		if (CheckIsSameDamageAndAddCount(InParseType, element->executeConditionId, hitSet, OutData))
		{
			continue;
		}

		FGsSchemaEffectTextData newData;
		newData.parseType = InParseType;
		newData.textParams.Emplace(InData->effectText);

		// 0: ExecuteConditionId
		newData.intParams.Emplace(element->executeConditionId);
		// 1: HitCount (처음이므로 1 저장)
		newData.intParams.Emplace(1);

		// 첫 정보이므로 MinMax동일하게 저장
		int32 skillPowerRate = 100 + hitSet->skillPowerRate;
		// 2 : MinRate
		newData.intParams.Emplace(skillPowerRate);
		// 3: MaxRate
		newData.intParams.Emplace(skillPowerRate);
		
		if (IsAdditionalDamage(hitSet))
		{
			// 4: NoneDamage
			newData.intParams.Emplace(hitSet->noneDamage);
			// 5: ElementalType
			newData.intParams.Emplace(static_cast<int32>(hitSet->elementalType));
			// 6: ElementalValue
			newData.intParams.Emplace(hitSet->elementalDamage);
			// 7: IgnoreDefense
			newData.intParams.Emplace(hitSet->ignoreDefenseDamage);
		}

		OutData.Emplace(newData);
	}

	return true;
}

bool FGsEffectTextFuncSkillDamage::CheckIsSameDamageAndAddCount(EGsEffectTextParseType InParseType,
	int32 InExecuteConditionId, const FGsSchemaSkillNotifyHitSet* InHitSet,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	// 추가 데미지 정보가 있는지 검사
	bool bIsAdditionalDamage = IsAdditionalDamage(InHitSet);

	// 기존 리스트에서 정보 비교해서 전부 동일하면 count만 올림. 없으면 새 정보를 추가함
	for (FGsSchemaEffectTextData& effectTextData : OutData)
	{
		// 같은 타입끼리만 검사
		if (InParseType != effectTextData.parseType)
		{
			continue;
		}

		if (InExecuteConditionId != effectTextData.intParams[EGsETSkillDamageParam::ET_ExecuteConditionId])
		{
			continue;
		}

		// 추가 정보가 있을때만 4보다 큰 값이 존재
		bool bIsAlreadyAdditionalDamage = (EGsETSkillDamageParam::ET_AdditionalStart < effectTextData.intParams.Num()) ? true : false;

		// 이전에 수집된 정보와 추가 정보 유무가 서로 다른지 검사 
		if (bIsAdditionalDamage != bIsAlreadyAdditionalDamage)
		{
			continue;
		}

		// 둘다 추가 정보가 있을 경우, 추가 정보가 동일한지 검사
		if (bIsAlreadyAdditionalDamage)
		{
			// 무속성 값이 같은지 확인
			if (effectTextData.intParams[EGsETSkillDamageParam::ET_NoneDamage] != InHitSet->noneDamage)
			{
				continue;
			}

			// 속성과 값이 같은지 확인
			if (effectTextData.intParams[EGsETSkillDamageParam::ET_ElementalType] != static_cast<int32>(InHitSet->elementalType) ||
				effectTextData.intParams[EGsETSkillDamageParam::ET_ElementalValue] != InHitSet->elementalDamage)
			{
				continue;
			}

			// 방어무시 값이 같은지 확인
			if (effectTextData.intParams[EGsETSkillDamageParam::ET_IgnoreDefense] != InHitSet->ignoreDefenseDamage)
			{
				continue;
			}
		}

		// 모두 동일한 상황이면, min~max 표기 위해 비교 및 카운트 증가
		int32 skillPowerRate = 100 + InHitSet->skillPowerRate;

		// min보다 작으면 저장
		if (effectTextData.intParams[EGsETSkillDamageParam::ET_MinRate] > skillPowerRate)
		{
			effectTextData.intParams[EGsETSkillDamageParam::ET_MinRate] = skillPowerRate;
		}

		// max보다 크면 저장
		if (effectTextData.intParams[EGsETSkillDamageParam::ET_MaxRate] < skillPowerRate)
		{
			effectTextData.intParams[EGsETSkillDamageParam::ET_MaxRate] = skillPowerRate;
		}

		// 카운트 증가
		effectTextData.intParams[EGsETSkillDamageParam::ET_HitCount] += 1;

		return true;
	}

	return false;
}

bool FGsEffectTextFuncSkillDamage::IsValidData(const FGsSchemaEffectTextData* InData)
{
	return (EGsETSkillDamageParam::ET_AdditionalStart <= InData->intParams.Num()) ? true : false;
}

void FGsEffectTextFuncSkillDamage::GetString(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	GetStringDamage(InData, OutString);

	if (EGsETSkillDamageParam::ET_AdditionalStart < InData->intParams.Num())
	{
		AddStringNoElemental(InData, OutString);
		AddStringElemental(InData, OutString);
		AddStringIgnoreDefense(InData, OutString);
	}

	AddStringHitCount(InData, OutString);
}

void FGsEffectTextFuncSkillDamage::GetStringDamage(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	// 0인 정보는 찍지 않음
	if (0 >= InData->intParams[EGsETSkillDamageParam::ET_MinRate])
	{
		return;
	}

	// TEXT: 공격력 {0}% 피해
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_skillPowerRate"), textFormat))
	{
		// min max 같을 경우 min만 찍음
		if (InData->intParams[EGsETSkillDamageParam::ET_MinRate] == InData->intParams[EGsETSkillDamageParam::ET_MaxRate])
		{
			FText textDamage = FText::Format(textFormat, InData->intParams[EGsETSkillDamageParam::ET_MinRate]);
			OutString = textDamage.ToString();
		}
		else
		{
			FString strRange;
			strRange.AppendInt(InData->intParams[EGsETSkillDamageParam::ET_MinRate]);
			strRange.Append(TEXT("~"));
			strRange.AppendInt(InData->intParams[EGsETSkillDamageParam::ET_MaxRate]);

			FText textDamage = FText::Format(textFormat, FText::FromString(strRange));
			OutString = textDamage.ToString();
		}
	}
}

void FGsEffectTextFuncSkillDamage::AddStringHitCount(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	// TEXT: {0} 회
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_NumberOfTimes"), textFormat))
	{
		FText textCount = FText::Format(textFormat, InData->intParams[EGsETSkillDamageParam::ET_HitCount]);

		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT(" "));
		}

		OutString.Append(textCount.ToString());
	}
}

void FGsEffectTextFuncSkillDamage::AddStringNoElemental(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	if (0 >= InData->intParams[EGsETSkillDamageParam::ET_NoneDamage])
	{
		return;
	}

	// TEXT: 무속성 피해 {0}
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_elementalDamage_None"), textFormat))
	{
		FText textDamage = FText::Format(textFormat, InData->intParams[EGsETSkillDamageParam::ET_NoneDamage]);

		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT(" + "));
		}
		
		OutString.Append(textDamage.ToString());
	}
}

void FGsEffectTextFuncSkillDamage::AddStringElemental(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	DamageElementalType elementalType = static_cast<DamageElementalType>(InData->intParams[EGsETSkillDamageParam::ET_ElementalType]);

	if (DamageElementalType::NONE == elementalType ||
		DamageElementalType::MAX == elementalType ||
		0 == InData->intParams[EGsETSkillDamageParam::ET_ElementalValue])
	{
		return;
	}

	// TEXT: XX 속성 피해 {0}
	FText textFormat;
	FGsEffectTextFuncText::GetElementalTypeText(elementalType, textFormat);

	if (textFormat.IsEmpty())
	{
		return;
	}

	FText textDamage = FText::Format(textFormat, InData->intParams[EGsETSkillDamageParam::ET_ElementalValue]);

	if (false == OutString.IsEmpty())
	{
		OutString.Append(TEXT(" + "));
	}

	OutString.Append(textDamage.ToString());
}

void FGsEffectTextFuncSkillDamage::AddStringIgnoreDefense(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	if (0 >= InData->intParams[EGsETSkillDamageParam::ET_IgnoreDefense])
	{
		return;
	}

	// TEXT: 방어 무시 피해 {0}
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_ignoreDefenseDamage"), textFormat))
	{
		FText textDamage = FText::Format(textFormat, InData->intParams[EGsETSkillDamageParam::ET_IgnoreDefense]);

		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT(" + "));
		}
		
		OutString.Append(textDamage.ToString());
	}
}
