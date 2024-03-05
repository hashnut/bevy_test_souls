/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTownData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Town/BSGsTownData"))
struct DATACENTER_API FGsSchemaTownData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Npc Id, reference npc table", DisplayName="CreatureId"))
	FGsSchemaNpcDataRow CreatureId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Pos"))
	FVector Pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Dir"))
	int32 Dir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isClientSpawn"))
	bool isClientSpawn;
public:
	FGsSchemaTownData();
	
protected:
	bool operator== (const FGsSchemaTownData& __Other) const
	{
		return FGsSchemaTownData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTownData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTownData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTownDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTownDataRow() : Super()
	{
	}
	
	const FGsSchemaTownData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTownData* Row = nullptr;
		return Table->FindRow<FGsSchemaTownData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTownDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTownDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTownData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Town");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTownData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTownData>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaTownData*& OutRow) const
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
		const FGsSchemaTownData* Temp;
		return !FindRowById(FGsSchemaTownData().id, Temp);
	}
#endif
};
