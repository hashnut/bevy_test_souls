/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealSlotRerollGainColorPool.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인슬롯에 획득 가능한 색상의 풀", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealSlotRerollGainColorPool"))
struct DATACENTER_API FGsSchemaSealSlotRerollGainColorPool
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rerollGainColorPoolId"))
	int32 rerollGainColorPoolId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 색상 타입", DisplayName="colorType"))
	SealSlotColor colorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상대비율", DisplayName="share"))
	int32 share;
public:
	FGsSchemaSealSlotRerollGainColorPool();
	
protected:
	bool operator== (const FGsSchemaSealSlotRerollGainColorPool& __Other) const
	{
		return FGsSchemaSealSlotRerollGainColorPool::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealSlotRerollGainColorPool::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealSlotRerollGainColorPool::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealSlotRerollGainColorPoolRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealSlotRerollGainColorPoolRow() : Super()
	{
	}
	
	const FGsSchemaSealSlotRerollGainColorPool* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealSlotRerollGainColorPool* Row = nullptr;
		return Table->FindRow<FGsSchemaSealSlotRerollGainColorPool>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealSlotRerollGainColorPoolRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealSlotRerollGainColorPoolRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealSlotRerollGainColorPool final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
