// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextManager.h"
#include "UTIL/GsTableUtil.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormality.h"
#include "DataSchema/EffectText/GsSchemaEffectTextPassivity.h"
#include "DataSchema/EffectText/GsSchemaEffectTextSkill.h"
#include "DataSchema/EffectText/GsSchemaEffectTextPassivityEffect.h"


FGsEffectTextManager::~FGsEffectTextManager()
{
	Finalize();
}

void FGsEffectTextManager::Initialize()
{
	_parser = new FGsEffectTextParser();
	_parser->Initialize();
}

void FGsEffectTextManager::Finalize()
{
	delete _parser;
	_parser = nullptr;
}

bool FGsEffectTextManager::GetEffectTextAbnormality(AbnormalityId InId, OUT FString& OutData, 
	int InLevel, int32 InOption)
{
	TArray<FGsSchemaEffectTextData> dataList;
	GetEffectTextAbnormalityRow(InId, dataList, InLevel);
	GetEffectTextInter(dataList, InLevel, InOption, OutData);

	if (OutData.IsEmpty())
	{
		return false;
	}

	return true;
}

bool FGsEffectTextManager::GetEffectTextListAbnormality(AbnormalityId InId, OUT TArray<TPair<FText, FText>>& OutData, 
	int InLevel, int32 InOption)
{
	TArray<FGsSchemaEffectTextData> dataList;
	GetEffectTextAbnormalityRow(InId, dataList, InLevel);

	for (const FGsSchemaEffectTextData& iter : dataList)
	{
		_parser->GetStringPair(&iter, InLevel, InOption, OutData);
	}

	if (0 < OutData.Num())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextManager::GetStringAbnormalityTime(AbnormalityId InId, OUT FString& OutData, 
	int InLevel, int32 InOption)
{
	TArray<FGsSchemaEffectTextData> dataList;
	GetEffectTextAbnormalityRow(InId, dataList, InLevel);

	for (const FGsSchemaEffectTextData& iter : dataList)
	{
		// B1 : 현재 Stat 형에서만 얻어올 수 있음
		// 누적하는게 아니라 얻어오면 바로 리턴함에 유의
		if (_parser->GetStringAbnormalityTime(&iter, InOption, OutData))
		{
			return true;
		}
	}

	return false;
}

void FGsEffectTextManager::GetEffectTextAbnormalityRow(AbnormalityId InId, OUT TArray<FGsSchemaEffectTextData>& OutList, int InLevel)
{
	if (const FGsSchemaEffectTextAbnormality* tableRow =
		UGsTableUtil::FindRowById<UGsTableEffectTextAbnormality, FGsSchemaEffectTextAbnormality>(InId))
	{
		for (const FGsSchemaEffectTextAbnormalityLevel& itLevel : tableRow->dataList)
		{
			// 레벨 정보를 사용하지 않는 항목은 기본값 사용(넘기는 레벨에 큰 의미 없음)
			if (NOT_USE_EFFECT_TEXT_LEVEL == itLevel.level)
			{
				OutList.Append(itLevel.dataList);
			}
			else if (InLevel == itLevel.level)
			{
				OutList.Append(itLevel.dataList);
			}
		}
	}
}

bool FGsEffectTextManager::GetEffectTextPassivity(PassivityId InId, OUT FString& OutData, int32 InOption)
{
	if (const FGsSchemaEffectTextPassivity* tableRow =
		UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InId))
	{
		TArray<FString> strArray;
		for (const FGsSchemaEffectTextPassivityEffect& itEffect : tableRow->effectList)
		{
			FString strOut;
			GetEffectTextInter(itEffect.dataList, DEFAULT_EFFECT_TEXT_LEVEL, InOption, strOut);
			if (false == strOut.IsEmpty())
			{
				strArray.Emplace(strOut);
			}
		}

		FGsEffectTextParser::GetMergedString(strArray, InOption, OutData);		

		if (false == OutData.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

bool FGsEffectTextManager::GetEffectTextListPassivity(PassivityId InId, OUT TArray<TPair<FText, FText>>& OutData, int32 InOption)
{
	if (const FGsSchemaEffectTextPassivity* tableRow =
		UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InId))
	{
		for (const FGsSchemaEffectTextPassivityEffect& itEffect : tableRow->effectList)
		{
			for (const FGsSchemaEffectTextData& iter : itEffect.dataList)
			{
				_parser->GetStringPair(&iter, DEFAULT_EFFECT_TEXT_LEVEL, InOption, OutData);
			}

			if (0 < OutData.Num())
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsEffectTextManager::GetEffectTextColectionPassivity(PassivityId InId, OUT FGsEffectTextCollection& OutData, int32 InOption)
{
	if (const FGsSchemaEffectTextPassivity* tableRow =
		UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InId))
	{
		for (const FGsSchemaEffectTextPassivityEffect& itEffect : tableRow->effectList)
		{
			for (const FGsSchemaEffectTextData& iter : itEffect.dataList)
			{
				_parser->GetStringCollection(&iter, DEFAULT_EFFECT_TEXT_LEVEL, InOption, OutData);
			}
		}
	}

	return true;
}

bool FGsEffectTextManager::GetEffectTextFromPassivityList(const TArray<PassivityId>& InIdList, OUT FGsEffectTextCollection& OutData, int32 InOption /*= 0*/)
{
	// 실제 parser 에 넣을 data 의 List
	TArray<FGsSchemaEffectTextData> dataList;

	// 테이블 순회하며 dataList 생성
	for (const PassivityId InId : InIdList)
	{
		if (const FGsSchemaEffectTextPassivity* tableRow =
			UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InId))
		{
			for (const FGsSchemaEffectTextPassivityEffect& itEffect : tableRow->effectList)
			{
				for (const FGsSchemaEffectTextData& iter : itEffect.dataList)
				{
					dataList.Emplace(iter);
				}
			}
		}
	}

	for (const FGsSchemaEffectTextData& iter : dataList)
	{
		_parser->GetStringCollection(&iter, DEFAULT_EFFECT_TEXT_LEVEL, InOption, OutData);
	}

	return true;
}

bool FGsEffectTextManager::GetStringPassivityEffect(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString& OutData, int32 InOption)
{
	GetEffectTextInter(InPassivityData->dataList, DEFAULT_EFFECT_TEXT_LEVEL, InOption, OutData);
	if (false == OutData.IsEmpty())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextManager::GetStringPassivityCondition(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString& OutData)
{
	for (const FGsSchemaEffectTextData& iter : InPassivityData->dataList)
	{
		if (_parser->GetStringPassivityCondition(&iter, OutData))
		{
			return true;
		}
	}

	return false;
}

bool FGsEffectTextManager::GetStringPassivityCauseAndRate(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString& OutData, int32 InOption)
{
	for (const FGsSchemaEffectTextData& iter : InPassivityData->dataList)
	{
		_parser->GetStringPassivityCauseAndRate(&iter, InOption, OutData);
		if (false == OutData.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

bool FGsEffectTextManager::GetStringPassivityTarget(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString& OutData, int32 InOption)
{
	for (const FGsSchemaEffectTextData& iter : InPassivityData->dataList)
	{
		_parser->GetStringPassivityTarget(&iter, InOption, OutData);
		if (false == OutData.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

bool FGsEffectTextManager::GetStringPassivityCoolTime(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString& OutData, int32 InOption)
{
	for (const FGsSchemaEffectTextData& iter : InPassivityData->dataList)
	{
		_parser->GetStringPassivityCoolTime(&iter, InOption, OutData);
		if (false == OutData.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

bool FGsEffectTextManager::GetEffectTextSkill(SkillId InId, SkillLevel InLevel, 
	OUT FString& OutData, int32 InOption)
{
	if (const FGsSchemaEffectTextSkill* tableRow =
		UGsTableUtil::FindRowById<UGsTableEffectTextSkill, FGsSchemaEffectTextSkill>(InId))
	{
		// 내용
		for (const FGsSchemaEffectTextSkillLevel& levelData : tableRow->levelList)
		{
			if (NOT_USE_EFFECT_TEXT_LEVEL == levelData.level ||
				InLevel == levelData.level)
			{
				GetEffectTextInter(levelData.dataList, InLevel, InOption, OutData);
				break;
			}
		}

		if (false == OutData.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

bool FGsEffectTextManager::GetEffectTextListSkill(SkillId InId, SkillLevel InLevel, 
	OUT TArray<TPair<FText, FText>>& OutData, int32 InOption)
{
	if (const FGsSchemaEffectTextSkill* tableRow =
		UGsTableUtil::FindRowById<UGsTableEffectTextSkill, FGsSchemaEffectTextSkill>(InId))
	{
		// 내용
		for (const FGsSchemaEffectTextSkillLevel& levelData : tableRow->levelList)
		{
			if (NOT_USE_EFFECT_TEXT_LEVEL == levelData.level ||
				InLevel == levelData.level)
			{
				for (const FGsSchemaEffectTextData& iter : levelData.dataList)
				{
					_parser->GetStringPair(&iter, InLevel, InOption, OutData);
				}
				break;
			}
		}
	}

	return true;
}

void FGsEffectTextManager::GetEffectTextInter(const TArray<FGsSchemaEffectTextData>& InList, 
	int InLevel, int32 InOption, OUT FString& OutData)
{
	for (const FGsSchemaEffectTextData& iter : InList)
	{
		TArray<FString> strArray;
		if (_parser->GetString(&iter, InLevel, InOption, strArray))
		{
			FGsEffectTextParser::GetMergedString(strArray, InOption, OutData);
		}
	}
}

#if WITH_EDITOR
bool FGsEffectTextManager::TestGetEffectTextSkillNotify(int32 InSkillNotifyId, OUT FString& OutData, int InLevel, int32 InOption)
{
	TArray<FString> strArray;
	_parser->GetStringSkillNotify(InSkillNotifyId, InLevel, InOption, strArray);

	FGsEffectTextParser::GetMergedString(strArray, InOption, OutData);

	if (OutData.IsEmpty())
	{
		return false;
	}

	return true;
}

void FGsEffectTextManager::TestGatherSkill(SkillId InId, SkillLevel InLevel)
{
	_parser->TestGatherSkill(InId, InLevel);
}
#endif
