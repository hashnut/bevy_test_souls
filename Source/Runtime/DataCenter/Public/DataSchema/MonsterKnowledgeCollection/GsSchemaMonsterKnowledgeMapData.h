/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterKnowledgeMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterKnowledgeCollection/BSGsMonsterKnowledgeMapData"))
struct DATACENTER_API FGsSchemaMonsterKnowledgeMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcData"))
	FGsSchemaNpcDataRow npcData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnMapDataArray"))
	TArray<FGsSchemaMapDataRow> spawnMapDataArray;
public:
	FGsSchemaMonsterKnowledgeMapData();
	
protected:
	bool operator== (const FGsSchemaMonsterKnowledgeMapData& __Other) const
	{
		return FGsSchemaMonsterKnowledgeMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMonsterKnowledgeMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMonsterKnowledgeMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMonsterKnowledgeMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMonsterKnowledgeMapDataRow() : Super()
	{
	}
	
	const FGsSchemaMonsterKnowledgeMapData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMonsterKnowledgeMapData* Row = nullptr;
		return Table->FindRow<FGsSchemaMonsterKnowledgeMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMonsterKnowledgeMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMonsterKnowledgeMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMonsterKnowledgeMapData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MonsterKnowledgeCollection");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMonsterKnowledgeMapData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMonsterKnowledgeMapData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMonsterKnowledgeMapData*& OutRow) const
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
		const FGsSchemaMonsterKnowledgeMapData* Temp;
		return !FindRowById(FGsSchemaMonsterKnowledgeMapData().id, Temp);
	}
#endif
};
