/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityAnimationPriority.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/BSGsAbnormalityAnimationPriority"))
struct DATACENTER_API FGsSchemaAbnormalityAnimationPriority
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="prev"))
	EGsAbnormalityAnimProcessType prev;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="next"))
	EGsAbnormalityAnimProcessType next;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="processType"))
	EGsAbnormalityProcessType processType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
public:
	FGsSchemaAbnormalityAnimationPriority();
	
protected:
	bool operator== (const FGsSchemaAbnormalityAnimationPriority& __Other) const
	{
		return FGsSchemaAbnormalityAnimationPriority::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityAnimationPriority::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityAnimationPriority::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityAnimationPriorityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityAnimationPriorityRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityAnimationPriority* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityAnimationPriority* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityAnimationPriority>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityAnimationPriorityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityAnimationPriorityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityAnimationPriority final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality");
	}
	
};
