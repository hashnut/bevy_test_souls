// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityCauseInfo.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaPassivitySet;
struct FGsSchemaPassivityEffectInfo;
struct FGsSchemaRowPoly;

/**
 * 패시비티 쪽은 조건 정보가 중복이라 부모 클래스를 하나 만들었다
 * FGsSchemaPassivityEffectConditionInfo 테이블 참고
 * - 예: HP 10% 이하일 때,
 * - int[0] : Condition유무 (0: 없음, 1: 있음)
 * 
 * (Condition이 있을때만 아래정보 입력)
 * - int[1] : PassivityConditionType
 * - int[2] : PassivityConditionMethod
 * - int[3] : param1
 * - int[4] : param2
 */
class T1PROJECT_API FGsEffectTextParsePassivityBase : public FGsEffectTextParseBase
{
public:
	/**
	* InvokeAbnormality, InvokeSkill쪽에서 사용되는 타입
	* 
	* Condition이 있으면 n = 5, 없으면 n = 1
	* - int[n] : abnormalityId/skillNotifyId
	* - int[n+1] : PassivityCauseType (발동 조건 타입)
	* - int[n+2] : ratePercent (발동 퍼센트)
	* - int[n+3] : PassivityInvokeTargetType (대상)
	*
	* 이하 Cause 타입에 따라 추가되는 정보
	* - START_SKILL_TYPE
	*   - int[n+4] : userSkillSetId
	* - EQUIP_ITEM_SELF
	*   - int[n+4] : itemId
	*   - int[n+5] : ItemGrade
	*   - int[n+6] : ItemCategorySub
	* - UPSERT_ABNORMALITY_TARGET
	*   - int[n+4] : abnormalityId
	*/	
	struct FGsInvokeTypeData
	{
	public:
		const FGsSchemaEffectTextData* _data = nullptr;

	public:
		explicit FGsInvokeTypeData(const FGsSchemaEffectTextData* InData)
			: _data(InData)
		{
		}

	public:
		bool IsCondition() const;
		int32 GetStartIndex() const;

		int32 GetId() const;
		PassivityCauseType GetCause() const;
		int32 GetRatePercent() const;
		PassivityInvokeTargetType GetTarget() const;

		// Cause 추가정보 관련
		int32 GetCauseUserSkillSetId() const;
		int32 GetCauseItemId() const;
		ItemGrade GetCauseItemGrade() const;
		ItemCategorySub GetCauseItemCategorySub() const;
		int32 GetCauseAbnormalityId() const;
	};

public:
	explicit FGsEffectTextParsePassivityBase(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}

	virtual ~FGsEffectTextParsePassivityBase() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::NONE; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override { return false; }

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override { return false; }
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override { return false; }

	virtual bool GetStringPassivityCondition(const FGsSchemaEffectTextData* InData, OUT FString& OutData) override;
	// 패시비티 중 일부(Invoke)에만 있는 기능
	//virtual void GetStringPassivityCauseAndRate(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) { return false; }
	//virtual void GetStringPassivityTarget(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) { return false; }
	//virtual void GetStringPassivityCoolTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) { return false; }

protected:
	void SetConditionData(const FGsSchemaPassivityEffectInfo& InData, OUT FGsSchemaEffectTextData& OutData);
	void SetAdditionalCauseParam(PassivityCauseType InType, const FGsSchemaPassivityCauseInfo& InCauseInfo,
		OUT FGsSchemaEffectTextData& OutData);
	
	bool GetStringCondition(const FGsSchemaEffectTextData* InData, OUT FString& OutString);
	void AddStringConditionMethod(PassivityConditionMethod InType, int32 InParam1, int32 InParam2, OUT FString& OutString);
	bool IsCondition(const FGsSchemaPassivityEffectInfo& InData) const;
	bool IsCondition(const FGsSchemaEffectTextData* InData) const;

	// 조건 및 확률 찍는 부분에 찍을 내용
	void AddStringCause(const FGsInvokeTypeData& InInvokeData, int32 InOption, OUT FString& OutSring);
	void AddStringRatePercent(int32 InRatePercent, int32 InOption, OUT FString& OutSring);
		
	// 대상에 찍을 내용	
	void AddStringTarget(PassivityInvokeTargetType InTarget, int32 InOption, OUT FString& OutSring);

	void GetTextPassivityCause(const FGsInvokeTypeData& InInvokeData, OUT FText& OutText);
	void GetTextItemCategory(ItemCategorySub InType, OUT FText& OutText);

	void GetTextWeaponType(CreatureWeaponType InType, OUT FText& OutText);
	
	void GetAbnormalityIngameName(int32 InAbnormalityId, OUT FText& OutText);
};
