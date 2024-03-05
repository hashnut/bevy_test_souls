// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParser.h"

#include "EffectText/EGsEffectTextParseType.h"

#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormality.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormalityLevel.h"
#include "DataSchema/EffectText/GsSchemaEffectTextPassivity.h"
#include "DataSchema/EffectText/GsSchemaEffectTextSkill.h"
#include "DataSchema/EffectText/GsSchemaEffectTextSkillLevel.h"
#include "DataSchema/EffectText/GsSchemaEffectTextSkillNotify.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalityEffectTextSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"

#include "EffectText/Parse/GsEffectTextParseAbnormalityDefault.h"
#include "EffectText/Parse/GsEffectTextParseAbnormalityStat.h"
#include "EffectText/Parse/GsEffectTextParseAbnormalityDurationOnly.h"
#include "EffectText/Parse/GsEffectTextParseAbnormalityOvertime.h"

#include "EffectText/Parse/GsEffectTextParsePassivityDefault.h"
#include "EffectText/Parse/GsEffectTextParsePassivityStat.h"
#include "EffectText/Parse/GsEffectTextParsePassivityInvokeAbnormality.h"
#include "EffectText/Parse/GsEffectTextParsePassivityInvokeSkillNotify.h"
#include "EffectText/Parse/GsEffectTextParsePassivitySkillUp.h"

#include "EffectText/Parse/GsEffectTextParseSkillNotifyDefault.h"
//#include "EffectText/Parse/GsEffectTextParseSkillNotifyDamage.h"
#include "EffectText/Parse/GsEffectTextParseSkillNotifyAbnormality.h"
//#include "EffectText/Parse/GsEffectTextParseSkillNotifyHeal.h"
#include "EffectText/Parse/GsEffectTextParseSkillNotifyAbsorption.h"
#include "EffectText/Parse/GsEffectTextParseSkillNotifyProjectile.h"

#include "EffectText/Parse/GsEffectTextParseSkillLevelDefault.h"
//#include "EffectText/Parse/GsEffectTextParseSkillLevelDamage.h"
#include "EffectText/Parse/GsEffectTextParseSkillLevelNotifyList.h"
#include "EffectText/Parse/GsEffectTextParseSkillPassivityList.h"
#include "EffectText/Parse/GsEffectTextParseSkillExeConditionAbnormality.h"
#include "EffectText/Parse/GsEffectTextParseSkillExeConditionStat.h"
#include "EffectText/Parse/GsEffectTextParseSkillExeConditionPassivity.h"

#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "GsEffectTextDefine.h"


FGsEffectTextParser::~FGsEffectTextParser()
{
	Finalize();
}

void FGsEffectTextParser::GetMergedString(TArray<FString>& InList, int32 InOption, OUT FString& OutData)
{
	for (FString& itStr : InList)
	{
		if (false == OutData.IsEmpty())
		{
			if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::COMMA))
			{
				OutData.Append(TEXT(", "));
			}
			else
			{
				OutData.Append(TEXT("\n"));
			}
		}

		OutData.Append(itStr);
	}
}

void FGsEffectTextParser::GetMergedStringByPair(TArray<TPair<FText, FText>>& InList, int32 InOption, 
	OUT FString& OutData)
{
	for (TPair<FText, FText>& itPair : InList)
	{
		if (false == OutData.IsEmpty())
		{
			if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::COMMA))
			{
				OutData.Append(TEXT(", "));
			}
			else
			{
				OutData.Append(TEXT("\n"));
			}
		}

		OutData.Append(itPair.Key.ToString());
		
		if (false == itPair.Value.IsEmpty())
		{
			OutData.Append(TEXT(" "));
			OutData.Append(itPair.Value.ToString());
		}
	}
}

