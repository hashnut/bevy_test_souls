/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackWavePattern.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Common/GsSchemaMinMaxInteger.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackData"))
struct DATACENTER_API FGsSchemaSpaceCrackData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnGroupId"))
	int32 spawnGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnLevel"))
	int32 spawnLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="activationTimeCron"))
	FString activationTimeCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="activationTime"))
	FGsSchemaMinMaxInteger activationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="duration"))
	int32 duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="despawnTime"))
	int32 despawnTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wavePatternList"))
	TArray<FGsSchemaSpaceCrackWavePattern> wavePatternList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="monsterPollutantId"))
	FGsSchemaNpcDataRow monsterPollutantId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="pollutantAbnormalityList"))
	TArray<FGsSchemaAbnormalitySetRow> pollutantAbnormalityList;
public:
	FGsSchemaSpaceCrackData();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackData& __Other) const
	{
		return FGsSchemaSpaceCrackData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpaceCrackData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpaceCrackData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpaceCrackDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpaceCrackDataRow() : Super()
	{
	}
	
	const FGsSchemaSpaceCrackData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpaceCrackData* Row = nullptr;
		return Table->FindRow<FGsSchemaSpaceCrackData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpaceCrackDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpaceCrackDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpaceCrackData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/SpaceCrack");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSpaceCrackData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSpaceCrackData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSpaceCrackData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSpaceCrackData* Temp;
		return !FindRowById(FGsSchemaSpaceCrackData().id, Temp);
	}
#endif
};
