// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseAbnormalityOvertime.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityOvertimeSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormalityLevel.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsGameObjectUtil.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"


bool FGsEffectTextParseAbnormalityOvertime::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	if (EGsETAbOverTime::ET_EffectType < InData->intParams.Num() &&
		2 == InData->floatParams.Num())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityOvertime::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	FText textName;
	FGsEffectTextFuncText::GetAbnormalityNameText(
		static_cast<AbnormalityEffectType>(InData->intParams[EGsETAbOverTime::ET_EffectType]), textName, InOption);

	FText textValue;
	GetStringInter(InData, textValue);

	FString outStr = FString::Format(TEXT("{0} {1}"), { *textName.ToString(), *textValue.ToString() });
	OutData.Emplace(outStr);

	return true;
}

bool FGsEffectTextParseAbnormalityOvertime::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	TPair<FText, FText> pairEffect;
	FGsEffectTextFuncText::GetAbnormalityNameText(
		static_cast<AbnormalityEffectType>(InData->intParams[EGsETAbOverTime::ET_EffectType]), pairEffect.Key, InOption);

	FText textValue;
	GetStringInter(InData, pairEffect.Value);

	OutData.Emplace(pairEffect);

	return true;
}

bool FGsEffectTextParseAbnormalityOvertime::SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
	OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData)
{
	for (const FGsSchemaAbnormalityIndexLevelInfo& itLevel : InIndexSet->levelList)
	{
		// 1레벨 부터 유효데이터이다
		if (0 == itLevel.level)
		{
			continue;
		}

		// 이미 레벨 정보가 있다면 해당 레벨에 정보 추가
		int32 outDataIndex = -1;
		for (int32 i = 0; i < OutData.Num(); ++i)
		{
			if (itLevel.level == OutData[i].level)
			{
				outDataIndex = i;
				break;
			}
		}

		FGsSchemaEffectTextData data;
		data.parseType = GetParseType();
		data.floatParams.Emplace(InData->durationMin); // 0
		data.floatParams.Emplace(InData->durationMax); // 1

		data.intParams.Emplace(static_cast<int32>(InIndexSet->effectType)); // 0: ET_EffectType

		// 살기일 때만 저장
		if (AbnormalityEffectType::OVERTIME_SENSITIVE == InIndexSet->effectType)
		{
			if (const FGsSchemaAbnormalityOvertimeSet* tableData =
				itLevel.effectRow.GetRowByType<FGsSchemaAbnormalityOvertimeSet>())
			{
				data.intParams.Emplace(static_cast<int32>(tableData->param.targetType)); // 1: ET_SkillEffect
				data.intParams.Emplace(static_cast<int32>(tableData->param.targetStat.GetStatType())); // 2: ET_StatType
				data.intParams.Emplace(tableData->param.value); // 3: ET_StatValue
				data.intParams.Emplace(static_cast<int32>(tableData->param.effectMethodType)); // 4: ET_EffectMethod
				data.intParams.Emplace(static_cast<int32>(tableData->param.damageCategory)); // 5: ET_DamageStat
			}
		}		

		if (0 > outDataIndex)
		{
			FGsSchemaEffectTextAbnormalityLevel dataLevel;
			dataLevel.level = itLevel.level;
			dataLevel.dataList.Emplace(data);
			OutData.Emplace(dataLevel);
		}
		else
		{
			OutData[outDataIndex].dataList.Emplace(data);
		}
	}

	if (0 < OutData.Num())
	{
		return true;
	}

	return false;
}

void FGsEffectTextParseAbnormalityOvertime::GetStringInter(const FGsSchemaEffectTextData* InData, OUT FText& OutText)
{
	FString strTime;
	GetTimeString(InData->floatParams[0], InData->floatParams[1], strTime);

	// 살기가 아닌 경우엔 시간만 표시
	AbnormalityEffectType effectType = static_cast<AbnormalityEffectType>(InData->intParams[EGsETAbOverTime::ET_EffectType]);
	if (AbnormalityEffectType::OVERTIME_SENSITIVE != effectType)
	{
		OutText = FText::FromString(strTime);
		return;
	}

	// 살기의 경우 전체 내용 표시 
	FText textTarget;
	GetTargetText(InData, textTarget);

	FText textEffect;
	GetEffectResultText(InData, textEffect);

	// TEXT: {Time}간 초당 {Target}의 {Effect}
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("Overtime_Total"), textFormat))
	{
		FFormatNamedArguments args;
		args.Add("Time", FText::FromString(strTime));
		args.Add("Target", textTarget);
		args.Add("Effect", textEffect);

		OutText = FText::Format(textFormat, args);
	}
}

