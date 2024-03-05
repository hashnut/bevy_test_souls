// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParsePassivityBase.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInfo.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeSkillNotifySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeAbnormalitySet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "EffectText/GsEffectTextParser.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "Skill/GsSkill.h"
#include "Item/GsItemHelper.h"
#include "UTIL/GsTableUtil.h"


//----------------------------------------------------------------------------------------------------------------------
bool FGsEffectTextParsePassivityBase::FGsInvokeTypeData::IsCondition() const
{
	return (0 < _data->intParams[0]) ? true : false;
}

int32 FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetStartIndex() const
{
	// 조건정보 유무에 따라 데이터 길이가 다름
	return IsCondition() ? 5 : 1;
}

int32 FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetId() const
{
	int32 startIndex = GetStartIndex();
	return _data->intParams[startIndex];
}

PassivityCauseType FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetCause() const
{
	int32 startIndex = GetStartIndex();
	return static_cast<PassivityCauseType>(_data->intParams[startIndex + 1]);
}

int32 FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetRatePercent() const
{
	int32 startIndex = GetStartIndex();
	return _data->intParams[startIndex + 2];
}

PassivityInvokeTargetType FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetTarget() const
{
	int32 startIndex = GetStartIndex();
	return static_cast<PassivityInvokeTargetType>(_data->intParams[startIndex + 3]);
}

int32 FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetCauseUserSkillSetId() const
{
	int32 startIndex = GetStartIndex();
	return _data->intParams[startIndex + 4];
}

int32 FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetCauseItemId() const
{
	int32 startIndex = GetStartIndex();
	return _data->intParams[startIndex + 4];
}

ItemGrade FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetCauseItemGrade() const
{
	int32 startIndex = GetStartIndex();
	return static_cast<ItemGrade>(_data->intParams[startIndex + 5]);
}

ItemCategorySub FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetCauseItemCategorySub() const
{
	int32 startIndex = GetStartIndex();
	return static_cast<ItemCategorySub>(_data->intParams[startIndex + 6]);
}

int32 FGsEffectTextParsePassivityBase::FGsInvokeTypeData::GetCauseAbnormalityId() const
{
	int32 startIndex = GetStartIndex();
	return _data->intParams[startIndex + 4];
}
//----------------------------------------------------------------------------------------------------------------------
bool FGsEffectTextParsePassivityBase::GetStringPassivityCondition(const FGsSchemaEffectTextData* InData, OUT FString& OutData)
{
	// 조건 없음
	if (false == IsValidData(InData) ||
		0 == InData->intParams[0])
	{
		return false;
	}

	return GetStringCondition(InData, OutData);
}

void FGsEffectTextParsePassivityBase::SetConditionData(const FGsSchemaPassivityEffectInfo& InData, OUT FGsSchemaEffectTextData& OutData)
{
	if (IsCondition(InData))
	{
		OutData.intParams.Emplace(1);
		OutData.intParams.Emplace(static_cast<int32>(InData.conditionInfo.type));
		OutData.intParams.Emplace(static_cast<int32>(InData.conditionInfo.method));
		OutData.intParams.Emplace(InData.conditionInfo.param1);
		OutData.intParams.Emplace(InData.conditionInfo.param2);
	}
	else
	{
		OutData.intParams.Emplace(0);
	}
}

void FGsEffectTextParsePassivityBase::SetAdditionalCauseParam(PassivityCauseType InType, const FGsSchemaPassivityCauseInfo& InCauseInfo,
	OUT FGsSchemaEffectTextData& OutData)
{
	// [TextParser] 데이터 구조 변경으로 참조 데이터 처리 간소화
	switch (InType)
	{
	case PassivityCauseType::START_SKILL_SELF:
	{
		// FGsSchemaUserSkillSet
		OutData.intParams.Emplace(InCauseInfo.id);
	}
	break;
	case PassivityCauseType::EQUIP_ITEM_SELF:
	{
		// FGsSchemaItemCommon
		OutData.intParams.Emplace(InCauseInfo.id);
		OutData.intParams.Emplace(static_cast<int32>(InCauseInfo.itemGrade));
		OutData.intParams.Emplace(static_cast<int32>(InCauseInfo.itemCategorySub));
	}
	break;
	case PassivityCauseType::UPSERT_ABNORMALITY_TARGET:
	{
		// FGsSchemaAbnormalitySet
		OutData.intParams.Emplace(InCauseInfo.id);
	}
	break;
	}
}

