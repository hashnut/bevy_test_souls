/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SystemMessage/Condition/GsSchemaSendingOption.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSystemMessageConditionTreasureMonster.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SystemMessage/Condition/BSGsSystemMessageConditionTreasureMonster"))
struct DATACENTER_API FGsSchemaSystemMessageConditionTreasureMonster
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useCategorySub"))
	ItemCategorySub useCategorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tickerRange"))
	TickerRange tickerRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minItemGrade"))
	ItemGrade minItemGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sendingOption"))
	FGsSchemaSendingOption sendingOption;
public:
	FGsSchemaSystemMessageConditionTreasureMonster();
	
protected:
	bool operator== (const FGsSchemaSystemMessageConditionTreasureMonster& __Other) const
	{
		return FGsSchemaSystemMessageConditionTreasureMonster::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSystemMessageConditionTreasureMonster::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSystemMessageConditionTreasureMonster::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSystemMessageConditionTreasureMonsterRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSystemMessageConditionTreasureMonsterRow() : Super()
	{
	}
	
	const FGsSchemaSystemMessageConditionTreasureMonster* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSystemMessageConditionTreasureMonster* Row = nullptr;
		return Table->FindRow<FGsSchemaSystemMessageConditionTreasureMonster>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSystemMessageConditionTreasureMonsterRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSystemMessageConditionTreasureMonsterRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSystemMessageConditionTreasureMonster final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SystemMessage/Condition");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSystemMessageConditionTreasureMonster*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSystemMessageConditionTreasureMonster>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSystemMessageConditionTreasureMonster*& OutRow) const
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
		const FGsSchemaSystemMessageConditionTreasureMonster* Temp;
		return !FindRowById(FGsSchemaSystemMessageConditionTreasureMonster().id, Temp);
	}
#endif
};
