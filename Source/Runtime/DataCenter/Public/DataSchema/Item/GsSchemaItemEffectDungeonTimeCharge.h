/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectDungeonTimeCharge.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectDungeonTimeCharge"))
struct DATACENTER_API FGsSchemaItemEffectDungeonTimeCharge
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungeonGroupDataId"))
	FGsSchemaDungeonGroupDataRow dungeonGroupDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="value"))
	int32 value;
public:
	FGsSchemaItemEffectDungeonTimeCharge();
	
protected:
	bool operator== (const FGsSchemaItemEffectDungeonTimeCharge& __Other) const
	{
		return FGsSchemaItemEffectDungeonTimeCharge::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectDungeonTimeCharge::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectDungeonTimeCharge::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectDungeonTimeChargeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectDungeonTimeChargeRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectDungeonTimeCharge* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectDungeonTimeCharge* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectDungeonTimeCharge>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectDungeonTimeChargeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectDungeonTimeChargeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectDungeonTimeCharge final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEffectDungeonTimeCharge*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectDungeonTimeCharge>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectDungeonTimeCharge*& OutRow) const
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
		const FGsSchemaItemEffectDungeonTimeCharge* Temp;
		return !FindRowById(FGsSchemaItemEffectDungeonTimeCharge().effectId, Temp);
	}
#endif
};