void FGsEffectTextParseAbnormalityOvertime::GetTimeString(float InTimeMin, float InTimeMax, OUT FString& OutStr)
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

void FGsEffectTextParseAbnormalityOvertime::GetTargetText(const FGsSchemaEffectTextData* InData, OUT FText& OutText)
{
	int32 targetValue = InData->intParams[EGsETAbOverTime::ET_StatValue];
	AbnormalityEffectAddSubMethodType valueType = 
		static_cast<AbnormalityEffectAddSubMethodType>(InData->intParams[EGsETAbOverTime::ET_EffectMethod]);

	// 값
	FText textValue;
	switch (valueType)
	{
	case AbnormalityEffectAddSubMethodType::ADD_SUB:
	{
		textValue = FText::AsNumber(targetValue);
	}
	break;
	case AbnormalityEffectAddSubMethodType::RESOURCE_RATE:
	{
		// 만분율 퍼센트 표기
		FGsEffectTextFuncText::GetValueRatioText(targetValue, true, textValue);
	}
	break;;
	}

	// 단순 데미지를 찍을 때는 StatType이 NONE으로 오며, 다른 항목은 표시하지 않음
	StatType statType = static_cast<StatType>(InData->intParams[EGsETAbOverTime::ET_StatType]);
	if (StatType::NONE == statType ||
		StatType::SENSITIVE == statType)
	{
		OutText = textValue;
		return;
	}

	// 타겟
	FText textSkillTarget;
	FGsEffectTextFuncText::GetSkillTargetTypeText(
		static_cast<SkillTargetType>(InData->intParams[EGsETAbOverTime::ET_TargetType]), textSkillTarget);

	// 스탯
	FText textStatName;
	FGsEffectTextFuncText::GetStatNameText(statType, textStatName);

	// 번역을 위해 FormatNamed 사용
	// TEXT: {SkillTarget} {StatType} {Value}
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("Overtime_Target"), textFormat))
	{
		FFormatNamedArguments args;
		args.Add("SkillTarget", textSkillTarget);
		args.Add("StatType", textStatName);
		args.Add("Value", textValue);

		OutText = FText::Format(textFormat, args);
	}
}

void FGsEffectTextParseAbnormalityOvertime::GetEffectResultText(const FGsSchemaEffectTextData* InData, OUT FText& OutText)
{
	// 번역을 위해 FormatNamed 사용
	// TEXT: {TargetStat} {Result}
	FText textFormat;
	FText::FindText(TEXT("TooltipText"), TEXT("Overtime_Effect"), textFormat);

	FText textDamageStatType;
	FGsEffectTextFuncText::GetDamageStatTypeText(
		static_cast<DamageStatType>(InData->intParams[EGsETAbOverTime::ET_DamageStat]), textDamageStatType);

	FText textResult;
	FText textFormatLimit;
	AbnormalityEffectType effectType = static_cast<AbnormalityEffectType>(InData->intParams[EGsETAbOverTime::ET_EffectType]);
	switch (effectType)
	{
	//case AbnormalityEffectType::OVERTIME_DAMAGE:
	//{
	//	// TEXT: 피해
	//	FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_result_DAMAGE"), textResult);

	//	// TEXT: (최대 {0}의 피해)
	//	FText::FindText(TEXT("TooltipText"), TEXT("Overtime_Limit"), textFormatLimit);
	//}
	//break;
	//case AbnormalityEffectType::OVERTIME_HOT:
	//{
	//	// TEXT: 회복
	//	FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_result_HEAL"), textResult);

	//	// TEXT: (최대 {0}의 회복)
	//	FText::FindText(TEXT("TooltipText"), TEXT("Overtime_Limit_Heal"), textFormatLimit);
	//}
	//break;
	case AbnormalityEffectType::OVERTIME_SENSITIVE:
	{
		// TEXT: 살기 획득
		FText::FindText(TEXT("TooltipText"), TEXT("Overtime_SensitiveGain"), textResult);
	}
	break;
	}	

	// 번역 때문에 FormatNamed 사용
	FFormatNamedArguments args;
	args.Add("TargetStat", textDamageStatType);
	args.Add("Result", textResult);

	OutText = FText::Format(textFormat, args);
}
