/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaSpawnVisibleData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsSpawnVisibleData"))
struct DATACENTER_API FGsSchemaSpawnVisibleData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnId", Tooltip))
	int64 spawnId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnType", Tooltip))
	CreatureSpawnType spawnType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcFunctionType", Tooltip))
	NpcFunctionType npcFunctionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnData", Tooltip))
	FGsSchemaSpawnElement spawnData;
	
public:
	FGsSchemaSpawnVisibleData();
	
protected:
	bool operator== (const FGsSchemaSpawnVisibleData& __Other) const
	{
		return FGsSchemaSpawnVisibleData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpawnVisibleData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpawnVisibleData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpawnVisibleDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpawnVisibleDataRow() : Super()
	{
	}
	
	const FGsSchemaSpawnVisibleData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpawnVisibleData* Row = nullptr;
		return Table->FindRow<FGsSchemaSpawnVisibleData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpawnVisibleDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpawnVisibleDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpawnVisibleData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Spawn");
	}
	
};
