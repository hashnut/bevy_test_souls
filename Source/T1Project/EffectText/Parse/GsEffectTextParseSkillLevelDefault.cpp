// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillLevelDefault.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelInfo.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"


bool FGsEffectTextParseSkillLevelDefault::IsValidData(const FGsSchemaEffectTextData* InData) const
{
#ifdef USE_CUSTOM_TEXT
	return (InData->textParams.IsValidIndex(0)) ? true : false;
#else
	return false;
#endif
}

bool FGsEffectTextParseSkillLevelDefault::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		OutData.Emplace(InData->textParams[0].ToString());
		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillLevelDefault::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (IsValidData(InData))
	{
		TPair<FText, FText> pairEffect;
		pairEffect.Key = InData->textParams[0];
		pairEffect.Value = FText::GetEmpty();

		OutData.Emplace(pairEffect);

		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillLevelDefault::SetData(const FGsSchemaSkillLevelInfo* InData, 
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();
	data.textParams.Emplace(InData->effectText);

	OutData.Emplace(data);

	return true;
}