bool FGsEffectTextParsePassivityBase::GetStringCondition(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	PassivityConditionType conditionType = static_cast<PassivityConditionType>(InData->intParams[1]);
	FText textCondition;

	switch (conditionType)
	{
	case PassivityConditionType::HP_RATE:
		// TEXT: HP
		FText::FindText(TEXT("TooltipText"), TEXT("Passivity_conditionType_HPRATE"), textCondition);
		break;
	case PassivityConditionType::MP_RATE:
		// TEXT: MP
		FText::FindText(TEXT("TooltipText"), TEXT("Passivity_conditionType_MPRATE"), textCondition);
		break;
	case PassivityConditionType::ABNORMALITY_ID:
	{
		// TEXT: {0} 상태일 때,
		FText textFormat;
		if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_conditionMethod_ABNORMALITY_ID"), textFormat))
		{
			// param1값이 AbnormalityId임
			FText textAbnormalityName;
			GetAbnormalityIngameName(InData->intParams[3], textAbnormalityName);

			FText textOut = FText::Format(textFormat, textAbnormalityName);
			OutString = textOut.ToString();
		}
		return true;
	}
	default:
		// [B1] 위 세개 이외의 타입은 변경 계획 있어서 현재 규칙 필요 없음. 버그 상황은 아니므로 true 리턴 
		return true;
	}

	OutString = textCondition.ToString();
	OutString.Append(TEXT(" "));

	PassivityConditionMethod methodType = static_cast<PassivityConditionMethod>(InData->intParams[2]);
	AddStringConditionMethod(methodType, InData->intParams[3], InData->intParams[4], OutString);	

	return true;
}

void FGsEffectTextParsePassivityBase::AddStringConditionMethod(PassivityConditionMethod InType,
	int32 InParam1, int32 InParam2, OUT FString& OutString)
{
	FText textFormat;
	FText textMethod;
	switch (InType)
	{
	case PassivityConditionMethod::BELOW:
	{
		// TEXT: {0}% 이하일 때,
		if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_conditionMethod_BELOW"), textFormat))
		{
			textMethod = FText::Format(textFormat, InParam1);
		}
	}
	break;
	case PassivityConditionMethod::ABOVE:
	{
		// TEXT: {0}% 이상일 때,
		if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_conditionMethod_ABOVE"), textFormat))
		{
			textMethod = FText::Format(textFormat, InParam1);
		}
	}
	break;
	case PassivityConditionMethod::EQUAL:
	{
		// TEXT: {0}% 일 때,
		if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_conditionMethod_EQUAL"), textFormat))
		{
			textMethod = FText::Format(textFormat, InParam1);
		}
	}
	break;
	case PassivityConditionMethod::RANGE:
	{
		// TEXT: {0}~{1}% 일 때,
		if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_conditionMethod_RANGE"), textFormat))
		{
			textMethod = FText::Format(textFormat, InParam1, InParam2);
		}
	}
	break;
	}

	OutString.Append(textMethod.ToString());
}

bool FGsEffectTextParsePassivityBase::IsCondition(const FGsSchemaPassivityEffectInfo& InData) const
{
	if (PassivityConditionType::NONE == InData.conditionInfo.type ||
		PassivityConditionMethod::NONE == InData.conditionInfo.method)
	{
		return false;
	}

	return true;
}

bool FGsEffectTextParsePassivityBase::IsCondition(const FGsSchemaEffectTextData* InData) const
{
	return (0 < InData->intParams[0]) ? true : false;
}

void FGsEffectTextParsePassivityBase::AddStringCause(const FGsInvokeTypeData& InInvokeData, int32 InOption, 
	OUT FString& OutSring)
{
	if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::SKIP_INVOKE_CAUSE))
	{
		return;
	}

	FText textCause;
	GetTextPassivityCause(InInvokeData, textCause);

	if (false == textCause.IsEmpty())
	{
		if (false == OutSring.IsEmpty())
		{
			OutSring.Append(TEXT(" "));
		}
		OutSring.Append(textCause.ToString());
	}
}

void FGsEffectTextParsePassivityBase::AddStringRatePercent(int32 InRatePercent, int32 InOption, OUT FString& OutSring)
{
	if (0 == InRatePercent)
	{
		return;
	}

	if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::SKIP_INVOKE_RATE))
	{
		return;
	}

	// TEXT: {0}% 확률로
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Rate"), textFormat))
	{
		FText textRate = FText::Format(textFormat, InRatePercent);

		if (false == OutSring.IsEmpty())
		{
			OutSring.Append(TEXT(" "));
		}

		OutSring.Append(textRate.ToString());
	}
}

