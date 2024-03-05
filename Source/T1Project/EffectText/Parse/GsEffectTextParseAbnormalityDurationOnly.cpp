// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseAbnormalityDurationOnly.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormalityLevel.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "EffectText/GsEffectTextParser.h"
#include "EffectText/Function/GsEffectTextFuncText.h"


bool FGsEffectTextParseAbnormalityDurationOnly::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	if (InData->intParams.IsValidIndex(0) &&
		2 == InData->floatParams.Num())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityDurationOnly::GetString(const FGsSchemaEffectTextData* InData, int InLevel, 
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		AbnormalityEffectType type = static_cast<AbnormalityEffectType>(InData->intParams[0]);
		float durationMin = InData->floatParams[0];
		float durationMax = InData->floatParams[1];
	
		FText textName;
		FGsEffectTextFuncText::GetAbnormalityNameText(type, textName, InOption);

		FString strEffect = textName.ToString();
		strEffect.Append(TEXT(" "));
		
		if (durationMin == durationMax)
		{
			strEffect.Append(FGsTimeStringHelper::GetSecondString(durationMin));
		}
		else
		{
			strEffect.Append(FGsTimeStringHelper::GetSecondString(durationMin));
			strEffect.Append(TEXT("~"));
			strEffect.Append(FGsTimeStringHelper::GetSecondString(durationMax));
		}

		OutData.Emplace(strEffect);

		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityDurationOnly::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (IsValidData(InData))
	{
		AbnormalityEffectType type = static_cast<AbnormalityEffectType>(InData->intParams[0]);
		float durationMin = InData->floatParams[0];
		float durationMax = InData->floatParams[1];

		FString strValue;
		if (durationMin == durationMax)
		{
			strValue = FGsTimeStringHelper::GetSecondString(durationMin);
		}
		else
		{
			strValue = FGsTimeStringHelper::GetSecondString(durationMin);
			strValue.Append(TEXT("~"));
			strValue.Append(FGsTimeStringHelper::GetSecondString(durationMax));
		}

		FText textName;
		FGsEffectTextFuncText::GetAbnormalityNameText(type, textName, InOption);

		TPair<FText, FText> pairEffect;
		pairEffect.Key = textName;
		pairEffect.Value = FText::FromString(strValue);

		OutData.Emplace(pairEffect);

		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityDurationOnly::SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
	OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData)
{
	// 지속 시간만 찍기 때문에 레벨별 정보를 수집할 필요가 없음. 
	// 구분을 위해 - 1값.기획팀에서는 기본 1레벨 데이터를 입력한다.	
	FGsSchemaEffectTextAbnormalityLevel dataLevel;
	dataLevel.level = NOT_USE_EFFECT_TEXT_LEVEL;

	// 주의: 이름을 저장하려했는데, FText::FindText 한 텍스트는 데이터가 잘 넣어져도 저장이 안됨을 확인
	// 테이블에 이미 입력된 값을 복사하는 것은 잘 된다... 뭐가문젠지 ㅠ
	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();
	data.intParams.Emplace(static_cast<int32>(InIndexSet->effectType));
	data.floatParams.Emplace(InData->durationMin);
	data.floatParams.Emplace(InData->durationMax);

	dataLevel.dataList.Emplace(data);
	OutData.Emplace(dataLevel);

	return true;
}
