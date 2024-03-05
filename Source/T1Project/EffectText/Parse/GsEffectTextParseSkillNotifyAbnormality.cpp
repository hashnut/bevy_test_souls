// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillNotifyAbnormality.h"
#include "DataSchema/Skill/GsSchemaSkillNotifyElement.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "EffectText/GsEffectTextParser.h"
#include "EffectText/Function/GsEffectTextFuncText.h"
#include "UTIL/GsTableUtil.h"


bool FGsEffectTextParseSkillNotifyAbnormality::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (EGsETSkillAbnormalityParam::ET_MaxParam == InData->intParams.Num()) ? true : false;
}

bool FGsEffectTextParseSkillNotifyAbnormality::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->textParams[0].IsEmpty())
	{
		OutData.Emplace(InData->textParams[0].ToString());
		return true;
	}
#endif

	AbnormalityApplyType applyType = static_cast<AbnormalityApplyType>(InData->intParams[EGsETSkillAbnormalityParam::ET_ApplyType]);
	switch (applyType)
	{
	case AbnormalityApplyType::ADD:
		return _parser->GetStringAbnormality(InData->intParams[EGsETSkillAbnormalityParam::ET_AbnormalityId], InLevel, InOption, OutData);
	case AbnormalityApplyType::ADD_GROUP:
	{
		FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::ABNORAMLITY_APPLY_TYPE_ADD_GROUP, InOption);		
		return _parser->GetStringAbnormality(InData->intParams[EGsETSkillAbnormalityParam::ET_AbnormalityId], InLevel, InOption, OutData);
	}
	break;
	case AbnormalityApplyType::REMOVE:
	{
		if (InData->textParams.IsValidIndex(1))
		{
			FText textRemove;
			if (GetTextRemove(InData->textParams[1], textRemove))
			{
				OutData.Emplace(textRemove.ToString());
				return true;
			}
		}
	}
	break;
	default:
	{
		FText textApplyType;
		if (FGsEffectTextFuncText::GetAbnormalityApplyType(applyType, textApplyType))
		{
			OutData.Emplace(textApplyType.ToString());
			return true;
		}
	}	
	break;
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyAbnormality::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->textParams[0].IsEmpty())
	{
		TPair<FText, FText> pairText;
		pairText.Key = InData->textParams[0];
		pairText.Value = FText::GetEmpty();

		OutData.Emplace(pairText);
		return true;
	}
#endif

	AbnormalityApplyType applyType = static_cast<AbnormalityApplyType>(InData->intParams[EGsETSkillAbnormalityParam::ET_ApplyType]);
	switch (applyType)
	{
	case AbnormalityApplyType::ADD:
		return _parser->GetStringPairAbnormality(InData->intParams[EGsETSkillAbnormalityParam::ET_AbnormalityId], InLevel, InOption, OutData);
	case AbnormalityApplyType::ADD_GROUP:
	{
		FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::ABNORAMLITY_APPLY_TYPE_ADD_GROUP, InOption);
		return _parser->GetStringPairAbnormality(InData->intParams[EGsETSkillAbnormalityParam::ET_AbnormalityId], InLevel, InOption, OutData);
	}
	break;
	case AbnormalityApplyType::REMOVE:
	{
		if (InData->textParams.IsValidIndex(1))
		{
			TPair<FText, FText> pairText;
			if (GetTextRemove(InData->textParams[1], pairText.Key))
			{
				pairText.Value = FText::GetEmpty();
				OutData.Emplace(pairText);
				return true;
			}
		}
	}
	break;
	default:
	{
		TPair<FText, FText> pairText;
		if (FGsEffectTextFuncText::GetAbnormalityApplyType(applyType, pairText.Key))
		{
			OutData.Emplace(pairText);
			return true;
		}
	}
	break;
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyAbnormality::GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT FGsEffectTextCollection& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->textParams[0].IsEmpty())
	{
		TPair<FText, FText> pairText;
		pairText.Key = InData->textParams[0];
		pairText.Value = FText::GetEmpty();

		OutData._effectList.Emplace(pairText);
		return true;
	}
#endif

	AbnormalityApplyType applyType = static_cast<AbnormalityApplyType>(InData->intParams[EGsETSkillAbnormalityParam::ET_ApplyType]);
	switch (applyType)
	{
	case AbnormalityApplyType::ADD:
		return _parser->GetStringCollectionAbnormality(InData->intParams[EGsETSkillAbnormalityParam::ET_AbnormalityId], InLevel, InOption, OutData);
	case AbnormalityApplyType::ADD_GROUP:
	{
		FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::ABNORAMLITY_APPLY_TYPE_ADD_GROUP, InOption);
		return _parser->GetStringCollectionAbnormality(InData->intParams[EGsETSkillAbnormalityParam::ET_AbnormalityId], InLevel, InOption, OutData);
	}
	break;
	case AbnormalityApplyType::REMOVE:
	{
		if (InData->textParams.IsValidIndex(1))
		{
			TPair<FText, FText> pairText;
			if (GetTextRemove(InData->textParams[1], pairText.Key))
			{
				OutData._effectList.Emplace(pairText);
				return true;
			}
		}
	}
	break;
	default:
	{
		TPair<FText, FText> pairText;
		if (FGsEffectTextFuncText::GetAbnormalityApplyType(applyType, pairText.Key))
		{
			OutData._effectList.Emplace(pairText);
			return true;
		}
	}
	break;
	}

	return false;
}

