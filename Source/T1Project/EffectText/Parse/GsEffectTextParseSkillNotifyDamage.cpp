// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillNotifyDamage.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "EffectText/GsEffectTextParser.h"
#include "EffectText/Function/GsEffectTextFuncSkillDamage.h"


bool FGsEffectTextParseSkillNotifyDamage::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return FGsEffectTextFuncSkillDamage::IsValidData(InData);
}

bool FGsEffectTextParseSkillNotifyDamage::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	FString strDamage;
	FGsEffectTextFuncSkillDamage::GetString(InData, strDamage);

	OutData.Emplace(strDamage);

	return true;
}

bool FGsEffectTextParseSkillNotifyDamage::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{	
	if (false == IsValidData(InData))
	{
		return false;
	}

	FString strDamage;
	FGsEffectTextFuncSkillDamage::GetString(InData, strDamage);

	TPair<FText, FText> pairDamage;
	pairDamage.Key = FText::FromString(strDamage);

	OutData.Emplace(pairDamage);

	return true;
}

bool FGsEffectTextParseSkillNotifyDamage::SetData(const FGsSchemaSkillNotifySet* InData, 
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (InData->type != SkillNotifyType::DAMAGE)
	{
		return false;
	}

	if (0 == InData->notifyElementList.Num())
	{
		return false;
	}

	// 동일한 데미지의 경우, 이 노티에서만의 카운트로 따로 세기 위해 임시 리스트에 담음
	TArray<FGsSchemaEffectTextData> tempList;
	if (false == FGsEffectTextFuncSkillDamage::SetData(GetParseType(), InData, tempList))
	{
		return false;
	}

	if (0 < tempList.Num())
	{
		OutData.Append(tempList);
	}
	
	return true;
}
