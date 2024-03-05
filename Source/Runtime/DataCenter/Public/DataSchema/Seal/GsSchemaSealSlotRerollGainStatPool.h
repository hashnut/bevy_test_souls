/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealSlotRerollGainStatPool.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인슬롯에 획득 가능한 스탯 풀", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealSlotRerollGainStatPool"))
struct DATACENTER_API FGsSchemaSealSlotRerollGainStatPool
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rerollGainStatPoolId"))
	int32 rerollGainStatPoolId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 스탯 타입", DisplayName="statType"))
	FGsStatTypeNameSelector statType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 스탯 깂", DisplayName="statValue"))
	int32 statValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상대비율", DisplayName="share"))
	int32 share;
public:
	FGsSchemaSealSlotRerollGainStatPool();
	
protected:
	bool operator== (const FGsSchemaSealSlotRerollGainStatPool& __Other) const
	{
		return FGsSchemaSealSlotRerollGainStatPool::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealSlotRerollGainStatPool::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealSlotRerollGainStatPool::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealSlotRerollGainStatPoolRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealSlotRerollGainStatPoolRow() : Super()
	{
	}
	
	const FGsSchemaSealSlotRerollGainStatPool* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealSlotRerollGainStatPool* Row = nullptr;
		return Table->FindRow<FGsSchemaSealSlotRerollGainStatPool>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealSlotRerollGainStatPoolRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealSlotRerollGainStatPoolRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealSlotRerollGainStatPool final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
