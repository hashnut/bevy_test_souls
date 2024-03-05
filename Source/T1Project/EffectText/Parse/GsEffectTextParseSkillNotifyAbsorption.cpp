// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillNotifyAbsorption.h"
#include "DataSchema/Skill/GsSchemaSkillNotifyElement.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"


bool FGsEffectTextParseSkillNotifyAbsorption::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (2 == InData->intParams.Num()) ? true : false;
}

bool FGsEffectTextParseSkillNotifyAbsorption::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->textParams[0].IsEmpty())
	{
		OutData.Emplace(InData->textParams[0].ToString());
		return true;
	}
#endif

	if (IsValidData(InData))
	{
		FString strInfo;
		if (GetStringAbsorption(InData->intParams[0], InData->intParams[1], strInfo))
		{
			OutData.Emplace(strInfo);
			return true;
		}
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyAbsorption::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->textParams[0].IsEmpty())
	{
		TPair<FText, FText> pairText;
		pairText.Key = InData->textParams[0];

		OutData.Emplace(pairText);
		return true;
	}
#endif

	if (IsValidData(InData))
	{	
		FString strInfo;
		if (GetStringAbsorption(InData->intParams[0], InData->intParams[1], strInfo))
		{
			TPair<FText, FText> pairText;
			pairText.Key = FText::FromString(strInfo);
			pairText.Value = FText::GetEmpty();
			OutData.Emplace(pairText);
			
			return true;
		}
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyAbsorption::SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (nullptr == InData)
	{
		return false;
	}

	// 없으면 안되는 정보이므로 바로 false 리턴
	const FGsSchemaSkillNotifyHitSet* hitSet = InData->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
	if (nullptr == hitSet)
	{
		return false;
	}

	// 둘다 없으면 리턴
	if (0 == hitSet->hpAbsorptionRate && 0 == hitSet->hpAbsorption)
	{
		return false;
	}

	// hitSet->hpAbsorptionRate는 백분율 값임에 유의
	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();
	data.textParams.Emplace(InData->effectText);
	data.intParams.Emplace(hitSet->hpAbsorptionRate);
	data.intParams.Emplace(hitSet->hpAbsorption);

	OutData.Emplace(data);
	
	return true;
}

bool FGsEffectTextParseSkillNotifyAbsorption::GetStringAbsorption(int32 InRate, int32 InValue, OUT FString& OutString)
{
	if (0 != InRate)
	{
		// TEXT: 피해량의 {0}% HP로 흡수
		FText textFormat;
		if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_hpAbsorptionRate"), textFormat))
		{
			FText textOut = FText::Format(textFormat, InRate);
			OutString = textOut.ToString();
		}
	}

	if (0 != InValue)
	{
		// TEXT: HP {0} 흡수
		FText textFormat;
		if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_hpAbsorption"), textFormat))
		{
			FText textOut = FText::Format(textFormat, InValue);
			if (OutString.IsEmpty())
			{
				OutString = textOut.ToString();
			}
			else
			{
				OutString.Append(TEXT(", "));
				OutString.Append(textOut.ToString());
			}
		}
	}

	if (OutString.IsEmpty())
	{
		return false;
	}

	return true;
}
