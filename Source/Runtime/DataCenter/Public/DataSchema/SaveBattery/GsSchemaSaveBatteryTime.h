/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSaveBatteryTime.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SaveBattery/BSGsSaveBatteryTime"))
struct DATACENTER_API FGsSchemaSaveBatteryTime
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="time"))
	float time;
public:
	FGsSchemaSaveBatteryTime();
	
protected:
	bool operator== (const FGsSchemaSaveBatteryTime& __Other) const
	{
		return FGsSchemaSaveBatteryTime::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSaveBatteryTime::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSaveBatteryTime::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSaveBatteryTimeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSaveBatteryTimeRow() : Super()
	{
	}
	
	const FGsSchemaSaveBatteryTime* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSaveBatteryTime* Row = nullptr;
		return Table->FindRow<FGsSchemaSaveBatteryTime>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSaveBatteryTimeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSaveBatteryTimeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSaveBatteryTime final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SaveBattery");
	}
	
};
