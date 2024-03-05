/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaResurrectionCost.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Resurrect/BSGsResurrectionCost"))
struct DATACENTER_API FGsSchemaResurrectionCost
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resurrectionCount", Tooltip))
	int32 resurrectionCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resurrectionCost", Tooltip))
	int32 resurrectionCost;
public:
	FGsSchemaResurrectionCost();
	
protected:
	bool operator== (const FGsSchemaResurrectionCost& __Other) const
	{
		return FGsSchemaResurrectionCost::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaResurrectionCost::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaResurrectionCost::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaResurrectionCostRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaResurrectionCostRow() : Super()
	{
	}
	
	const FGsSchemaResurrectionCost* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaResurrectionCost* Row = nullptr;
		return Table->FindRow<FGsSchemaResurrectionCost>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaResurrectionCostRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaResurrectionCostRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableResurrectionCost final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Resurrect");
	}
	
};
