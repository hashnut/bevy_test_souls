/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLoadingScreenPlayTip.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Loading/BSGsLoadingScreenPlayTip"))
struct DATACENTER_API FGsSchemaLoadingScreenPlayTip
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minLevel"))
	int32 minLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxLevel"))
	int32 maxLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tipText"))
	FText tipText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tipColor"))
	FColor tipColor;
public:
	FGsSchemaLoadingScreenPlayTip();
	
protected:
	bool operator== (const FGsSchemaLoadingScreenPlayTip& __Other) const
	{
		return FGsSchemaLoadingScreenPlayTip::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLoadingScreenPlayTip::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLoadingScreenPlayTip::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLoadingScreenPlayTipRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLoadingScreenPlayTipRow() : Super()
	{
	}
	
	const FGsSchemaLoadingScreenPlayTip* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLoadingScreenPlayTip* Row = nullptr;
		return Table->FindRow<FGsSchemaLoadingScreenPlayTip>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLoadingScreenPlayTipRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLoadingScreenPlayTipRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLoadingScreenPlayTip final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Loading");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaLoadingScreenPlayTip*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaLoadingScreenPlayTip>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaLoadingScreenPlayTip*& OutRow) const
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
		const FGsSchemaLoadingScreenPlayTip* Temp;
		return !FindRowById(FGsSchemaLoadingScreenPlayTip().id, Temp);
	}
#endif
};
