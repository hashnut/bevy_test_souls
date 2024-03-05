/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillTargetFilter.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillTargetFilter"))
struct DATACENTER_API FGsSchemaSkillTargetFilter
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="targetCount"))
	int32 targetCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iffType"))
	IffApplyType iffType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="TargetFilterType 열거형", DisplayName="targetFilterType"))
	SkillTargetFilterType targetFilterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="targetFilterType 타입에 따라 달라지는 문자열(enum) param 데이터", DisplayName="filterParam1"))
	FString filterParam1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="targetFilterType 타입에 따라 달라지는 정수 param 데이터", DisplayName="filterParam2"))
	int32 filterParam2;
public:
	FGsSchemaSkillTargetFilter();
	
protected:
	bool operator== (const FGsSchemaSkillTargetFilter& __Other) const
	{
		return FGsSchemaSkillTargetFilter::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillTargetFilter::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillTargetFilter::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillTargetFilterRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillTargetFilterRow() : Super()
	{
	}
	
	const FGsSchemaSkillTargetFilter* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillTargetFilter* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillTargetFilter>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillTargetFilterRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillTargetFilterRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillTargetFilter final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
};
