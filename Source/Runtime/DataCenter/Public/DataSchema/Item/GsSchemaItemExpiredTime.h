/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemExpiredTime.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemExpiredTime"))
struct DATACENTER_API FGsSchemaItemExpiredTime
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expiredTimeId"))
	int32 expiredTimeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="startTime"))
	FString startTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endTime"))
	FString endTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expiredTimeM"))
	int32 expiredTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationTime"))
	int32 durationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isAllowToDelete"))
	bool isAllowToDelete;
public:
	FGsSchemaItemExpiredTime();
	
protected:
	bool operator== (const FGsSchemaItemExpiredTime& __Other) const
	{
		return FGsSchemaItemExpiredTime::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemExpiredTime::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemExpiredTime::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemExpiredTimeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemExpiredTimeRow() : Super()
	{
	}
	
	const FGsSchemaItemExpiredTime* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemExpiredTime* Row = nullptr;
		return Table->FindRow<FGsSchemaItemExpiredTime>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemExpiredTimeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemExpiredTimeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemExpiredTime final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemExpiredTime*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemExpiredTime>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->expiredTimeId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemExpiredTime*& OutRow) const
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
		const FGsSchemaItemExpiredTime* Temp;
		return !FindRowById(FGsSchemaItemExpiredTime().expiredTimeId, Temp);
	}
#endif
};
