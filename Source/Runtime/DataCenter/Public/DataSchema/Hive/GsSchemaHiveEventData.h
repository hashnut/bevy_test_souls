/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Hive/GsHiveEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaHiveEventData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Hive/BSGsHiveEventData"))
struct DATACENTER_API FGsSchemaHiveEventData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hiveActionType"))
	EGsHiveActionType hiveActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="elementName"))
	FName elementName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ordinalCount"))
	int32 ordinalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventName"))
	FString eventName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventDesc"))
	FName eventDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="uniqueDummyId"))
	int32 uniqueDummyId;
public:
	FGsSchemaHiveEventData();
	
protected:
	bool operator== (const FGsSchemaHiveEventData& __Other) const
	{
		return FGsSchemaHiveEventData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaHiveEventData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaHiveEventData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaHiveEventDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaHiveEventDataRow() : Super()
	{
	}
	
	const FGsSchemaHiveEventData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaHiveEventData* Row = nullptr;
		return Table->FindRow<FGsSchemaHiveEventData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaHiveEventDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaHiveEventDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableHiveEventData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Hive");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaHiveEventData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaHiveEventData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->uniqueDummyId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaHiveEventData*& OutRow) const
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
		const FGsSchemaHiveEventData* Temp;
		return !FindRowById(FGsSchemaHiveEventData().uniqueDummyId, Temp);
	}
#endif
};