void FGsEffectTextParser::Initialize()
{
	_parseMap.Emplace(EGsEffectTextParseType::ABNORMALITY_DEFAULT, new FGsEffectTextParseAbnormalityDefault(this));
	_parseMap.Emplace(EGsEffectTextParseType::ABNORMALITY_STAT, new FGsEffectTextParseAbnormalityStat(this));
	_parseMap.Emplace(EGsEffectTextParseType::ABNORMALITY_DURATION_ONLY, new FGsEffectTextParseAbnormalityDurationOnly(this));
	_parseMap.Emplace(EGsEffectTextParseType::ABNORMALITY_OVERTIME, new FGsEffectTextParseAbnormalityOvertime(this));
	
	_parseMap.Emplace(EGsEffectTextParseType::PASSIVITY_DEFAULT, new FGsEffectTextParsePassivityDefault(this));
	_parseMap.Emplace(EGsEffectTextParseType::PASSIVITY_STAT, new FGsEffectTextParsePassivityStat(this));
	_parseMap.Emplace(EGsEffectTextParseType::PASSIVITY_INVOKE_ABNORMALITY, new FGsEffectTextParsePassivityInvokeAbnormality(this));
	_parseMap.Emplace(EGsEffectTextParseType::PASSIVITY_INVOKE_SKILL_NOTIFY, new FGsEffectTextParsePassivityInvokeSkillNotify(this));
	_parseMap.Emplace(EGsEffectTextParseType::PASSIVITY_SKILL_UP, new FGsEffectTextParsePassivitySkillUp(this));

	_parseMap.Emplace(EGsEffectTextParseType::SKILL_NOTIFY_DEFAULT, new FGsEffectTextParseSkillNotifyDefault(this));
	//_parseMap.Emplace(EGsEffectTextParseType::SKILL_NOTIFY_DAMAGE, new FGsEffectTextParseSkillNotifyDamage(this)); // 스킬이 아닌 Invoke에 의해 올 경우 이 규칙 사용
	_parseMap.Emplace(EGsEffectTextParseType::SKILL_NOTIFY_ABNORMALITY, new FGsEffectTextParseSkillNotifyAbnormality(this));
	//_parseMap.Emplace(EGsEffectTextParseType::SKILL_NOTIFY_HEAL, new FGsEffectTextParseSkillNotifyHeal(this));
	_parseMap.Emplace(EGsEffectTextParseType::SKILL_NOTIFY_ABSORPTION, new FGsEffectTextParseSkillNotifyAbsorption(this));
	_parseMap.Emplace(EGsEffectTextParseType::SKILL_NOTIFY_PROJECTILE, new FGsEffectTextParseSkillNotifyProjectile(this));

	_parseMap.Emplace(EGsEffectTextParseType::SKILL_LEVEL_DEFAULT, new FGsEffectTextParseSkillLevelDefault(this));
	//_parseMap.Emplace(EGsEffectTextParseType::SKILL_LEVEL_DAMAGE, new FGsEffectTextParseSkillLevelDamage(this)); // 스킬일 경우엔 데미지 규칙 사용
	_parseMap.Emplace(EGsEffectTextParseType::SKILL_LEVEL_NOTIFY_LIST, new FGsEffectTextParseSkillLevelNotifyList(this));

	_parseMap.Emplace(EGsEffectTextParseType::SKILL_PASSIVITY_LIST, new FGsEffectTextParseSkillPassivityList(this));

	_parseMap.Emplace(EGsEffectTextParseType::SKILL_EXE_CONDITION_ABNORMALITY, new FGsEffectTextParseSkillExeConditionAbnormality(this));
	_parseMap.Emplace(EGsEffectTextParseType::SKILL_EXE_CONDITION_STAT, new FGsEffectTextParseSkillExeConditionStat(this));
	_parseMap.Emplace(EGsEffectTextParseType::SKILL_EXE_CONDITION_PASSIVITY, new FGsEffectTextParseSkillExeConditionPassivity(this));
}

void FGsEffectTextParser::Finalize()
{
	for (TPair<EGsEffectTextParseType, FGsEffectTextParseBase*>& iter : _parseMap)
	{
		delete iter.Value;
		iter.Value = nullptr;
	}
	_parseMap.Empty();
}

bool FGsEffectTextParser::IsEmptyData(const FGsSchemaEffectTextData* InData) const
{
	if (0 == InData->textParams.Num() &&
		0 == InData->intParams.Num() &&
		0 == InData->floatParams.Num())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData)
{
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
	{
		return (*parseFunc)->GetString(InData, InLevel, InOption, OutData);
	}

	return false;
}

bool FGsEffectTextParser::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
	{
		return (*parseFunc)->GetStringPair(InData, InLevel, InOption, OutData);
	}

	return false;
}

bool FGsEffectTextParser::GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData)
{
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
	{
		return (*parseFunc)->GetStringCollection(InData, InLevel, InOption, OutData);
	}

	return false;
}

