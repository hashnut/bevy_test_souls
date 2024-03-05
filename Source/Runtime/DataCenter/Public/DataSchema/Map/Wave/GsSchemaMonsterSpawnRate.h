/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterSpawnRate.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Wave/BSGsMonsterSpawnRate"))
struct DATACENTER_API FGsSchemaMonsterSpawnRate
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcId"))
	FGsSchemaNpcDataRow npcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rate"))
	int32 rate;
public:
	FGsSchemaMonsterSpawnRate();
	
protected:
	bool operator== (const FGsSchemaMonsterSpawnRate& __Other) const
	{
		return FGsSchemaMonsterSpawnRate::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMonsterSpawnRate::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMonsterSpawnRate::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMonsterSpawnRateRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMonsterSpawnRateRow() : Super()
	{
	}
	
	const FGsSchemaMonsterSpawnRate* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMonsterSpawnRate* Row = nullptr;
		return Table->FindRow<FGsSchemaMonsterSpawnRate>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMonsterSpawnRateRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMonsterSpawnRateRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMonsterSpawnRate final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Wave");
	}
	
};
