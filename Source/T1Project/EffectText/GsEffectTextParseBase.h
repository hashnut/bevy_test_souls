// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextDefine.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"

// 텍스트 사용 여부
#define USE_CUSTOM_TEXT

struct FGsSchemaAbnormalitySet;
struct FGsSchemaPassivitySet;
struct FGsSchemaSkillLevelInfo;
struct FGsSchemaSkillNotifySet;
struct FGsSchemaSkillSet;
struct FGsSchemaSkillNotifyElement;
struct FGsSchemaSkillExecuteConditionSet;

/**
 * 효과 텍스트 기본 클래스
 */
class T1PROJECT_API FGsEffectTextParseBase
{
protected:
	class FGsEffectTextParser* _parser;

public: 
	explicit FGsEffectTextParseBase(class FGsEffectTextParser* InParser)
		: _parser(InParser) 
	{
	}

	virtual ~FGsEffectTextParseBase() {}

public:
	// 필수 구현
	virtual EGsEffectTextParseType GetParseType() const = 0;
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const = 0;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) = 0;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) = 0;

	// 아래와 같은 처리 아닌 경우만 오버라이딩 해서 구현
	virtual bool GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData)
	{
		return GetStringPair(InData, InLevel, InOption, OutData._effectList);
	}

public:
	// Abnoramlity 중 필요한 곳에서만 상속 구현
	virtual bool GetStringAbnormalityTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) { return false; }

public:
	// Passivity 중 필요한 곳에서만 상속 구현
	virtual bool GetStringPassivityCondition(const FGsSchemaEffectTextData* InData, OUT FString& OutData) { return false; }
	virtual void GetStringPassivityCauseAndRate(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) {}
	virtual void GetStringPassivityTarget(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) {}
	virtual void GetStringPassivityCoolTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) {}

public:
	virtual bool SetData(const FGsSchemaAbnormalitySet* InData, const struct FGsSchemaAbnormalityIndexSet* InIndexSet,
		OUT TArray<struct FGsSchemaEffectTextAbnormalityLevel>& OutData) { return false; }
		
	virtual bool SetData(const FGsSchemaPassivitySet* InData, int32 InIndex, OUT TArray<FGsSchemaEffectTextData>& OutData) { return false; }
	virtual bool SetData(const FGsSchemaPassivitySet* InData, OUT TMap<int32, TArray<FGsSchemaEffectTextData>>& OutData) { return false; }	

	virtual bool SetData(const FGsSchemaSkillLevelInfo* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) { return false; }
	virtual bool SetData(const FGsSchemaSkillNotifySet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) { return false; }

	// 패시브 스킬일 경우, 레벨정보가 없으므로 사용
	virtual bool SetData(const FGsSchemaSkillSet* InData, OUT FGsSchemaEffectTextData& OutData) { return false; }

	// 리턴값. -1: 에러, 0: 없음, 1: 있음 
	virtual int IsExistSameDate(const FGsSchemaSkillNotifyElement* InData, TArray<FGsSchemaEffectTextData>& InPrevData) { return 0; }
	virtual bool SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) { return false; }
	// 스킬의 조건 정보 세팅
	virtual bool SetSkillConditionData(const FGsSchemaSkillExecuteConditionSet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) { return false; }
};
