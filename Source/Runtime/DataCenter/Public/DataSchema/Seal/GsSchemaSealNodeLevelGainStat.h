/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealNodeLevelGainStat.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인노드 레벨 획득 스탯", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealNodeLevelGainStat"))
struct DATACENTER_API FGsSchemaSealNodeLevelGainStat
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인노드 레벨", DisplayName="levelGainStatId"))
	int32 levelGainStatId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 스탯 타입", DisplayName="statType"))
	FGsStatTypeNameSelector statType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 스탯 값", DisplayName="statValue"))
	int32 statValue;
public:
	FGsSchemaSealNodeLevelGainStat();
	
protected:
	bool operator== (const FGsSchemaSealNodeLevelGainStat& __Other) const
	{
		return FGsSchemaSealNodeLevelGainStat::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealNodeLevelGainStat::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealNodeLevelGainStat::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealNodeLevelGainStatRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealNodeLevelGainStatRow() : Super()
	{
	}
	
	const FGsSchemaSealNodeLevelGainStat* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealNodeLevelGainStat* Row = nullptr;
		return Table->FindRow<FGsSchemaSealNodeLevelGainStat>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealNodeLevelGainStatRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealNodeLevelGainStatRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealNodeLevelGainStat final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
