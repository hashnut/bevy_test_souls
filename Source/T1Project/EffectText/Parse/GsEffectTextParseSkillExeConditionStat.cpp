// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillExeConditionStat.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/Skill/GsSchemaSkillExecuteConditionSet.h"
#include "DataSchema/Skill/Param/GsSchemaSkillExecuteConditionMethodStat.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "GameObject/Stat/GsStatHelper.h"


bool FGsEffectTextParseSkillExeConditionStat::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (EGsETSkillConditionStatIndex::ET_MAX_INDEX == InData->intParams.Num()) ? true : false;;
}

bool FGsEffectTextParseSkillExeConditionStat::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	FText outText;
	GetTextInter(InData, outText);

	OutData.Emplace(outText.ToString());

	return true;
}

bool FGsEffectTextParseSkillExeConditionStat::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	TPair<FText, FText> outPair;
	GetTextInter(InData, outPair.Key);
	outPair.Value = FText::GetEmpty();

	OutData.Emplace(outPair);

	return true;
}

bool FGsEffectTextParseSkillExeConditionStat::SetSkillConditionData(const FGsSchemaSkillExecuteConditionSet* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	const FGsSchemaSkillExecuteConditionMethodStat* param =
		InData->conditionMethodParam.GetRowByType<FGsSchemaSkillExecuteConditionMethodStat>();
	if (nullptr == param)
	{
		return false;
	}

	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();

	data.intParams.Emplace(static_cast<int32>(param->target)); // ET_TARGET
	data.intParams.Emplace(static_cast<int32>(param->statType.GetStatType())); // ET_STAT_TYPE	
	data.intParams.Emplace(static_cast<int32>(param->type)); // ET_VALUE_TYPE
	data.intParams.Emplace(param->param); // ET_VALUE
	data.intParams.Emplace(static_cast<int32>(param->statCondition)); // ET_CONDITION

	OutData.Emplace(data);

	return true;
}

void FGsEffectTextParseSkillExeConditionStat::GetTextInter(const FGsSchemaEffectTextData* InData, OUT FText& OutText)
{
	// 대상
	FText textTarget;
	SkillTargetType targetType = static_cast<SkillTargetType>(
		InData->intParams[EGsETSkillConditionStatIndex::ET_TARGET]);

	FGsEffectTextFuncText::GetSkillTargetTypeText(targetType, textTarget);

	// 스탯이름
	FText textStatName;
	StatType statType = static_cast<StatType>(InData->intParams[EGsETSkillConditionStatIndex::ET_STAT_TYPE]);
	FGsStatHelper::GetDisPlayStatName(statType, textStatName);

	// 값
	FText textStatValue;
	GetTextStatValue(InData->intParams[EGsETSkillConditionStatIndex::ET_VALUE_TYPE],
		InData->intParams[EGsETSkillConditionStatIndex::ET_VALUE],
		textStatValue);

	// 이상 이하
	FText textCondition;
	GetTextCondition(InData->intParams[EGsETSkillConditionStatIndex::ET_CONDITION], textCondition);

	// 글자 조합
	// TEXT: {0}의 {1} {2} {3} 일 때, 추가 효과:
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("ExecuteCondition_STAT_tooltip"), textFormat))
	{
		OutText = FText::Format(textFormat, textTarget, textStatName, textStatValue, textCondition);
	}
}

void FGsEffectTextParseSkillExeConditionStat::GetTextStatValue(int32 InValueType, int32 InValue, OUT FText& OutText)
{
	SkillExecuteConditionStatConditionType valueType = static_cast<SkillExecuteConditionStatConditionType>(InValueType);

	switch (valueType)
	{
	case SkillExecuteConditionStatConditionType::VALUE:
		OutText = FText::AsNumber(InValue);
		break;
	case SkillExecuteConditionStatConditionType::RATIO:
		FGsEffectTextFuncText::GetValueRatioText(InValue, true, OutText);
		break;
	}
}

void FGsEffectTextParseSkillExeConditionStat::GetTextCondition(int32 InType, OUT FText& OutText)
{
	SkillExecuteConditionStatConditionMethod conditionType = static_cast<SkillExecuteConditionStatConditionMethod>(InType);

	switch (conditionType)
	{
	case SkillExecuteConditionStatConditionMethod::LESS:
		// TEXT: 미만
		FText::FindText(TEXT("TooltipText"), TEXT("ExecuteCondition_STAT_condition_LESS"), OutText);
		break;
	case SkillExecuteConditionStatConditionMethod::MORE:
		// TEXT: 이상
		FText::FindText(TEXT("TooltipText"), TEXT("ExecuteCondition_STAT_condition_MORE"), OutText);
		break;
	}
}