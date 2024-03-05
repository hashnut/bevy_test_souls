/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaItemEffectSpawnTreasureMonster.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectSpawnTreasureMonster"))
struct DATACENTER_API FGsSchemaItemEffectSpawnTreasureMonster
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectId"))
	int64 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcId"))
	int32 npcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="time"))
	int64 time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="allowedLocation"))
	TArray<int32> allowedLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="successRewardId"))
	int32 successRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tickerRange"))
	TickerRange tickerRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemPresentRate"))
	int32 itemPresentRate;
public:
	FGsSchemaItemEffectSpawnTreasureMonster();
	
protected:
	bool operator== (const FGsSchemaItemEffectSpawnTreasureMonster& __Other) const
	{
		return FGsSchemaItemEffectSpawnTreasureMonster::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectSpawnTreasureMonster::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectSpawnTreasureMonster::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectSpawnTreasureMonsterRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectSpawnTreasureMonsterRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectSpawnTreasureMonster* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectSpawnTreasureMonster* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectSpawnTreasureMonster>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectSpawnTreasureMonsterRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectSpawnTreasureMonsterRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectSpawnTreasureMonster final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectSpawnTreasureMonster*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectSpawnTreasureMonster>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaItemEffectSpawnTreasureMonster*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaItemEffectSpawnTreasureMonster* Temp;
		return !FindRowById(FGsSchemaItemEffectSpawnTreasureMonster().effectId, Temp);
	}
#endif
};
