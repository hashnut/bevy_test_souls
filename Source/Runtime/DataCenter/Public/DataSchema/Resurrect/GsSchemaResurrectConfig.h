/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Resurrect/GsSchemaResurrectionCost.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaResurrectConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Resurrect/BSGsResurrectConfig"))
struct DATACENTER_API FGsSchemaResurrectConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="freeReviveCount"))
	int32 freeReviveCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MinimumLevel"))
	int32 MinimumLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxExpPenaltyCount"))
	int32 maxExpPenaltyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxItemPenaltyCount"))
	int32 maxItemPenaltyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resurrrectionCostList"))
	TArray<FGsSchemaResurrectionCost> resurrrectionCostList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="autoRespawnSecond"))
	int32 autoRespawnSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="InstantResurrectionSecond"))
	int32 InstantResurrectionSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spaceCrackRespawnSecond"))
	int32 spaceCrackRespawnSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="resurrectItem"))
	FGsSchemaItemCommonRow resurrectItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxChargeCount"))
	int32 maxChargeCount;
public:
	FGsSchemaResurrectConfig();
	
protected:
	bool operator== (const FGsSchemaResurrectConfig& __Other) const
	{
		return FGsSchemaResurrectConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaResurrectConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaResurrectConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaResurrectConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaResurrectConfigRow() : Super()
	{
	}
	
	const FGsSchemaResurrectConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaResurrectConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaResurrectConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaResurrectConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaResurrectConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableResurrectConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Resurrect");
	}
	
};
