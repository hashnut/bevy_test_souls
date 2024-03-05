// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParsePassivitySkillUp.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectSkillUpSet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "EffectText/GsEffectTextParser.h"
#include "UTIL/GsTableUtil.h"


bool FGsEffectTextParsePassivitySkillUp::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	if (3 <= InData->intParams.Num())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextParsePassivitySkillUp::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	FString strEffect;
	if (false == GetStringInter(InData, strEffect))
	{
		return false;
	}

	OutData.Emplace(strEffect);

	return true;
}

bool FGsEffectTextParsePassivitySkillUp::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{	
	FString strEffect;
	if (false == GetStringInter(InData, strEffect))
	{
		return false;
	}

	TPair<FText, FText> pairEffect;
	pairEffect.Key = FText::FromString(strEffect);
	pairEffect.Value = FText::GetEmpty();

	OutData.Emplace(pairEffect);

	return true;
}

bool FGsEffectTextParsePassivitySkillUp::SetData(const FGsSchemaPassivitySet* InData, int32 InIndex,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{	
	if (0 == InData->effectInfoList.Num())
	{
		return false;
	}

	const FGsSchemaPassivityEffectInfo& effectInfo = InData->effectInfoList[InIndex];

	const FGsSchemaPassivityEffectSkillUpSet* tableData =
		UGsTableUtil::FindRowById<UGsTablePassivityEffectSkillUpSet,
		FGsSchemaPassivityEffectSkillUpSet>(effectInfo.effectId);

	if (nullptr == tableData)
	{
		return false;
	}

	for (const FGsSchemaPassivityEffectSkillUpParam& param : tableData->param)
	{
		FGsSchemaEffectTextData data;
		data.parseType = GetParseType();
		data.textParams.Emplace(InData->effectText);
		SetConditionData(effectInfo, data);

		data.intParams.Emplace(param.userSkillSetId);
		data.intParams.Emplace(param.value);

		OutData.Emplace(data);
	}

	if (0 < OutData.Num())
	{
		return true;
	}

	return true;
}

bool FGsEffectTextParsePassivitySkillUp::GetStringInter(const FGsSchemaEffectTextData* InData, OUT FString& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	int startIndex = 1;
	if (IsCondition(InData))
	{
		startIndex = 5;
	}

	int32 userSkillSetId = InData->intParams[startIndex];
	int32 value = InData->intParams[startIndex + 1];

	const FGsSchemaUserSkillSet* userSkillSet =
		UGsTableUtil::FindRowById<UGsTableUserSkillSet, FGsSchemaUserSkillSet>(userSkillSetId);
	if (nullptr == userSkillSet)
	{
		return false;
	}

	const FGsSchemaSkillSet* skillSet = userSkillSet->maleSkillId.GetRow();
	if (nullptr == skillSet)
	{
		return false;
	}

	FText textWeapon;
	if (CreatureWeaponType::NONE != skillSet->requireWeapon &&
		CreatureWeaponType::HAND != skillSet->requireWeapon)
	{
		GetTextWeaponType(skillSet->requireWeapon, textWeapon);
	}

	// 무기
	if (false == OutData.IsEmpty())
	{
		OutData.Append(TEXT(" "));
	}
	OutData.Append(textWeapon.ToString());

	// 스킬 이름
	if (false == OutData.IsEmpty())
	{
		OutData.Append(TEXT(" "));
	}
	OutData.Append(userSkillSet->nameText.ToString());

	// 레벨
	OutData.Append(TEXT(" Lv.+"));
	OutData.AppendInt(value);

	return true;
}
