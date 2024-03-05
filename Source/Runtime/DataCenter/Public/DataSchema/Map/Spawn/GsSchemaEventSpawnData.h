/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventSpawnData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="SpawnData 테이블에서 키가 되는 MapId 와 SpawnGroupId 만 추출한 테이블. 클라에서 사용하지 않는 테이블을 로드하지 않지만, 서버와의 통신을 위해 키값이 필요하여 생성하여 사용함.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsEventSpawnData"))
struct DATACENTER_API FGsSchemaEventSpawnData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MapId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Territory zone unique id"))
	int32 SpawnGroupId;
	
public:
	FGsSchemaEventSpawnData();
	
protected:
	bool operator== (const FGsSchemaEventSpawnData& __Other) const
	{
		return FGsSchemaEventSpawnData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEventSpawnData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEventSpawnData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEventSpawnDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEventSpawnDataRow() : Super()
	{
	}
	
	const FGsSchemaEventSpawnData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEventSpawnData* Row = nullptr;
		return Table->FindRow<FGsSchemaEventSpawnData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEventSpawnDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEventSpawnDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEventSpawnData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Spawn");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEventSpawnData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEventSpawnData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->SpawnGroupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaEventSpawnData*& OutRow) const
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
		const FGsSchemaEventSpawnData* Temp;
		return !FindRowById(FGsSchemaEventSpawnData().SpawnGroupId, Temp);
	}
#endif
};
