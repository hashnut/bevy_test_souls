/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcMapInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaNpcMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Npc/BSGsNpcMapData"))
struct DATACENTER_API FGsSchemaNpcMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isDummy"))
	bool isDummy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="balanceId"))
	int64 balanceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rank"))
	CreatureRankType rank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnDataArray"))
	TArray<FGsSchemaNpcMapInfo> spawnDataArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="clientSpawnDataArray"))
	TArray<FGsSchemaNpcMapInfo> clientSpawnDataArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventSpawnDataArray"))
	TArray<FGsSchemaNpcMapInfo> eventSpawnDataArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spaceCrackArray"))
	TArray<FGsSchemaNpcMapInfo> spaceCrackArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildDungeonArray"))
	TArray<FGsSchemaNpcMapInfo> guildDungeonArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fieldBossArray"))
	TArray<FGsSchemaNpcMapInfo> fieldBossArray;
public:
	FGsSchemaNpcMapData();
	
protected:
	bool operator== (const FGsSchemaNpcMapData& __Other) const
	{
		return FGsSchemaNpcMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcMapDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcMapData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcMapData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcMapData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Npc");
	}
	
};