bool FGsEffectTextParser::GetStringAbnormality(int32 InId, int InLevel, int32 InOption, OUT TArray<FString>& OutData)
{
	const FGsSchemaEffectTextAbnormality* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextAbnormality, FGsSchemaEffectTextAbnormality>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextAbnormalityLevel& itLevel : table->dataList)
		{
			// 레벨 정보를 사용하지 않는 항목(NOT_USE_EFFECT_TEXT_LEVEL)은 그냥 값 출력.
			// 이미 레벨을 찾았으므로 넘기는 레벨에 큰 의미가 없음
			if (NOT_USE_EFFECT_TEXT_LEVEL == itLevel.level ||
				InLevel == itLevel.level)
			{
				for (const FGsSchemaEffectTextData& data : itLevel.dataList)
				{
					if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
					{
						(*parseFunc)->GetString(&data, InLevel, InOption, OutData);
					}
				}
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringPairAbnormality(int32 InId, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	const FGsSchemaEffectTextAbnormality* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextAbnormality, FGsSchemaEffectTextAbnormality>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextAbnormalityLevel& itLevel : table->dataList)
		{
			// 레벨 정보를 사용하지 않는 항목(NOT_USE_EFFECT_TEXT_LEVEL)은 그냥 값 출력.
			// 이미 레벨을 찾았으므로 넘기는 레벨에 큰 의미가 없음
			if (NOT_USE_EFFECT_TEXT_LEVEL == itLevel.level ||
				InLevel == itLevel.level)
			{
				for (const FGsSchemaEffectTextData& data : itLevel.dataList)
				{
					if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
					{
						(*parseFunc)->GetStringPair(&data, InLevel, InOption, OutData);
					}
				}
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringCollectionAbnormality(int32 InId, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData)
{
	const FGsSchemaEffectTextAbnormality* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextAbnormality, FGsSchemaEffectTextAbnormality>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextAbnormalityLevel& itLevel : table->dataList)
		{
			// 레벨 정보를 사용하지 않는 항목(NOT_USE_EFFECT_TEXT_LEVEL)은 그냥 값 출력.
			// 이미 레벨을 찾았으므로 넘기는 레벨에 큰 의미가 없음
			if (NOT_USE_EFFECT_TEXT_LEVEL == itLevel.level ||
				InLevel == itLevel.level)
			{
				for (const FGsSchemaEffectTextData& data : itLevel.dataList)
				{
					if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
					{
						(*parseFunc)->GetStringCollection(&data, InLevel, InOption, OutData);
					}
				}
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringAbnormalityTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData)
{
	if (InData)
	{
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
		{
			return (*parseFunc)->GetStringAbnormalityTime(InData, InOption, OutData);
		}
	}

	return false;
}

bool FGsEffectTextParser::GetStringPassivity(int32 InId, int32 InOption, OUT TArray<FString>& OutData)
{
	const FGsSchemaEffectTextPassivity* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextPassivityEffect& itEffect : table->effectList)
		{
			for (const FGsSchemaEffectTextData& data : itEffect.dataList)
			{
				if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
				{
					// 패시비티는 레벨정보 없음
					(*parseFunc)->GetString(&data, NOT_USE_EFFECT_TEXT_LEVEL, InOption, OutData);
				}
			}
		}		

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringPairPassivity(int32 InId, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	const FGsSchemaEffectTextPassivity* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextPassivityEffect& itEffect : table->effectList)
		{
			for (const FGsSchemaEffectTextData& data : itEffect.dataList)
			{
				if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
				{
					// 패시비티는 레벨정보 없음
					(*parseFunc)->GetStringPair(&data, NOT_USE_EFFECT_TEXT_LEVEL, InOption, OutData);
				}
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringCollectionPassivity(int32 InId, int32 InOption, OUT FGsEffectTextCollection& OutData)
{
	const FGsSchemaEffectTextPassivity* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextPassivityEffect& itEffect : table->effectList)
		{
			for (const FGsSchemaEffectTextData& data : itEffect.dataList)
			{
				if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
				{
					// 패시비티는 레벨정보 없음
					(*parseFunc)->GetStringCollection(&data, NOT_USE_EFFECT_TEXT_LEVEL, InOption, OutData);
				}
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringPassivityCondition(const FGsSchemaEffectTextData* InData, OUT FString& OutData)
{
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
	{
		return (*parseFunc)->GetStringPassivityCondition(InData, OutData);
	}

	return false;
}

void FGsEffectTextParser::GetStringPassivityCauseAndRate(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData)
{
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
	{
		(*parseFunc)->GetStringPassivityCauseAndRate(InData, InOption, OutData);
	}
}

void FGsEffectTextParser::GetStringPassivityTarget(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData)
{
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
	{
		(*parseFunc)->GetStringPassivityTarget(InData, InOption, OutData);
	}
}

void FGsEffectTextParser::GetStringPassivityCoolTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData)
{
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(InData->parseType))
	{
		(*parseFunc)->GetStringPassivityCoolTime(InData, InOption, OutData);
	}
}

bool FGsEffectTextParser::GetStringSkillNotify(int32 InId, int InLevel, int32 InOption, OUT TArray<FString>& OutData, bool bIsSkill)
{
	const FGsSchemaEffectTextSkillNotify* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextSkillNotify, FGsSchemaEffectTextSkillNotify>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextData& data : table->dataList)
		{
			// 스킬은 여러 노티의 통합 데미지 정보를 사용하므로 데미지 노티는 패스
			//if (bIsSkill)
			//{
			//	if (EGsEffectTextParseType::SKILL_NOTIFY_DAMAGE == data.parseType)
			//	{
			//		continue;
			//	}
			//}

			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
			{
				(*parseFunc)->GetString(&data, InLevel, InOption, OutData);
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringPairSkillNotify(int32 InId, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData, bool bIsSkill)
{
	const FGsSchemaEffectTextSkillNotify* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextSkillNotify, FGsSchemaEffectTextSkillNotify>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextData& data : table->dataList)
		{
			// 스킬은 여러 노티의 통합 데미지 정보를 사용하므로 데미지 노티는 패스
			//if (bIsSkill)
			//{
			//	if (EGsEffectTextParseType::SKILL_NOTIFY_DAMAGE == data.parseType)
			//	{
			//		continue;
			//	}
			//}

			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
			{
				(*parseFunc)->GetStringPair(&data, InLevel, InOption, OutData);
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::GetStringCollectionSkillNotify(int32 InId, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData, bool bIsSkill)
{
	const FGsSchemaEffectTextSkillNotify* table =
		UGsTableUtil::FindRowById<UGsTableEffectTextSkillNotify, FGsSchemaEffectTextSkillNotify>(InId);
	if (table)
	{
		for (const FGsSchemaEffectTextData& data : table->dataList)
		{
			// 스킬은 여러 노티의 통합 데미지 정보를 사용하므로 데미지 노티는 패스
			//if (bIsSkill)
			//{
			//	if (EGsEffectTextParseType::SKILL_NOTIFY_DAMAGE == data.parseType)
			//	{
			//		continue;
			//	}
			//}

			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(data.parseType))
			{
				(*parseFunc)->GetStringCollection(&data, InLevel, InOption, OutData);
			}
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParser::SetAbnormalityData(const FGsSchemaAbnormalitySet* InData, OUT FGsSchemaEffectTextAbnormality& OutData)
{
	OutData.abnormalityId = InData->id;

#ifdef USE_CUSTOM_TEXT
	// effectText 텍스트가 존재할 경우, 무조건 해당 텍스트를 찍는다
	const FGsSchemaAbnormalityEffectTextSet* effectTextSet =
		UGsTableUtil::FindRowById<UGsTableAbnormalityEffectTextSet, FGsSchemaAbnormalityEffectTextSet>(InData->id);
	if (effectTextSet)
	{
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::ABNORMALITY_DEFAULT))
		{
			return (*parseFunc)->SetData(InData, nullptr, OutData.dataList);
		}
	}
#endif

	// effectText가 없으면 타입에 맞춰 파싱한다
	bool bResult = false;
	for (int32 effectId : InData->effectIdList)
	{
		EGsEffectTextParseType targetType = EGsEffectTextParseType::ABNORMALITY_DEFAULT;
		if (const FGsSchemaAbnormalityIndexSet* indexTable =
			UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet, FGsSchemaAbnormalityIndexSet>(effectId))
		{
			// 저장하지 않을 항목 스킵. 에러는 아니므로 true를 넣음
			if (IsIgnoreAbnormalityType(indexTable->effectType))
			{
				bResult = true;
				continue;
			}

			EGsAbnormalityEffectMainType mainType = UGsGameObjectUtil::GetAbnormalityMainType(indexTable->effectType);
			switch (mainType)
			{
			case EGsAbnormalityEffectMainType::OVERTIME:
				targetType = EGsEffectTextParseType::ABNORMALITY_OVERTIME;
				break;
			case EGsAbnormalityEffectMainType::CC:
			case EGsAbnormalityEffectMainType::UTIL:
				targetType = EGsEffectTextParseType::ABNORMALITY_DURATION_ONLY;
				break;
			case EGsAbnormalityEffectMainType::STAT:
				targetType = EGsEffectTextParseType::ABNORMALITY_STAT;
				break;
			default:
				targetType = EGsEffectTextParseType::ABNORMALITY_DEFAULT;
				break;
			}

			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(targetType))
			{
				bResult = (*parseFunc)->SetData(InData, indexTable, OutData.dataList);
				if (false == bResult)
				{
					return false;
				}
			}
		}
	}

	return bResult;
}

bool FGsEffectTextParser::SetPassivityData(const FGsSchemaPassivitySet* InData, OUT FGsSchemaEffectTextPassivity& OutData)
{
	OutData.passivityId = InData->id;
	
	for (int32 i = 0; i < InData->effectInfoList.Num(); ++i)
	{
		FGsSchemaEffectTextPassivityEffect effectData;
		effectData.effectId = InData->effectInfoList[i].effectId;

		if (false == SetPassivityEffectData(InData, i, effectData))
		{
			return false;
		}

		OutData.effectList.Emplace(effectData);
	}

	return true;
}

bool FGsEffectTextParser::SetPassivityEffectData(const FGsSchemaPassivitySet* InData, int32 InIndex, 
	OUT FGsSchemaEffectTextPassivityEffect& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->effectText.IsEmpty())
	{
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::PASSIVITY_DEFAULT))
		{
			return (*parseFunc)->SetData(InData, InIndex, OutData.dataList);
		}

		return false;
	}
#endif

	EGsEffectTextParseType targetType = EGsEffectTextParseType::PASSIVITY_DEFAULT;
	switch (InData->type)
	{
	case PassivityType::STAT:
		targetType = EGsEffectTextParseType::PASSIVITY_STAT;
		break;
	case PassivityType::INVOKE_ABNORMALITY:
		targetType = EGsEffectTextParseType::PASSIVITY_INVOKE_ABNORMALITY;
		break;
	case PassivityType::INVOKE_SKILL_NOTIFY:
		targetType = EGsEffectTextParseType::PASSIVITY_INVOKE_SKILL_NOTIFY;
		break;
	case PassivityType::SKILL_LEVEL:
		targetType = EGsEffectTextParseType::PASSIVITY_SKILL_UP;
		break;
	default:
		targetType = EGsEffectTextParseType::PASSIVITY_DEFAULT;
		break;
	}

	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(targetType))
	{
		return (*parseFunc)->SetData(InData, InIndex, OutData.dataList);
	}

	return false;
}

bool FGsEffectTextParser::SetSkillData(const FGsSchemaSkillSet* InData, OUT FGsSchemaEffectTextSkill& OutData)
{
	OutData.skillId = InData->id;

	// Passive 스킬의 경우, LevelSet 정보가 존재하지 않음
	if (SkillCategory::PASSIVE == InData->category)
	{
		FGsSchemaEffectTextSkillLevel levelData;
		levelData.level = NOT_USE_EFFECT_TEXT_LEVEL;

		FGsSchemaEffectTextData effectTextData;
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_PASSIVITY_LIST))
		{
			(*parseFunc)->SetData(InData, effectTextData);
		}

		levelData.dataList.Emplace(effectTextData);
		OutData.levelList.Emplace(levelData);
	}
	else
	{
		const FGsSchemaSkillLevelSet* tableLevelSet =
			UGsTableUtil::FindRowById<UGsTableSkillLevelSet, FGsSchemaSkillLevelSet>(InData->id);
		if (tableLevelSet)
		{
			for (const FGsSchemaSkillLevelInfo& itLevel : tableLevelSet->levelInfoList)
			{
				FGsSchemaEffectTextSkillLevel levelData;

				if (false == SetSkillLevelData(&itLevel, levelData))
				{
					return false;
				}				

				OutData.levelList.Emplace(levelData);
			}
		}
	}

	// NPC나 평타 등은 레벨셋이 없을 수도 있으므로 그냥 true로 처리 함
	return true;
}

bool FGsEffectTextParser::SetSkillLevelData(const FGsSchemaSkillLevelInfo* InData, 
	OUT FGsSchemaEffectTextSkillLevel& OutData)
{
	OutData.level = InData->level;

#ifdef USE_CUSTOM_TEXT
	if (false == InData->effectText.IsEmpty())
	{
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_LEVEL_DEFAULT))
		{
			return (*parseFunc)->SetData(InData, OutData.dataList);
		}

		return false;
	}
#endif
	
	//if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_LEVEL_DAMAGE))
	//{
	//	(*parseFunc)->SetData(InData, OutData.dataList);
	//}

	// Damage도 Abnormality 표시 때문에 노티를 수집함. 출력 시에 스킬쪽 코드이면 노티파이 데미지는 표시 안함
	if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_LEVEL_NOTIFY_LIST))
	{
		(*parseFunc)->SetData(InData, OutData.dataList);
	}

	return true;
}