void FGsEffectTextParsePassivityBase::AddStringTarget(PassivityInvokeTargetType InTarget, int32 InOption,
	OUT FString& OutSring)
{
	if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::SKIP_INVOKE_TARGET))
	{
		return;
	}

	FText textTarget;
	switch (InTarget)
	{
	case PassivityInvokeTargetType::SELF:
	{		
		if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::PASSIVITY_INVOKE_TARGET_SHORT))
		{
			// TEXT: 자신
			FText::FindText(TEXT("TooltipText"), TEXT("FairySkill_target_SELF"), textTarget);
		}
		else
		{
			// TEXT: 나에게
			FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_target_SELF"), textTarget);
		}
	}
	break;
	case PassivityInvokeTargetType::TARGET:
	{
		if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::PASSIVITY_INVOKE_TARGET_SHORT))
		{
			// TEXT: 대상
			FText::FindText(TEXT("TooltipText"), TEXT("FairySkill_target_TARGET"), textTarget);
		}
		else
		{
			// TEXT: 대상에게
			FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_target_TARGET"), textTarget);
		}
	}	
	break;
	}

	if (false == textTarget.IsEmpty())
	{
		if (false == OutSring.IsEmpty())
		{
			OutSring.Append(TEXT(" "));
		}

		OutSring.Append(textTarget.ToString());
	}
}

void FGsEffectTextParsePassivityBase::GetTextPassivityCause(const FGsInvokeTypeData& InInvokeData, OUT FText& OutText)
{
	PassivityCauseType causeType = InInvokeData.GetCause();

	if (FGsEffectTextFuncText::GetPassivityCauseText(causeType, OutText))
	{
		return;
	}
	
	// 단순 텍스트가 아니라 별도 처리규칙이 있는 항목들
	switch (causeType)
	{	
	case PassivityCauseType::START_SKILL_SELF:
	{
		const FGsSchemaUserSkillSet* skillSet =
			UGsTableUtil::FindRowById<UGsTableUserSkillSet, FGsSchemaUserSkillSet>(InInvokeData.GetCauseUserSkillSetId());
		if (skillSet)
		{
			// TEXT: 스킬 {skillName} 사용 시
			FText textFormat;
			if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_STARTSKILLSELF"), textFormat))
			{
				OutText = FText::Format(textFormat, skillSet->nameText);
			}
		}
		else
		{
			// TEXT: 평타 및 스킬 사용 시
			FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_STARTSKILLSELF_None"), OutText);
		}
	}		
	break;
	case PassivityCauseType::EQUIP_ITEM_SELF:
	{
		FString strItem;
		int32 itemId = InInvokeData.GetCauseItemId();
		if (0 == itemId)
		{
			ItemGrade grade = InInvokeData.GetCauseItemGrade();
			if (ItemGrade::NONE != grade)
			{
				FText textGrade;
				if (FGsItemHelper::GetItemGradeText(grade, textGrade))
				{
					if (false == strItem.IsEmpty())
					{
						strItem.Append(TEXT(" "));
					}

					strItem.Append(textGrade.ToString());
				}
			}

			ItemCategorySub category = InInvokeData.GetCauseItemCategorySub();
			if (ItemCategorySub::NONE < category &&
				ItemCategorySub::SHOTGUN >= category)
			{
				FText textItemCategory;
				GetTextItemCategory(category, textItemCategory);
				if (false == textItemCategory.IsEmpty())
				{
					if (false == strItem.IsEmpty())
					{
						strItem.Append(TEXT(" "));
					}

					strItem.Append(textItemCategory.ToString());
				}
			}
		}
		else
		{
			const FGsSchemaItemCommon* itemCommon =
				UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(itemId);
			if (itemCommon)
			{
				strItem = itemCommon->name.ToString();
			}
		}		

		if (false == strItem.IsEmpty())
		{
			// TEXT: {0} 착용 중
			FText textFormatEquip;
			if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_EquipItem"), textFormatEquip))
			{
				OutText = FText::Format(textFormatEquip, FText::FromString(strItem));
			}
		}
	}		
	break;
	case PassivityCauseType::UPSERT_ABNORMALITY_TARGET:
	{
		// TEXT: {abnormalityName} 상태 대상
		FText textFormat;
		if (FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_UPSERTABNORMALITY"), textFormat))
		{
			FText textAbnormalityName;
			GetAbnormalityIngameName(InInvokeData.GetCauseAbnormalityId(), textAbnormalityName);

			OutText = FText::Format(textFormat, textAbnormalityName);
		}
	}	
	break;
	}
}

void FGsEffectTextParsePassivityBase::GetTextItemCategory(ItemCategorySub InType, OUT FText& OutText)
{
	FGsEffectTextFuncText::GetItemCategoryText(InType, OutText);
}

void FGsEffectTextParsePassivityBase::GetTextWeaponType(CreatureWeaponType InType, OUT FText& OutText)
{
	FGsEffectTextFuncText::GetCreatureWeaponText(InType, OutText);
}

void FGsEffectTextParsePassivityBase::GetAbnormalityIngameName(int32 InAbnormalityId, OUT FText& OutText)
{
	if (const FGsSchemaAbnormalitySet* abnormalitySet =
		UGsTableUtil::FindRowById<UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(InAbnormalityId))
	{
		OutText = abnormalitySet->ingameName;
	}
}