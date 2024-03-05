/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyHeal.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifyHeal"))
struct DATACENTER_API FGsSchemaSkillNotifyHeal
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="DamageStatType", DisplayName="healStatType"))
	DamageStatType healStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="VARY_HP_MP 스킬 효과 실행 시 캐스터의 상태를 따를 것인지, 타겟의 상태를 따를 것인지 판단.", DisplayName="target"))
	SkillTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비율 관련 타입", DisplayName="healRatioValueType"))
	HealRatioValueType healRatioValueType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비율에 따른 회복량 (ValueType 에 따른 수치 * healRatioStatValue) = 비율 회복량", DisplayName="healRatioStatValue"))
	int32 healRatioStatValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="고정 회복량", DisplayName="healFixedStatValue"))
	int32 healFixedStatValue;
public:
	FGsSchemaSkillNotifyHeal();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyHeal& __Other) const
	{
		return FGsSchemaSkillNotifyHeal::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyHeal::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyHeal::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyHealRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyHealRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyHeal* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyHeal* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyHeal>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyHealRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyHealRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyHeal final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
