// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseAbnormalityDefault.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalityEffectTextSet.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormalityLevel.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTableUtil.h"


bool FGsEffectTextParseAbnormalityDefault::IsValidData(const FGsSchemaEffectTextData* InData) const
{
#ifdef USE_CUSTOM_TEXT
	return (InData->textParams.IsValidIndex(0)) ? true : false;
#else
	return false;
#endif
}

bool FGsEffectTextParseAbnormalityDefault::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		OutData.Emplace(InData->textParams[0].ToString());
		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityDefault::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (IsValidData(InData))
	{
		TPair<FText, FText> pairText;
		pairText.Key = InData->textParams[0];
		pairText.Value = FText::GetEmpty();

		OutData.Emplace(pairText);
		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityDefault::SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
	OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData)
{
	// effectText 있을 경우
	const FGsSchemaAbnormalityEffectTextSet* effectTextSet =
		UGsTableUtil::FindRowById<UGsTableAbnormalityEffectTextSet, FGsSchemaAbnormalityEffectTextSet>(InData->id);
	if (effectTextSet)
	{
		const int32 INVALID_OUT_DATA_INDEX = -1;

		for (const TPair<int32, FText>& itPair : effectTextSet->effectText)
		{
			// 이미 레벨 정보가 있다면 해당 레벨에 정보 추가
			int32 outDataIndex = INVALID_OUT_DATA_INDEX;
			for (int32 i = 0; i < OutData.Num(); ++i)
			{
				if (itPair.Key == OutData[i].level)
				{
					outDataIndex = i;
					break;
				}
			}

			FGsSchemaEffectTextData data;
			data.parseType = GetParseType();
			data.textParams.Emplace(itPair.Value);

			// 지속시간 같은걸 따로 찍는 경우가 있어서 저장함. InIndexSet이 null인 경우가 있는듯
			//data.intParams.Emplace(static_cast<int32>(InIndexSet->effectType));
			data.floatParams.Emplace(InData->durationMin);
			data.floatParams.Emplace(InData->durationMax);

			if (INVALID_OUT_DATA_INDEX == outDataIndex)
			{	
				FGsSchemaEffectTextAbnormalityLevel dataLevel;
				dataLevel.level = itPair.Key;
				dataLevel.dataList.Emplace(data);

				OutData.Emplace(dataLevel);
			}
			else
			{
				OutData[outDataIndex].dataList.Emplace(data);
			}
		}

		return true;
	}

	// 레벨별 정보 수집할 필요가 없음. 구분을 위해 -1값. 기획팀에서는 기본 1레벨 데이터를 입력한다.
	FGsSchemaEffectTextAbnormalityLevel dataLevel;
	dataLevel.level = NOT_USE_EFFECT_TEXT_LEVEL;

	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();
	data.textParams.Emplace(InData->ingameName);

	// 지속시간 같은걸 따로 찍는 경우가 있어서 저장함
	//data.intParams.Emplace(static_cast<int32>(InIndexSet->effectType));
	data.floatParams.Emplace(InData->durationMin);
	data.floatParams.Emplace(InData->durationMax);

	dataLevel.dataList.Emplace(data);
	OutData.Emplace(dataLevel);

	return true;
}

bool FGsEffectTextParseAbnormalityDefault::GetStringAbnormalityTime(const FGsSchemaEffectTextData* InData, int32 InOption,
	OUT FString& OutData)
{
	if (2 == InData->floatParams.Num())
	{
		if (0 < InData->floatParams[0])
		{
			GetTimeString(InData->floatParams[0], InData->floatParams[1], OutData);
			return true;
		}
	}

	return false;
}

void FGsEffectTextParseAbnormalityDefault::GetTimeString(float InTimeMin, float InTimeMax, OUT FString& OutStr)
{
	if (InTimeMin == InTimeMax)
	{
		OutStr.Append(FGsTimeStringHelper::GetSecondString(InTimeMin));
	}
	else
	{
		OutStr.Append(FGsTimeStringHelper::GetSecondString(InTimeMin));
		OutStr.Append(TEXT("~"));
		OutStr.Append(FGsTimeStringHelper::GetSecondString(InTimeMax));
	}
}
