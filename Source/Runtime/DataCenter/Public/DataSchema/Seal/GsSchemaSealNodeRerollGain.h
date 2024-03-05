/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealNodeRerollGain.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인노드 레벨 별 봉인슬롯의 리롤 획득", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealNodeRerollGain"))
struct DATACENTER_API FGsSchemaSealNodeRerollGain
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="리롤획득ID", DisplayName="rerollGainId"))
	int32 rerollGainId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인노드 레벨", DisplayName="sealNodeLevel"))
	int32 sealNodeLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="리롤획득스탯풀ID", DisplayName="rerollGainStatPoolId"))
	int32 rerollGainStatPoolId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="리롤획득색상풀ID", DisplayName="rerollGainColorPoolId"))
	int32 rerollGainColorPoolId;
public:
	FGsSchemaSealNodeRerollGain();
	
protected:
	bool operator== (const FGsSchemaSealNodeRerollGain& __Other) const
	{
		return FGsSchemaSealNodeRerollGain::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealNodeRerollGain::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealNodeRerollGain::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealNodeRerollGainRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealNodeRerollGainRow() : Super()
	{
	}
	
	const FGsSchemaSealNodeRerollGain* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealNodeRerollGain* Row = nullptr;
		return Table->FindRow<FGsSchemaSealNodeRerollGain>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealNodeRerollGainRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealNodeRerollGainRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealNodeRerollGain final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
