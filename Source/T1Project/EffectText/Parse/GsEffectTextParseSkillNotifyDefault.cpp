// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillNotifyDefault.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifyElement.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"


bool FGsEffectTextParseSkillNotifyDefault::IsValidData(const FGsSchemaEffectTextData* InData) const
{
#ifdef USE_CUSTOM_TEXT
	return (InData->textParams.IsValidIndex(0)) ? true : false;
#else
	return false;
#endif
}

bool FGsEffectTextParseSkillNotifyDefault::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		OutData.Emplace(InData->textParams[0].ToString());
		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyDefault::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParseSkillNotifyDefault::SetData(const FGsSchemaSkillNotifySet* InData, 
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();
	data.textParams.Emplace(InData->effectText);

	OutData.Emplace(data);
	return true;
}

bool FGsEffectTextParseSkillNotifyDefault::SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData, 
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();
	data.textParams.Emplace(InData->effectText);

	OutData.Emplace(data);
	return true;
}
