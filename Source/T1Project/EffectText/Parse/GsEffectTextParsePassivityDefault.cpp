// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParsePassivityDefault.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"


bool FGsEffectTextParsePassivityDefault::IsValidData(const FGsSchemaEffectTextData* InData) const
{
#ifdef USE_CUSTOM_TEXT
	return (InData->textParams.IsValidIndex(0)) ? true : false;
#else
	return false;
#endif
}

bool FGsEffectTextParsePassivityDefault::GetString(const FGsSchemaEffectTextData* InData, int InLevel, 
	int32 InOption, OUT TArray<FString>& OutData)
{	
	if (IsValidData(InData))
	{
		OutData.Emplace(InData->textParams[0].ToString());
		return true;
	}

	return false;
}

bool FGsEffectTextParsePassivityDefault::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParsePassivityDefault::SetData(const FGsSchemaPassivitySet* InData, int32 InIndex, 
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (InData)
	{
		FGsSchemaEffectTextData data;
		data.parseType = GetParseType();
		data.textParams.Emplace(InData->effectText);

		OutData.Emplace(data);
		return true;
	}

	return false;
}
