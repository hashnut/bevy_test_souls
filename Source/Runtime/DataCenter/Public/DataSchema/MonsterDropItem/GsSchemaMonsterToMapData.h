/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/MonsterDropItem/GsSchemaMonsterInMapData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterToMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterDropItem/BSGsMonsterToMapData"))
struct DATACENTER_API FGsSchemaMonsterToMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id"))
	int64 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MonsterName"))
	FName MonsterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MonsterInMapDataList"))
	TArray<FGsSchemaMonsterInMapData> MonsterInMapDataList;
public:
	FGsSchemaMonsterToMapData();
	
protected:
	bool operator== (const FGsSchemaMonsterToMapData& __Other) const
	{
		return FGsSchemaMonsterToMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMonsterToMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMonsterToMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMonsterToMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMonsterToMapDataRow() : Super()
	{
	}
	
	const FGsSchemaMonsterToMapData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMonsterToMapData* Row = nullptr;
		return Table->FindRow<FGsSchemaMonsterToMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMonsterToMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMonsterToMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMonsterToMapData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<FName, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MonsterDropItem");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMonsterToMapData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMonsterToMapData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->MonsterName, Row.Key);
		}
	}
	
public:
	bool FindRowById(const FName& InId, OUT const FGsSchemaMonsterToMapData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const FName& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaMonsterToMapData* Temp;
		return !FindRowById(FGsSchemaMonsterToMapData().MonsterName, Temp);
	}
#endif
};
