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
#include "GsSchemaHiveEvent.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Hive/BSGsHiveEvent"))
struct DATACENTER_API FGsSchemaHiveEvent
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
	FGsSchemaHiveEvent();
	
protected:
	bool operator== (const FGsSchemaHiveEvent& __Other) const
	{
		return FGsSchemaHiveEvent::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaHiveEvent::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaHiveEvent::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaHiveEventRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaHiveEventRow() : Super()
	{
	}
	
	const FGsSchemaHiveEvent* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaHiveEvent* Row = nullptr;
		return Table->FindRow<FGsSchemaHiveEvent>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaHiveEventRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaHiveEventRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableHiveEvent final : public UGsTable
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
		TMap<FName, const FGsSchemaHiveEvent*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaHiveEvent>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaHiveEvent*& OutRow) const
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
		const FGsSchemaHiveEvent* Temp;
		return !FindRowById(FGsSchemaHiveEvent().uniqueDummyId, Temp);
	}
#endif
};