bool FGsEffectTextParser::SetSkillNotifyData(const FGsSchemaSkillNotifySet* InData, 
	OUT FGsSchemaEffectTextSkillNotify& OutData)
{
	OutData.skillNotifyId = InData->id;

#ifdef USE_CUSTOM_TEXT
	if (false == InData->effectText.IsEmpty())
	{
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_NOTIFY_DEFAULT))
		{
			return (*parseFunc)->SetData(InData, OutData.dataList);
		}

		return false;
	}
#endif

	if (0 == InData->notifyElementList.Num())
	{
		return false;
	}

	for (int i = 0; i < InData->notifyElementList.Num(); ++i)
	{
		// 없으면 안되는 정보이므로 바로 false 리턴
		const FGsSchemaSkillNotifyElement* element = InData->notifyElementList[i].GetRow();
		if (nullptr == element)
		{
			return false;
		}

		bool bIsValid = false;
		TArray<FGsSchemaEffectTextData> tempList;

		switch (InData->type)
		{
		case SkillNotifyType::DAMAGE:
		{
			//bool bReturnDamage = false;
			//// 특수. 이 타입일때 abnormality도 검사해야 함
			//if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_NOTIFY_DAMAGE))
			//{
			//	bReturnDamage = (*parseFunc)->SetData(InData, OutData.dataList);
			//}

			bool bReturnAbnormality = false;
			// 특수. 이 타입일때 abnormality도 검사해야 함
			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_NOTIFY_ABNORMALITY))
			{
				// 같은 정보가 버그는 아니고 중복저장만 안하는 것이므로 true를 저장
				if ((*parseFunc)->IsExistSameDate(element, OutData.dataList))
				{
					bReturnAbnormality = true;
				}
				else
				{
					bReturnAbnormality = (*parseFunc)->SetSkillNotifyElementData(element, tempList);
				}
			}

			bool bReturnAbsorption = false;
			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_NOTIFY_ABSORPTION))
			{
				// 같은 정보가 버그는 아니고 중복저장만 안하는 것이므로 true를 저장
				if ((*parseFunc)->IsExistSameDate(element, OutData.dataList))
				{
					bReturnAbnormality = true;
				}
				else
				{
					bReturnAbsorption = (*parseFunc)->SetSkillNotifyElementData(element, tempList);
				}
			}

			bIsValid = /*bReturnDamage ||*/ bReturnAbnormality || bReturnAbsorption;
		}
		break;
		case SkillNotifyType::VARY_HP_MP:
		{
			// 수집 안함. 에러는 아니므로 true 리턴
			return true;
			//if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_NOTIFY_HEAL))
			//{
			//	bIsValid = (*parseFunc)->SetData(InData, OutData.dataList);
			//}
		}
		break;
		case SkillNotifyType::FIRE_PROJECTILE:
		{
			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_NOTIFY_PROJECTILE))
			{
				// 같은 정보가 버그는 아니고 중복저장만 안하는 것이므로 true를 저장
				if ((*parseFunc)->IsExistSameDate(element, OutData.dataList))
				{
					bIsValid = true;
				}
				else
				{
					bIsValid = (*parseFunc)->SetSkillNotifyElementData(element, tempList);
				}
			}
		}
		break;
		default:
		{
			if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_NOTIFY_DEFAULT))
			{
				bIsValid = (*parseFunc)->SetSkillNotifyElementData(element, tempList);
			}
		}
		break;
		}

		if (false == bIsValid)
		{
			return false;
		}

		if (0 < tempList.Num())
		{
			// 조건을 먼저 저장한다
			SetSkillNotifyExeConditionData(element->executeConditionId, OutData.dataList);

			// 그 뒤에 수집 정보를 저장함
			OutData.dataList.Append(tempList);
		}
	}

	return true;
}