int FGsEffectTextParseSkillNotifyAbnormality::IsExistSameDate(const FGsSchemaSkillNotifyElement* InData, 
	TArray<FGsSchemaEffectTextData>& InPrevData)
{
	if (nullptr == InData)
	{
		return -1;
	}

	const FGsSchemaSkillNotifyHitSet* hitSet = InData->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
	if (nullptr == hitSet)
	{
		return -1;
	}

	// 1,2번 중 하나라도 다르면 다른 데이터로 본다

	// 1번 동일 효과 있는지 검사
	if (AbnormalityApplyType::NONE != hitSet->abnormalityApplyType1)
	{
		if (false == IsSameAbnormalityEffect(InData->executeConditionId, hitSet->abnormalityApplyType1, hitSet->abnormalityId1, InPrevData))
		{
			return 0;
		}
	}

	// 2번 동일 효과 있는지 검사
	if (AbnormalityApplyType::NONE != hitSet->abnormalityApplyType2)
	{
		if (false == IsSameAbnormalityEffect(InData->executeConditionId, hitSet->abnormalityApplyType2, hitSet->abnormalityId2, InPrevData))
		{
			return 0;
		}
	}

	return 1;
}

bool FGsEffectTextParseSkillNotifyAbnormality::SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (nullptr == InData)
	{
		return false;
	}

	// 없으면 안되는 정보이므로 바로 false 리턴
	const FGsSchemaSkillNotifyHitSet* hitSet = InData->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
	if (nullptr == hitSet)
	{
		return false;
	}

	// 1,2번 중 중복이 있을수도 있으니 다시 검사해서 동일하지 않은 정보만 넣음

	// 1번
	if (AbnormalityApplyType::NONE != hitSet->abnormalityApplyType1)
	{
		if (false == IsSameAbnormalityEffect(InData->executeConditionId, hitSet->abnormalityApplyType1, hitSet->abnormalityId1, OutData))
		{
			FGsSchemaEffectTextData data;
			data.parseType = GetParseType();
			data.textParams.Emplace(InData->effectText);
			data.intParams.Emplace(InData->executeConditionId);

			SetAbnormalityDataByType(hitSet->abnormalityApplyType1, hitSet->abnormalityId1, data);

			OutData.Emplace(data);
		}
	}

	// 2번
	if (AbnormalityApplyType::NONE != hitSet->abnormalityApplyType2)
	{
		if (false == IsSameAbnormalityEffect(InData->executeConditionId, hitSet->abnormalityApplyType2, hitSet->abnormalityId2, OutData))
		{
			FGsSchemaEffectTextData data;
			data.parseType = GetParseType();
			data.textParams.Emplace(InData->effectText);
			data.intParams.Emplace(InData->executeConditionId);

			SetAbnormalityDataByType(hitSet->abnormalityApplyType2, hitSet->abnormalityId2, data);

			OutData.Emplace(data);
		}
	}

	return true;
}

void FGsEffectTextParseSkillNotifyAbnormality::SetAbnormalityDataByType(AbnormalityApplyType InType,
	int32 InAbnormalityId, OUT FGsSchemaEffectTextData& OutData)
{
	OutData.intParams.Emplace(static_cast<int32>(InType));

	switch (InType)
	{
	case AbnormalityApplyType::ADD:
	case AbnormalityApplyType::ADD_GROUP:
	{
		OutData.intParams.Emplace(InAbnormalityId);
	}
	break;
	case AbnormalityApplyType::REMOVE:
	{
		OutData.intParams.Emplace(InAbnormalityId);
		SetAbnormalityName(InAbnormalityId, OutData);
	}
	break;
	default:
	{
		// 나머지 경우들은 id를 따로 사용하지 않지만, Vaild체크를 위해 INVALID값으로 넣어줌
		OutData.intParams.Emplace(INVALID_ABNORMALITY_ID);
	}
	break;
	}
}

void FGsEffectTextParseSkillNotifyAbnormality::SetAbnormalityName(int32 InAbnormalityId, 
	OUT FGsSchemaEffectTextData& OutData)
{
	const FGsSchemaAbnormalitySet* row =
		UGsTableUtil::FindRowById<UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(InAbnormalityId);
	if (row)
	{
		OutData.textParams.Emplace(row->ingameName);
	}
}

bool FGsEffectTextParseSkillNotifyAbnormality::IsSameAbnormalityEffect(int32 InExecuteConditionId,
	AbnormalityApplyType InType, int32 InAbnormalityId, TArray<FGsSchemaEffectTextData>& InData)
{
	for (FGsSchemaEffectTextData& effectTextData : InData)
	{
		// 같은 타입끼리만 검사
		if (GetParseType() != effectTextData.parseType)
		{
			continue;
		}

		if (InExecuteConditionId == effectTextData.intParams[EGsETSkillAbnormalityParam::ET_ExecuteConditionId])
		{
			if (static_cast<int32>(InType) == effectTextData.intParams[EGsETSkillAbnormalityParam::ET_ApplyType])
			{
				if (AbnormalityApplyType::ADD == InType ||
					AbnormalityApplyType::ADD_GROUP == InType ||
					AbnormalityApplyType::REMOVE == InType)
				{
					if (InAbnormalityId == effectTextData.intParams[EGsETSkillAbnormalityParam::ET_AbnormalityId])
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyAbnormality::GetTextRemove(const FText& InAbnormalityName, OUT FText& OutText)
{
	// {0} 해제
	FText textFormat;
	if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_abnormalityApply_REMOVE"), textFormat))
	{
		OutText = FText::Format(textFormat, InAbnormalityName);
		return true;
	}

	return false;
}
