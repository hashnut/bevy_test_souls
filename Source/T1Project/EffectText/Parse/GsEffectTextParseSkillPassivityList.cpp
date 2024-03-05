// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillPassivityList.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "EffectText/GsEffectTextParser.h"


bool FGsEffectTextParseSkillPassivityList::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (0 < InData->intParams.Num()) ? true : false;
}

bool FGsEffectTextParseSkillPassivityList::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		for (int32 i = 0; i < InData->intParams.Num(); ++i)
		{
			_parser->GetStringPassivity(InData->intParams[i], InOption, OutData);
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillPassivityList::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (IsValidData(InData))
	{
		for (int32 i = 0; i < InData->intParams.Num(); ++i)
		{
			_parser->GetStringPairPassivity(InData->intParams[i], InOption, OutData);
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillPassivityList::SetData(const FGsSchemaSkillSet* InData, OUT FGsSchemaEffectTextData& OutData)
{
	OutData.parseType = GetParseType();

	for (const FGsSchemaPassivitySetRow& iter : InData->passivityIdList)
	{
		if (const FGsSchemaPassivitySet* passivitySet = iter.GetRow())
		{
			OutData.intParams.Emplace(passivitySet->id);
		}
	}

	return true;
}
