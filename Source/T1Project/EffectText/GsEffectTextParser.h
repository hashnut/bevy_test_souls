// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextDefine.h"
#include "EffectText/GsEffectTextParseBase.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaEffectTextAbnormality;
struct FGsSchemaEffectTextPassivity;
struct FGsSchemaEffectTextPassivityEffect;
struct FGsSchemaEffectTextSkill;
struct FGsSchemaEffectTextSkillLevel;
struct FGsSchemaEffectTextSkillNotify;

struct FGsSchemaAbnormalitySet;
struct FGsSchemaPassivitySet;
struct FGsSchemaSkillSet;
struct FGsSchemaSkillLevelInfo;
struct FGsSchemaSkillNotifySet;

/**
 * 스킬 효과텍스트를 툴타임에 수집, 수집한 정보를 이용해 텍스트를 생성
 */
class T1PROJECT_API FGsEffectTextParser final
{
protected:
	TMap<EGsEffectTextParseType, FGsEffectTextParseBase*> _parseMap;
	
public:
	FGsEffectTextParser() = default;
	~FGsEffectTextParser();

public:
	static void GetMergedString(TArray<FString>& InList, int32 InOption, OUT FString& OutData);
	static void GetMergedStringByPair(TArray<TPair<FText, FText>>& InList, int32 InOption, OUT FString& OutData);

public:
	void Initialize();
	void Finalize();

	bool IsEmptyData(const FGsSchemaEffectTextData* InData) const;

	bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData);
	bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData);
	bool GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData);

	// Abnormality
public:
	bool GetStringAbnormality(int32 InId, int InLevel, int32 InOption, OUT TArray<FString>& OutData);
	bool GetStringPairAbnormality(int32 InId, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData);
	bool GetStringCollectionAbnormality(int32 InId, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData);
	
	bool GetStringAbnormalityTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData);

	// Passivity
public:
	bool GetStringPassivity(int32 InId, int32 InOption, OUT TArray<FString>& OutData);
	bool GetStringPairPassivity(int32 InId, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData);
	bool GetStringCollectionPassivity(int32 InId, int32 InOption, OUT FGsEffectTextCollection& OutData);

	bool GetStringPassivityCondition(const FGsSchemaEffectTextData* InData, OUT FString& OutData);
	void GetStringPassivityCauseAndRate(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData);
	void GetStringPassivityTarget(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData);
	void GetStringPassivityCoolTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData);

	// Skill
public:
	bool GetStringSkillNotify(int32 InId, int InLevel, int32 InOption, OUT TArray<FString>& OutData, bool bIsSkill = false);
	bool GetStringPairSkillNotify(int32 InId, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData, bool bIsSkill = false);
	bool GetStringCollectionSkillNotify(int32 InId, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData, bool bIsSkill = false);

	// Set (DataTableConverter 에서만 사용)
public:
	bool SetAbnormalityData(const FGsSchemaAbnormalitySet* InData, OUT FGsSchemaEffectTextAbnormality& OutData);
	bool SetPassivityData(const FGsSchemaPassivitySet* InData, OUT FGsSchemaEffectTextPassivity& OutData);
	bool SetPassivityEffectData(const FGsSchemaPassivitySet* InData, int32 InIndex, OUT FGsSchemaEffectTextPassivityEffect& OutData);
	bool SetSkillData(const FGsSchemaSkillSet* InData, OUT FGsSchemaEffectTextSkill& OutData);	
	bool SetSkillLevelData(const FGsSchemaSkillLevelInfo* InData, OUT FGsSchemaEffectTextSkillLevel& OutData);
	bool SetSkillNotifyData(const FGsSchemaSkillNotifySet* InData, OUT FGsSchemaEffectTextSkillNotify& OutData);
	bool SetSkillNotifyExeConditionData(int32 InExecuteCondtionId, OUT TArray<FGsSchemaEffectTextData>& OutData);
	
public:
	bool IsIgnoreAbnormalityType(AbnormalityEffectType InType) const;

#if WITH_EDITOR
public:
	// 스킬의 Set로직 디버깅을 위한 함수 
	void TestGatherSkill(int32 InSkillId, int32 InSkillLevel);
#endif
};
