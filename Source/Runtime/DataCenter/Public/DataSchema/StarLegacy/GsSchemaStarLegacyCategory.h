/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaStarLegacyCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/StarLegacy/BSGsStarLegacyCategory"))
struct DATACENTER_API FGsSchemaStarLegacyCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shuffleGold"))
	int32 shuffleGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rouletteGold"))
	int32 rouletteGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="selectMaxCount"))
	int32 selectMaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="chanceItemId"))
	int32 chanceItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="changeItemCount"))
	int32 changeItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rouletteFailRewardId"))
	int32 rouletteFailRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rouletteFailRewardCount"))
	int32 rouletteFailRewardCount;
public:
	FGsSchemaStarLegacyCategory();
	
protected:
	bool operator== (const FGsSchemaStarLegacyCategory& __Other) const
	{
		return FGsSchemaStarLegacyCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStarLegacyCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStarLegacyCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStarLegacyCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStarLegacyCategoryRow() : Super()
	{
	}
	
	const FGsSchemaStarLegacyCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStarLegacyCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaStarLegacyCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStarLegacyCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStarLegacyCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStarLegacyCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("StarLegacy");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaStarLegacyCategory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaStarLegacyCategory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaStarLegacyCategory*& OutRow) const
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
		const FGsSchemaStarLegacyCategory* Temp;
		return !FindRowById(FGsSchemaStarLegacyCategory().id, Temp);
	}
#endif
};
