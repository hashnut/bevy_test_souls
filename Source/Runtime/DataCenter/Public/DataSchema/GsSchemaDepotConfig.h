/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaExtendSlotCost.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDepotConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsDepotConfig"))
struct DATACENTER_API FGsSchemaDepotConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="depositCost"))
	int32 depositCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="withdrawCost"))
	int32 withdrawCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendSlotCount"))
	int32 extendSlotCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendList"))
	TArray<FGsSchemaExtendSlotCost> extendList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defaultSlotCount"))
	int32 defaultSlotCount;
public:
	FGsSchemaDepotConfig();
	
protected:
	bool operator== (const FGsSchemaDepotConfig& __Other) const
	{
		return FGsSchemaDepotConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDepotConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDepotConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDepotConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDepotConfigRow() : Super()
	{
	}
	
	const FGsSchemaDepotConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDepotConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaDepotConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDepotConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDepotConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDepotConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
};
