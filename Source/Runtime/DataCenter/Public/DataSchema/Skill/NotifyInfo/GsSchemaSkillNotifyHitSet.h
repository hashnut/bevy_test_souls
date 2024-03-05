/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyHitSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifyHitSet"))
struct DATACENTER_API FGsSchemaSkillNotifyHitSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="skillPowerRate"))
	int32 skillPowerRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ignoreDefenseDamage"))
	int32 ignoreDefenseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="noDamage"))
	bool noDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hpAbsorptionRate"))
	int32 hpAbsorptionRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hpLimitAbsorptionRate"))
	int32 hpLimitAbsorptionRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hpAbsorption"))
	int32 hpAbsorption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hpAbsorptionAffectedByBonusRate"))
	bool hpAbsorptionAffectedByBonusRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상태 이상 1번의 \\\'적중\\\' – \\\'내성\\\' 공식에서 \\\'적중\\\' 에 더해지는 만 분률 값", DisplayName="abnormalityHitAdd1"))
	int32 abnormalityHitAdd1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상태이상 제거 확률", DisplayName="abnormalityRemoveRate1"))
	int32 abnormalityRemoveRate1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityApplyType1"))
	AbnormalityApplyType abnormalityApplyType1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 Damage notify 실행으로 계산된 대미지의 해당 비율만큼 어그로 증폭", DisplayName="abnormalityId1"))
	int32 abnormalityId1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="abnormalityApplyType 에 따라서 추가,삭제 할 GroupId", DisplayName="abnormalityGroupId1"))
	TArray<int32> abnormalityGroupId1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상태 이상 2번의 \\\'적중\\\' – \\\'내성\\\' 공식에서 \\\'적중\\\' 에 더해지는 만 분률 값", DisplayName="abnormalityHitAdd2"))
	int32 abnormalityHitAdd2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityRemoveRate2"))
	int32 abnormalityRemoveRate2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityApplyType2"))
	AbnormalityApplyType abnormalityApplyType2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityId2"))
	int32 abnormalityId2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityGroupId2"))
	TArray<int32> abnormalityGroupId2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="noneDamage"))
	int32 noneDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="elementalType"))
	DamageElementalType elementalType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="elementalDamage"))
	int32 elementalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="multipleAggro"))
	int32 multipleAggro;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SkillNotifyAoeVarySet 테이블 Id", DisplayName="aoeVaryId"))
	int32 aoeVaryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SkillPowerRate 연산 후 스킬의 고정된 추가 피해", DisplayName="fixedAdditionalDamage"))
	int32 fixedAdditionalDamage;
public:
	FGsSchemaSkillNotifyHitSet();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyHitSet& __Other) const
	{
		return FGsSchemaSkillNotifyHitSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyHitSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyHitSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyHitSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyHitSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyHitSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyHitSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyHitSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyHitSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyHitSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyHitSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
