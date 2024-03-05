/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattleArenaRewardPointData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattleArena/BSGsBattleArenaRewardPointData"))
struct DATACENTER_API FGsSchemaBattleArenaRewardPointData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcDataId"))
	int32 npcDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="killPoint"))
	int32 killPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="assitPoint"))
	int32 assitPoint;
public:
	FGsSchemaBattleArenaRewardPointData();
	
protected:
	bool operator== (const FGsSchemaBattleArenaRewardPointData& __Other) const
	{
		return FGsSchemaBattleArenaRewardPointData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattleArenaRewardPointData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattleArenaRewardPointData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattleArenaRewardPointDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattleArenaRewardPointDataRow() : Super()
	{
	}
	
	const FGsSchemaBattleArenaRewardPointData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattleArenaRewardPointData* Row = nullptr;
		return Table->FindRow<FGsSchemaBattleArenaRewardPointData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattleArenaRewardPointDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattleArenaRewardPointDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattleArenaRewardPointData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattleArena");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaBattleArenaRewardPointData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattleArenaRewardPointData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->npcDataId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaBattleArenaRewardPointData*& OutRow) const
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
		const FGsSchemaBattleArenaRewardPointData* Temp;
		return !FindRowById(FGsSchemaBattleArenaRewardPointData().npcDataId, Temp);
	}
#endif
};
