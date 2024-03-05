/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSummonConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Summon/BSGsSummonConfig"))
struct DATACENTER_API FGsSchemaSummonConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyMileage"))
	int32 fairyMileage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fairyMileageRewardItem"))
	FGsSchemaItemCommonRow fairyMileageRewardItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeMileage"))
	int32 costumeMileage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="costumeMileageRewardItem"))
	FGsSchemaItemCommonRow costumeMileageRewardItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyMileageUnique"))
	int32 fairyMileageUnique;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fairyMileageRewardItemUnique"))
	FGsSchemaItemCommonRow fairyMileageRewardItemUnique;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeMileageUnique"))
	int32 costumeMileageUnique;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="costumeMileageRewardItemUnique"))
	FGsSchemaItemCommonRow costumeMileageRewardItemUnique;
public:
	FGsSchemaSummonConfig();
	
protected:
	bool operator== (const FGsSchemaSummonConfig& __Other) const
	{
		return FGsSchemaSummonConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSummonConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSummonConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSummonConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSummonConfigRow() : Super()
	{
	}
	
	const FGsSchemaSummonConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSummonConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaSummonConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSummonConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSummonConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSummonConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Summon");
	}
	
};
