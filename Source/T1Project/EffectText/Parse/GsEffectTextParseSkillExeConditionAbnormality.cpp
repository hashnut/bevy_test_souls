// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillExeConditionAbnormality.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/Skill/GsSchemaSkillExecuteConditionSet.h"
#include "DataSchema/Skill/Param/GsSchemaSkillExecuteConditionMethodAbnormality.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"


bool FGsEffectTextParseSkillExeConditionAbnormality::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (EGsETSkillConditionAbIndex::ET_MAX_INDEX == InData->intParams.Num()) ? true : false;;
}

bool FGsEffectTextParseSkillExeConditionAbnormality::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParseSkillExeConditionAbnormality::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParseSkillExeConditionAbnormality::SetSkillConditionData(const FGsSchemaSkillExecuteConditionSet* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	const FGsSchemaSkillExecuteConditionMethodAbnormality* param =
		InData->conditionMethodParam.GetRowByType<FGsSchemaSkillExecuteConditionMethodAbnormality>();
	if (nullptr == param)
	{
		return false;
	}

	int32 abnormalityDataValue = -1;
	switch (param->type)
	{
	case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_ID:
	case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_GROUP_ID: // 주의: 추후 룰 변경 예정
		abnormalityDataValue = param->param2;
		break;
	case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_TYPE:
		abnormalityDataValue = static_cast<int32>(param->param1);
		break;
	}

	if (-1 == abnormalityDataValue)
	{
		return false;
	}

	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();

	data.intParams.Emplace(static_cast<int32>(param->target)); // TARGET
	data.intParams.Emplace(static_cast<int32>(param->type)); // ABNORMALITY_DATA_TYPE
	data.intParams.Emplace(abnormalityDataValue); // ABNORMALITY_DATA_VALUE
	data.intParams.Emplace(static_cast<int32>(param->setAbnormalityAllow)); // ALLOW_TYPE
	
	OutData.Emplace(data);

	return true;
}

void FGsEffectTextParseSkillExeConditionAbnormality::GetTextInter(const FGsSchemaEffectTextData* InData, 
	OUT FText& OutText)
{
	// 대상
	FText textTarget;
	SkillTargetType targetType = static_cast<SkillTargetType>(
		InData->intParams[EGsETSkillConditionAbIndex::ET_TARGET]);

	FGsEffectTextFuncText::GetSkillTargetTypeText(targetType, textTarget);

	// 이름
	FText textAbName;
	GetTextAbnormalityName(
		InData->intParams[EGsETSkillConditionAbIndex::ET_ABNORMALITY_DATA_TYPE],
		InData->intParams[EGsETSkillConditionAbIndex::ET_ABNORMALITY_DATA_VALUE],
		textAbName);	

	// 보유시/미보유시
	FText textAllow;
	GetTextAllowType(InData->intParams[EGsETSkillConditionAbIndex::ET_ALLOW_TYPE], textAllow);

	// 글자 조합
	// TEXT: {0}|hpp(이,가) {1}을 {2}, 추가 효과:
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("ExecuteCondition_ABNORMALITY_tooltip"), textFormat))
	{
		OutText = FText::Format(textFormat, textTarget, textAbName, textAllow);
	}
}

void FGsEffectTextParseSkillExeConditionAbnormality::GetTextAbnormalityName(int32 InType, int32 InValue, 
	OUT FText& OutText)
{
	SkillExecuteConditionAbnormalityConditionType dataType = 
		static_cast<SkillExecuteConditionAbnormalityConditionType>(InType);

	switch (dataType)
	{
	case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_ID:
	case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_GROUP_ID: // 주의: 추후 룰 변경 예정
	{
		FGsEffectTextFuncText::GetAbnormalityIdNameText(InValue, OutText);
	}
	break;
	case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_TYPE:
	{
		AbnormalityEffectType effectType = static_cast<AbnormalityEffectType>(InValue);

		FGsEffectTextFuncText::GetAbnormalityNameText(effectType, OutText);
	}
	break;
	}
}

void FGsEffectTextParseSkillExeConditionAbnormality::GetTextAllowType(int32 InType, OUT FText& OutText)
{
	SetAbnormalityAllow allowType = static_cast<SetAbnormalityAllow>(InType);

	switch (allowType)
	{
	case SetAbnormalityAllow::ALLOW:
		// TEXT: 보유 시
		FText::FindText(TEXT("TooltipText"), TEXT("ExecuteCondition_ABNORMALITY_condition_ALLOW"), OutText);
		break;
	case SetAbnormalityAllow::DISALLOW:
		// TEXT: 미보유 시
		FText::FindText(TEXT("TooltipText"), TEXT("ExecuteCondition_ABNORMALITY_condition_DISALLOW"), OutText);
		break;
	}
}
