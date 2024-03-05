// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillLevelNotifyList.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelInfo.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "EffectText/GsEffectTextParser.h"
#include "UTIL/GsTableUtil.h"


bool FGsEffectTextParseSkillLevelNotifyList::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (0 < InData->intParams.Num()) ? true : false;
}

bool FGsEffectTextParseSkillLevelNotifyList::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		for (int32 i = 0; i < InData->intParams.Num(); ++i)
		{
			_parser->GetStringSkillNotify(InData->intParams[i], InLevel, InOption, OutData, true);
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillLevelNotifyList::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (IsValidData(InData))
	{
		for (int32 i = 0; i < InData->intParams.Num(); ++i)
		{
			_parser->GetStringPairSkillNotify(InData->intParams[i], InLevel, InOption, OutData, true);
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParseSkillLevelNotifyList::SetData(const FGsSchemaSkillLevelInfo* InData, 
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{	
	TArray<int32> abnormalityIdList;

	FGsSchemaEffectTextData data;
	data.parseType = GetParseType();

	for (int notifyId : InData->notifyInfoList)
	{
		const FGsSchemaSkillNotifySet* notifySet =
			UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(notifyId);
		if (notifySet)
		{	
			switch (notifySet->type)
			{
			case SkillNotifyType::DAMAGE:
			{
				// [TextParser] 리스트 형태이지만 툴팁은 0번만 사용한다
				if (0 == notifySet->notifyElementList.Num())
				{
					continue;
				}

				const FGsSchemaSkillNotifyElement* element = notifySet->notifyElementList[0].GetRow();
				if (nullptr == element)
				{
					continue;
				}

				const FGsSchemaSkillNotifyHitSet* hitSet = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
				if (nullptr == hitSet)
				{
					continue;
				}

				if (AbnormalityApplyType::ADD == hitSet->abnormalityApplyType1)
				{
					// 중복 abnormality일 경우 수집하지 않기 위해 체크함
					if (abnormalityIdList.Contains(hitSet->abnormalityId1))
					{
						continue;
					}
					
					abnormalityIdList.Emplace(hitSet->abnormalityId1);
					data.intParams.Emplace(notifySet->id);
				}
			}
			break;
			case SkillNotifyType::FIRE_PROJECTILE:
			case SkillNotifyType::VARY_HP_MP:
			{
				data.intParams.Emplace(notifySet->id);
			}
			break;
			}
		}
	}

	if (_parser->IsEmptyData(&data))
	{
		return false;
	}

	OutData.Emplace(data);
	return true;
}