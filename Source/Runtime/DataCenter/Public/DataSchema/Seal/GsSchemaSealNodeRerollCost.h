/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealNodeRerollCost.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인노드 레벨별 리롤 비용", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealNodeRerollCost"))
struct DATACENTER_API FGsSchemaSealNodeRerollCost
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="리롤 비용 ID", DisplayName="rerollCostId"))
	int32 rerollCostId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="잠금 갯수", DisplayName="slotLockCount"))
	uint8 slotLockCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 아이템ID", DisplayName="itemId"))
	TArray<int32> itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 아이템갯수", DisplayName="itemAmount"))
	TArray<int32> itemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 재화 타입", DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 재화 갯수", DisplayName="currencyAmount"))
	int32 currencyAmount;
public:
	FGsSchemaSealNodeRerollCost();
	
protected:
	bool operator== (const FGsSchemaSealNodeRerollCost& __Other) const
	{
		return FGsSchemaSealNodeRerollCost::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealNodeRerollCost::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealNodeRerollCost::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealNodeRerollCostRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealNodeRerollCostRow() : Super()
	{
	}
	
	const FGsSchemaSealNodeRerollCost* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealNodeRerollCost* Row = nullptr;
		return Table->FindRow<FGsSchemaSealNodeRerollCost>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealNodeRerollCostRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealNodeRerollCostRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealNodeRerollCost final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
