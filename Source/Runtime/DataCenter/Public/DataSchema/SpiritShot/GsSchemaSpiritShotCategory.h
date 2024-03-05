/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpiritShotCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SpiritShot/BSGsSpiritShotCategory"))
struct DATACENTER_API FGsSchemaSpiritShotCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryName"))
	FText categoryName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="unlockContentsId"))
	FGsSchemaUnlockContentsRow unlockContentsId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="coreImage"))
	FSoftObjectPath coreImage;
public:
	FGsSchemaSpiritShotCategory();
	
protected:
	bool operator== (const FGsSchemaSpiritShotCategory& __Other) const
	{
		return FGsSchemaSpiritShotCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpiritShotCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpiritShotCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpiritShotCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpiritShotCategoryRow() : Super()
	{
	}
	
	const FGsSchemaSpiritShotCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpiritShotCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaSpiritShotCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpiritShotCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpiritShotCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpiritShotCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SpiritShot");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSpiritShotCategory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSpiritShotCategory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSpiritShotCategory*& OutRow) const
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
		const FGsSchemaSpiritShotCategory* Temp;
		return !FindRowById(FGsSchemaSpiritShotCategory().id, Temp);
	}
#endif
};
