/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SpiritShot/GsSchemaSpiritShotEffect.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpiritShotEnchantSumLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SpiritShot/BSGsSpiritShotEnchantSumLevel"))
struct DATACENTER_API FGsSchemaSpiritShotEnchantSumLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="needLevelSum"))
	int32 needLevelSum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityId"))
	FGsSchemaPassivitySetRow passivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="spiritShotEffect"))
	FGsSchemaSpiritShotEffect spiritShotEffect;
public:
	FGsSchemaSpiritShotEnchantSumLevel();
	
protected:
	bool operator== (const FGsSchemaSpiritShotEnchantSumLevel& __Other) const
	{
		return FGsSchemaSpiritShotEnchantSumLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpiritShotEnchantSumLevel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpiritShotEnchantSumLevel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpiritShotEnchantSumLevelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpiritShotEnchantSumLevelRow() : Super()
	{
	}
	
	const FGsSchemaSpiritShotEnchantSumLevel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpiritShotEnchantSumLevel* Row = nullptr;
		return Table->FindRow<FGsSchemaSpiritShotEnchantSumLevel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpiritShotEnchantSumLevelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpiritShotEnchantSumLevelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpiritShotEnchantSumLevel final : public UGsTable
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
		TMap<FName, const FGsSchemaSpiritShotEnchantSumLevel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSpiritShotEnchantSumLevel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSpiritShotEnchantSumLevel*& OutRow) const
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
		const FGsSchemaSpiritShotEnchantSumLevel* Temp;
		return !FindRowById(FGsSchemaSpiritShotEnchantSumLevel().id, Temp);
	}
#endif
};
