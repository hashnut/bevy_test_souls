// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "EffectText/GsEffectTextDefine.h"
#include "EffectText/GsEffectTextParser.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/GsScopeHolder.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"

struct FGsSchemaEffectTextPassivityEffect;
struct FGsSchemaEffectTextAbnormalityLevel;

/**
 * 스킬, Abnormality, Passivity등의 효과 텍스트 출력 관리
 */
class T1PROJECT_API FGsEffectTextManager final : public IGsManager
{
protected:
	FGsEffectTextParser* _parser = nullptr;

public:
	FGsEffectTextManager() = default;
	virtual ~FGsEffectTextManager();

	// IGsManager override
public:	
	virtual void Initialize() override;
	virtual void Finalize() override;

	// Abnormality
public:	
	bool GetEffectTextAbnormality(AbnormalityId InId, OUT FString& OutData, int InLevel = INVALID_EFFECT_TEXT_LEVEL, int32 InOption = 0);
	bool GetEffectTextListAbnormality(AbnormalityId InId, OUT TArray<TPair<FText, FText>>& OutData, int InLevel = INVALID_EFFECT_TEXT_LEVEL, int32 InOption = 0);

	bool GetStringAbnormalityTime(AbnormalityId InId, OUT FString& OutData, int InLevel = INVALID_EFFECT_TEXT_LEVEL, int32 InOption = 0);

private:
	void GetEffectTextAbnormalityRow(AbnormalityId InId, OUT TArray<FGsSchemaEffectTextData>&OutList, int InLevel = INVALID_EFFECT_TEXT_LEVEL);

	// Passivity
public:	
	bool GetEffectTextPassivity(PassivityId InId, OUT FString& OutData, int32 InOption = 0);
	bool GetEffectTextListPassivity(PassivityId InId, OUT TArray<TPair<FText, FText>>& OutData, int32 InOption = 0);
	bool GetEffectTextColectionPassivity(PassivityId InId, OUT FGsEffectTextCollection& OutData, int32 InOption = 0);
	bool GetEffectTextFromPassivityList(const TArray<PassivityId>& InIdList, OUT FGsEffectTextCollection& OutData, int32 InOption = 0);

	bool GetStringPassivityEffect(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString & OutData, int32 InOption = 0);
	bool GetStringPassivityCondition(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString & OutData);
	bool GetStringPassivityCauseAndRate(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString & OutData, int32 InOption = 0);
	bool GetStringPassivityTarget(const FGsSchemaEffectTextPassivityEffect* InPassivityData, OUT FString & OutData, int32 InOption = 0);
	bool GetStringPassivityCoolTime(const FGsSchemaEffectTextPassivityEffect * InPassivityData, OUT FString & OutData, int32 InOption = 0);

	// Skill
public:	
	bool GetEffectTextSkill(SkillId InId, SkillLevel InLevel, OUT FString& OutData, int32 InOption = 0);
	bool GetEffectTextListSkill(SkillId InId, SkillLevel InLevel, OUT TArray<TPair<FText, FText>>& OutData, int32 InOption = 0);

	// 공용
private:
	void GetEffectTextInter(const TArray<FGsSchemaEffectTextData>& InList, int InLevel, int32 InOption, OUT FString& OutData);

#if WITH_EDITOR
public:
	bool TestGetEffectTextSkillNotify(int32 InSkillNotifyId, OUT FString & OutData, int InLevel = INVALID_EFFECT_TEXT_LEVEL, int32 InOption = 0);
	void TestGatherSkill(SkillId InId, SkillLevel InLevel);
#endif
};

#define GSEffectText() UGsScopeHolder::GetGameManagerFType<FGsEffectTextManager>(UGsScopeGame::EManagerType::EffectText)
