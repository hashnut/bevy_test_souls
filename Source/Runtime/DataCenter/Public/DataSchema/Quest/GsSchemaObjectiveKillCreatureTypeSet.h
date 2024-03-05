/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaObjectiveKillCreatureTypeSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsObjectiveKillCreatureTypeSet"))
struct DATACENTER_API FGsSchemaObjectiveKillCreatureTypeSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureRankType"))
	CreatureRankType creatureRankType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcFunctionType"))
	NpcFunctionType npcFunctionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="count"))
	int32 count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropRate"))
	float dropRate;
public:
	FGsSchemaObjectiveKillCreatureTypeSet();
	
protected:
	bool operator== (const FGsSchemaObjectiveKillCreatureTypeSet& __Other) const
	{
		return FGsSchemaObjectiveKillCreatureTypeSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaObjectiveKillCreatureTypeSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaObjectiveKillCreatureTypeSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaObjectiveKillCreatureTypeSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaObjectiveKillCreatureTypeSetRow() : Super()
	{
	}
	
	const FGsSchemaObjectiveKillCreatureTypeSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaObjectiveKillCreatureTypeSet* Row = nullptr;
		return Table->FindRow<FGsSchemaObjectiveKillCreatureTypeSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaObjectiveKillCreatureTypeSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaObjectiveKillCreatureTypeSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableObjectiveKillCreatureTypeSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaObjectiveKillCreatureTypeSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaObjectiveKillCreatureTypeSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaObjectiveKillCreatureTypeSet*& OutRow) const
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
		const FGsSchemaObjectiveKillCreatureTypeSet* Temp;
		return !FindRowById(FGsSchemaObjectiveKillCreatureTypeSet().id, Temp);
	}
#endif
};
