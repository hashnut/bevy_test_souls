// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillExeConditionPassivity.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/Skill/GsSchemaSkillExecuteConditionSet.h"
#include "DataSchema/Skill/Param/GsSchemaSkillExecuteConditionMethodPassivity.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"


bool FGsEffectTextParseSkillExeConditionPassivity::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (2 == InData->intParams.Num()) ? true : false;
}

bool FGsEffectTextParseSkillExeConditionPassivity::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParseSkillExeConditionPassivity::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParseSkillExeConditionPassivity::SetSkillConditionData(const FGsSchemaSkillExecuteConditionSet* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	const FGsSchemaSkillExecuteConditionMethodPassivity* param =
		InData->conditionMethodParam.GetRowByType<FGsSchemaSkillExecuteConditionMethodPassivity>();
	if (nullptr == param)
	{
		return false;
	}

	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();

	data.intParams.Emplace(static_cast<int32>(param->target)); // 1 : 타겟
	data.intParams.Emplace(static_cast<int32>(param->value)); // 2 : PassivityId

	OutData.Emplace(data);

	return true;
}

void FGsEffectTextParseSkillExeConditionPassivity::GetTextInter(const FGsSchemaEffectTextData* InData, OUT FText& OutText)
{
	// 대상
	FText textTarget;
	SkillTargetType targetType = static_cast<SkillTargetType>(InData->intParams[0]);

	FGsEffectTextFuncText::GetSkillTargetTypeText(targetType, textTarget);

	// 이름
	FText textPassivityName;
	FGsEffectTextFuncText::GetPassivityIdNameText(InData->intParams[1], textPassivityName);

	// 글자 조합
	// TEXT: {0}|hpp(이,가) {1}을 가지고 있을 때, 추가 효과:
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("ExecuteCondition_PASSIVITY_tooltip"), textFormat))
	{
		OutText = FText::Format(textFormat, textTarget, textPassivityName);
	}
}