bool FGsEffectTextParser::SetSkillNotifyExeConditionData(int32 InExecuteCondtionId,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	// 0일 수 있음. 무시하기 위해 true 리턴
	if (0 == InExecuteCondtionId)
	{
		return true;
	}

	const FGsSchemaSkillExecuteConditionSet* condition =
		UGsTableUtil::FindRowById<UGsTableSkillExecuteConditionSet, FGsSchemaSkillExecuteConditionSet>(InExecuteCondtionId);
	if (nullptr == condition)
	{
		return false;
	}

	switch (condition->conditionMethod)
	{
	case SkillExecuteConditionMethod::ABNORMALITY:
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_EXE_CONDITION_ABNORMALITY))
		{
			return (*parseFunc)->SetSkillConditionData(condition, OutData);
		}
		break;
	case SkillExecuteConditionMethod::STAT:
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_EXE_CONDITION_STAT))
		{
			return (*parseFunc)->SetSkillConditionData(condition, OutData);
		}
		break;
	case SkillExecuteConditionMethod::PASSIVITY:
		if (FGsEffectTextParseBase** parseFunc = _parseMap.Find(EGsEffectTextParseType::SKILL_EXE_CONDITION_PASSIVITY))
		{
			return (*parseFunc)->SetSkillConditionData(condition, OutData);
		}
		break;
	}

	return false;
}

