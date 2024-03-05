/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillExecuteConditionMethodAbnormality.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Param/BSGsSkillExecuteConditionMethodAbnormality"))
struct DATACENTER_API FGsSchemaSkillExecuteConditionMethodAbnormality
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="누구의 상태를 검증할 것인지 판단 (ShareSkillEnum.h - SkillTargetType enum 참조)", DisplayName="target"))
	SkillTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="true - 하위 조건 충족 시 부합, false - 하위 조건 미충족 시 부합", DisplayName="setAbnormalityAllow"))
	SetAbnormalityAllow setAbnormalityAllow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="어떤 이상상태 조건을 검증할 것인지 판단, NONE 일 경우 무조건 충족으로 판단", DisplayName="type"))
	SkillExecuteConditionAbnormalityConditionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="\"type\" 항목이 ABNORMALITY_EFFECT_TYPE 일 경우 사용. AbnormalityEffectType 내용 기재", DisplayName="param1"))
	AbnormalityEffectType param1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="\"type\" 항목이 ABNORMALITY_ID 일 경우 사용. Abnormality ID 내용 기재", DisplayName="param2"))
	int32 param2;
public:
	FGsSchemaSkillExecuteConditionMethodAbnormality();
	
protected:
	bool operator== (const FGsSchemaSkillExecuteConditionMethodAbnormality& __Other) const
	{
		return FGsSchemaSkillExecuteConditionMethodAbnormality::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillExecuteConditionMethodAbnormality::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillExecuteConditionMethodAbnormality::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillExecuteConditionMethodAbnormalityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillExecuteConditionMethodAbnormalityRow() : Super()
	{
	}
	
	const FGsSchemaSkillExecuteConditionMethodAbnormality* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillExecuteConditionMethodAbnormality* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillExecuteConditionMethodAbnormality>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillExecuteConditionMethodAbnormalityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillExecuteConditionMethodAbnormalityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillExecuteConditionMethodAbnormality final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Param");
	}
	
};
