// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"
#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifyElement;
struct FGsSchemaSkillNotifyHitSet;
/**
 * 노티파이의 Abnormality 정보
 * : 여러 노티파이에 동일한 효과가 있는 경우 1개만 출력(IsSameAbnormalityEffect 로 판단)
 * - int[0] : executeConditionId(필수)
 * - int[1] : AbnormalityApplyType (필수) NONE/ADD/REMOVE/REMOVE_ALL
 * - int[2] : AbnormalityId(필수) ADD, ADD_GROUP, REMOVE일 경우에만 AbnormalityId 값이 있고 나머지는 INVALID_ABNORMALITY_ID
 *
 * - text[0] : effectText (필수) // 빈 정보라도 필수 입력
 * - text[1] : REMOVE일 경우에만 Abnormality이름 
 * 
 * AbnormalityApplyType 별 처리
 * - NONE: 출력 없음
 * - ADD: AbnormalitySet 동일 규칙
 * - ADD_GROUP: AbnormalitySet 동일 규칙인데, 상태이상 이름이 "특정 상태이상"
 * - REMOVE: "{상태이상이름} 해제" 출력
 * - REMOVE_ALL: "모든 상태이상 해제" 출력
 * - REMOVE_BUFF: "모든 버프 해제" 출력
 * - REMOVE_DEBUFF: "모든 디버프 해제" 출력
 * - REMOVE_GROUP: "특정 상태이상 해제" 출력
 */
class FGsEffectTextParseSkillNotifyAbnormality final : public FGsEffectTextParseBase
{
public:
	enum EGsETSkillAbnormalityParam : uint8
	{
		ET_ExecuteConditionId = 0, // executeConditionId(필수)
		ET_ApplyType = 1, // AbnormalityApplyType(필수)
		ET_AbnormalityId = 2, // (필수)활용하지 않는 타입은 INVALID_ABNORMALITY_ID를 저장함

		ET_MaxParam,
	};

public:
	explicit FGsEffectTextParseSkillNotifyAbnormality(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}

	virtual ~FGsEffectTextParseSkillNotifyAbnormality() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_NOTIFY_ABNORMALITY; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;\
	// 주의: 엮인 Abnormality에 Stat이 있어서 누적해야 할 지도 모르므로 이 함수를 오버라이드 해서 구현해 줌
	virtual bool GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData) override;

	virtual int IsExistSameDate(const FGsSchemaSkillNotifyElement* InData, TArray<FGsSchemaEffectTextData>& InPrevData) override;
	virtual bool SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

private:
	void SetAbnormalityDataByType(AbnormalityApplyType InType, int32 InAbnormalityId,
		OUT FGsSchemaEffectTextData& OutData);
	void SetAbnormalityName(int32 InAbnormalityId, OUT FGsSchemaEffectTextData& OutData);
	bool IsSameAbnormalityEffect(int32 InExecuteConditionId, AbnormalityApplyType InType, int32 InAbnormalityId,
		TArray<FGsSchemaEffectTextData>& InData);

	bool GetTextRemove(const FText& InAbnormalityName, OUT FText& OutText);
};