bool FGsEffectTextParser::IsIgnoreAbnormalityType(AbnormalityEffectType InType) const
{
	// 아래 항목은 내용 출력하지 않음
	switch (InType)
	{
	case AbnormalityEffectType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY:
	case AbnormalityEffectType::UTIL_NOT_AI_CHANGE_PHASE:
	case AbnormalityEffectType::UTIL_BUFF_COUNTER:
	case AbnormalityEffectType::UTIL_BURY_KEEP_TARGET:
	case AbnormalityEffectType::UTIL_DAMAGE_SHARE:
	case AbnormalityEffectType::UTIL_CREATE_DEPON_NPC:
	case AbnormalityEffectType::UTIL_HP_ABSORPTION:
	case AbnormalityEffectType::UTIL_MOUNT_VEHICLE:
		return true;
	}

	return false;
}


#if WITH_EDITOR
void FGsEffectTextParser::TestGatherSkill(int32 InSkillId, int32 InSkillLevel)
{
	const FGsSchemaSkillSet* skillRow = 
		UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(InSkillId);

	FGsSchemaEffectTextSkill outSkill;
	outSkill.RowName = skillRow->RowName;
	if (false == SetSkillData(skillRow, outSkill))
	{
		UE_LOG(LogTemp, Error, TEXT("[Converter] Failed SetData: SkillId: %d"), skillRow->id);
	}

	for (FGsSchemaEffectTextSkillLevel& levelData : outSkill.levelList)
	{
		if (InSkillLevel == levelData.level)
		{
			for (FGsSchemaEffectTextData& effectData : levelData.dataList)
			{
				if (EGsEffectTextParseType::SKILL_LEVEL_NOTIFY_LIST == effectData.parseType)
				{
					for (int32 i = 0; i < effectData.intParams.Num(); ++i)
					{
						const FGsSchemaSkillNotifySet* skillNotifyRow =
							UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(effectData.intParams[i]);

						FGsSchemaEffectTextSkillNotify outNotify;
						if (false == SetSkillNotifyData(skillNotifyRow, outNotify))
						{
							UE_LOG(LogTemp, Error, TEXT("[Converter] Failed SetData: SkillNotifyId: %d"), skillNotifyRow->id);
						}
					}
				}
			}
		}
	}
}
#endif
