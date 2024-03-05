/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/GameObject/Define/BP_EGsGameObjectType.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyBeamParticle.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifyBeamParticle"))
struct DATACENTER_API FGsSchemaSkillNotifyBeamParticle
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetType"))
	SkillTargetType targetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "targetType == SkillTargetType::CASTER", EditConditionHides, DisplayName="targetCreatureType"))
	BP_EGsGameObjectType targetCreatureType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "targetType == SkillTargetType::CASTER", EditConditionHides, Tooltip="대상 오브젝트 테이블 Id(스폰되어있어야 함)", DisplayName="targetCreatureId"))
	int32 targetCreatureId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대상 파티클 위치 설정", DisplayName="targetSocketName"))
	FName targetSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectPath"))
	FSoftObjectPath effectPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="지속 시간 설정(Sec) 0초일 경우 스킬 지속시간까지 유효", DisplayName="duration"))
	float duration;
public:
	FGsSchemaSkillNotifyBeamParticle();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyBeamParticle& __Other) const
	{
		return FGsSchemaSkillNotifyBeamParticle::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyBeamParticle::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyBeamParticle::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyBeamParticleRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyBeamParticleRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyBeamParticle* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyBeamParticle* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyBeamParticle>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyBeamParticleRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyBeamParticleRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyBeamParticle final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
