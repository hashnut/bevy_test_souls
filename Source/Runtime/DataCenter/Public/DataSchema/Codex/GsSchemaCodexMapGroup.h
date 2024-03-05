/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Codex/GsSchemaCodexMapGroupBalance.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCodexMapGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Codex/BSGsCodexMapGroup"))
struct DATACENTER_API FGsSchemaCodexMapGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapList"))
	TArray<FGsSchemaMapDataRow> mapList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="balanceId"))
	FGsSchemaCodexMapGroupBalanceRow balanceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ingredientItemList"))
	TArray<FGsSchemaItemCommonRow> ingredientItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapGroupImage"))
	FSoftObjectPath mapGroupImage;
public:
	FGsSchemaCodexMapGroup();
	
protected:
	bool operator== (const FGsSchemaCodexMapGroup& __Other) const
	{
		return FGsSchemaCodexMapGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCodexMapGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCodexMapGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCodexMapGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCodexMapGroupRow() : Super()
	{
	}
	
	const FGsSchemaCodexMapGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCodexMapGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaCodexMapGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCodexMapGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCodexMapGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCodexMapGroup final : public UGsTable
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
		TMap<FName, const FGsSchemaCodexMapGroup*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCodexMapGroup>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCodexMapGroup*& OutRow) const
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
		const FGsSchemaCodexMapGroup* Temp;
		return !FindRowById(FGsSchemaCodexMapGroup().id, Temp);
	}
#endif
};
