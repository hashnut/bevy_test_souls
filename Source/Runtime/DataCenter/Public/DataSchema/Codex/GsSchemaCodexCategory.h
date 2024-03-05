/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Codex/GsSchemaCodexCategoryBalance.h"
#include "Runtime/DataCenter/Public/DataSchema/Codex/GsSchemaCodexMapGroup.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCodexCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Codex/BSGsCodexCategory"))
struct DATACENTER_API FGsSchemaCodexCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapGroupList"))
	TArray<FGsSchemaCodexMapGroupRow> mapGroupList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="balanceId"))
	FGsSchemaCodexCategoryBalanceRow balanceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="categoryBG"))
	FSoftObjectPath categoryBG;
public:
	FGsSchemaCodexCategory();
	
protected:
	bool operator== (const FGsSchemaCodexCategory& __Other) const
	{
		return FGsSchemaCodexCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCodexCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCodexCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCodexCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCodexCategoryRow() : Super()
	{
	}
	
	const FGsSchemaCodexCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCodexCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaCodexCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCodexCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCodexCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCodexCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Codex");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCodexCategory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCodexCategory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCodexCategory*& OutRow) const
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
		const FGsSchemaCodexCategory* Temp;
		return !FindRowById(FGsSchemaCodexCategory().id, Temp);
	}
#endif
};
