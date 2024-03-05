// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillLevelDamage.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelInfo.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "EffectText/Function/GsEffectTextFuncSkillDamage.h"
#include "UTIL/GsTableUtil.h"


bool FGsEffectTextParseSkillLevelDamage::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return FGsEffectTextFuncSkillDamage::IsValidData(InData);
}

bool FGsEffectTextParseSkillLevelDamage::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParseSkillLevelDamage::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
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

bool FGsEffectTextParseSkillLevelDamage::GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT FGsEffectTextCollection& OutData)
{
	// 콜렉션 할 일 없음
	return false;
}

bool FGsEffectTextParseSkillLevelDamage::SetData(const FGsSchemaSkillLevelInfo* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	for (int notifyId : InData->notifyInfoList)
	{
		const FGsSchemaSkillNotifySet* notifySet =
			UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(notifyId);
		if (notifySet)
		{
			if (SkillNotifyType::DAMAGE == notifySet->type)
			{
				if (false == SetData(notifySet, OutData))
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool FGsEffectTextParseSkillLevelDamage::SetData(const FGsSchemaSkillNotifySet* InData, 
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

	// 노티쪽과 동일 로직 사용
	if (false == FGsEffectTextFuncSkillDamage::SetData(GetParseType(), InData, OutData))
	{
		return false;
	}

	return true;
}
