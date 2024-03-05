// Fill out your copyright notice in the Description page of Project Settings.

#include "GsEffectTextParseSkillNotifyHeal.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHeal.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"


bool FGsEffectTextParseSkillNotifyHeal::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	if (EGsETSkillHealParam::ET_MaxParam == InData->intParams.Num())
	{
		if (0 != InData->intParams[EGsETSkillHealParam::ET_DamageStatType])
		{
			return true;
		}
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyHeal::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		FString strEffect;
		GetStringInter(InData, strEffect);

		OutData.Emplace(strEffect);

		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyHeal::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{	
	if (IsValidData(InData))
	{
		FString strEffect;
		GetStringInter(InData, strEffect);

		TPair<FText, FText> pairEffect;
		pairEffect.Key = FText::FromString(strEffect);
		pairEffect.Value = FText::GetEmpty();
		
		OutData.Emplace(pairEffect);
		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyHeal::SetData(const FGsSchemaSkillNotifySet* InData, 
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (0 == InData->notifyElementList.Num())
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
		const FGsSchemaSkillNotifyHeal* healInfo = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyHeal>();
		if (nullptr == healInfo)
		{
			return false;
		}
		
		FGsSchemaEffectTextData data;
		data.parseType = GetParseType();
		data.textParams.Emplace(InData->effectText);

		data.intParams.Emplace(static_cast<int32>(healInfo->healStatType)); // 0: ET_HealStatType
		data.intParams.Emplace(healInfo->healFixedStatValue); // 1: ET_FixedValue
		data.intParams.Emplace(healInfo->healRatioStatValue); // 2: ET_RatioValue		
		data.intParams.Emplace(static_cast<int32>(healInfo->healRatioValueType)); // 3: ET_RatioValueType
		data.intParams.Emplace(static_cast<int32>(healInfo->target)); // 4: ET_TargetType

		OutData.Emplace(data);
	}

	return true;
}

void FGsEffectTextParseSkillNotifyHeal::GetStringInter(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	DamageStatType damageStatType = static_cast<DamageStatType>(InData->intParams[EGsETSkillHealParam::ET_DamageStatType]);

	// 고정 값 일경우 
	int32 fixedValue = InData->intParams[EGsETSkillHealParam::ET_FixedValue];
	if (0 < fixedValue)
	{
		GetStatValueString(damageStatType, fixedValue, false, OutString);
	}

	HealRatioValueType healValueType = static_cast<HealRatioValueType>(InData->intParams[EGsETSkillHealParam::ET_RatioValueType]);
	if (HealRatioValueType::NONE == healValueType)
	{
		return;
	}

	// 비율 표시일 경우
	FString strRatioValue;
	int32 ratioValue = InData->intParams[EGsETSkillHealParam::ET_RatioValue];
	GetStatValueString(damageStatType, ratioValue, true, strRatioValue);

	FText textSkillTarget;
	FGsEffectTextFuncText::GetSkillTargetTypeText(
		static_cast<SkillTargetType>(InData->intParams[EGsETSkillHealParam::ET_TargetType]), textSkillTarget);

	FText textHealValueType;
	FGsEffectTextFuncText::GetHealRatioValueTypeText(healValueType, textHealValueType);

	// 번역을 위해 FormatNamed 사용
	// TEXT: {Target} {HealValueType} {Effect}
	FText textOutRatio;
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("Heal_Effect2"), textFormat))
	{
		FFormatNamedArguments args;
		args.Add("Target", textSkillTarget);
		args.Add("HealValueType", textHealValueType);
		args.Add("Effect", FText::FromString(strRatioValue));

		textOutRatio = FText::Format(textFormat, args);
	}	

	// 고정값, 비율값 두개 다 쓸 경우엔 줄바꿈해서 표시
	if (false == OutString.IsEmpty())
	{
		OutString.Append(TEXT("\n"));
	}
	
	OutString.Append(textOutRatio.ToString());
}

void FGsEffectTextParseSkillNotifyHeal::GetStatValueString(DamageStatType InType, int32 InValue, bool bIsRatio, OUT FString& OutString)
{
	FText textStatType;
	FGsEffectTextFuncText::GetDamageStatTypeText(InType, textStatType);

	FText textUpDown;
	if (DamageStatType::SENSITIVE == InType)
	{
		if (0 < InValue)
		{
			// TEXT: 획득
			FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_Heal_Effect_Sensitive_Positive"), textUpDown);
		}
		else
		{
			// TEXT: 소모
			FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_Heal_Effect_Sensitive_Negative"), textUpDown);
		}
	}
	else
	{
		if (0 < InValue)
		{
			// TEXT: 회복
			FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_Heal_Effect_Positive"), textUpDown);
		}
		else
		{
			// TEXT: 감소
			FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_Heal_Effect_Negative"), textUpDown);
		}
	}

	// 번역을 위해 FormatNamed 사용
	// TEXT: {StatType} {Value} {Effect}
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("Heal_Effect1"), textFormat))
	{
		FFormatNamedArguments args;
		args.Add("StatType", textStatType);

		if (bIsRatio)
		{
			FText textValue;
			FGsEffectTextFuncText::GetValueRatioText(InValue, true, textValue);
			args.Add("Value", textValue);
		}
		else
		{
			args.Add("Value", InValue);
		}

		args.Add("Effect", textUpDown);

		FText textOut = FText::Format(textFormat, args);
		OutString = textOut.ToString();
	}
}
