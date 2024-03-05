/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillExecuteConditionMethodPassivity.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Param/BSGsSkillExecuteConditionMethodPassivity"))
struct DATACENTER_API FGsSchemaSkillExecuteConditionMethodPassivity
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="누구의 상태를 검증할 것인지 판단 (ShareSkillEnum.h - SkillTargetType enum 참조)", DisplayName="target"))
	SkillTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대상에게 부여된 패시비티 Id 검증", DisplayName="value"))
	int32 value;
public:
	FGsSchemaSkillExecuteConditionMethodPassivity();
	
protected:
	bool operator== (const FGsSchemaSkillExecuteConditionMethodPassivity& __Other) const
	{
		return FGsSchemaSkillExecuteConditionMethodPassivity::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillExecuteConditionMethodPassivity::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillExecuteConditionMethodPassivity::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillExecuteConditionMethodPassivityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillExecuteConditionMethodPassivityRow() : Super()
	{
	}
	
	const FGsSchemaSkillExecuteConditionMethodPassivity* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillExecuteConditionMethodPassivity* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillExecuteConditionMethodPassivity>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillExecuteConditionMethodPassivityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillExecuteConditionMethodPassivityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillExecuteConditionMethodPassivity final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Param");
	}
	
};
