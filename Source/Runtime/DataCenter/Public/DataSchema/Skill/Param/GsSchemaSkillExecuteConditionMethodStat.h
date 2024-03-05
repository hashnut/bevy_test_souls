/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillExecuteConditionMethodStat.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Param/BSGsSkillExecuteConditionMethodStat"))
struct DATACENTER_API FGsSchemaSkillExecuteConditionMethodStat
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="누구의 상태를 검증할 것인지 판단 (ShareSkillEnum.h - SkillTargetType enum 참조)", DisplayName="target"))
	SkillTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="어떤 스탯에 대한 검증을 할 것인지 판단, NONE 일 경우 무조건 충족으로 판단", DisplayName="statType"))
	FGsStatTypeNameSelector statType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 스탯이 특정 수치 \"이상\"/\"이하\" 판단", DisplayName="statCondition"))
	SkillExecuteConditionStatConditionMethod statCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 스탯이 비율인지 값인지 판단", DisplayName="type"))
	SkillExecuteConditionStatConditionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 비율 or 값 입력", DisplayName="param"))
	int32 param;
public:
	FGsSchemaSkillExecuteConditionMethodStat();
	
protected:
	bool operator== (const FGsSchemaSkillExecuteConditionMethodStat& __Other) const
	{
		return FGsSchemaSkillExecuteConditionMethodStat::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillExecuteConditionMethodStat::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillExecuteConditionMethodStat::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillExecuteConditionMethodStatRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillExecuteConditionMethodStatRow() : Super()
	{
	}
	
	const FGsSchemaSkillExecuteConditionMethodStat* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillExecuteConditionMethodStat* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillExecuteConditionMethodStat>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillExecuteConditionMethodStatRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillExecuteConditionMethodStatRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillExecuteConditionMethodStat final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Param");
	}
	
};
