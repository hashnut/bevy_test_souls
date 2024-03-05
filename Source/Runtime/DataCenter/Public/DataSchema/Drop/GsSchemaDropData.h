/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropItemGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Drop/GsSchemaDropProbGroup.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDropData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Drop/BSGsDropData"))
struct DATACENTER_API FGsSchemaDropData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropType"))
	DropCheckType dropType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropValue"))
	FString dropValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rate"))
	int32 rate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemGroup"))
	FGsSchemaDropItemGroupRow itemGroup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="probGroup"))
	FGsSchemaDropProbGroupRow probGroup;
public:
	FGsSchemaDropData();
	
protected:
	bool operator== (const FGsSchemaDropData& __Other) const
	{
		return FGsSchemaDropData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDropData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDropData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDropDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDropDataRow() : Super()
	{
	}
	
	const FGsSchemaDropData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDropData* Row = nullptr;
		return Table->FindRow<FGsSchemaDropData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDropDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDropDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDropData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Drop");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaDropData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaDropData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaDropData*& OutRow) const
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
		const FGsSchemaDropData* Temp;
		return !FindRowById(FGsSchemaDropData().id, Temp);
	}
#endif
};
