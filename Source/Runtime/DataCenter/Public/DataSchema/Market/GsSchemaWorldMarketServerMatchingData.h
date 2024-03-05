/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMarketServerMatchingData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Market/BSGsWorldMarketServerMatchingData"))
struct DATACENTER_API FGsSchemaWorldMarketServerMatchingData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldMarketId"))
	int32 worldMarketId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="descNotUse"))
	FString descNotUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldList"))
	TArray<FGsSchemaWorldDataRow> worldList;
public:
	FGsSchemaWorldMarketServerMatchingData();
	
protected:
	bool operator== (const FGsSchemaWorldMarketServerMatchingData& __Other) const
	{
		return FGsSchemaWorldMarketServerMatchingData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldMarketServerMatchingData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldMarketServerMatchingData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldMarketServerMatchingDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldMarketServerMatchingDataRow() : Super()
	{
	}
	
	const FGsSchemaWorldMarketServerMatchingData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldMarketServerMatchingData* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldMarketServerMatchingData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldMarketServerMatchingDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldMarketServerMatchingDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldMarketServerMatchingData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Market");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWorldMarketServerMatchingData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldMarketServerMatchingData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->worldMarketId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldMarketServerMatchingData*& OutRow) const
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
		const FGsSchemaWorldMarketServerMatchingData* Temp;
		return !FindRowById(FGsSchemaWorldMarketServerMatchingData().worldMarketId, Temp);
	}
#endif
};
