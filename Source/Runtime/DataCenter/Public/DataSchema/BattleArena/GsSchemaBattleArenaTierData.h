/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingGroupData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedRankEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattleArenaTierData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattleArena/BSGsBattleArenaTierData"))
struct DATACENTER_API FGsSchemaBattleArenaTierData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankSearchType"))
	RankSearchType rankSearchType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="symbolPath"))
	FSoftObjectPath symbolPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="scroeRange"))
	FGsSchemaRankingGroupData scroeRange;
public:
	FGsSchemaBattleArenaTierData();
	
protected:
	bool operator== (const FGsSchemaBattleArenaTierData& __Other) const
	{
		return FGsSchemaBattleArenaTierData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattleArenaTierData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattleArenaTierData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattleArenaTierDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattleArenaTierDataRow() : Super()
	{
	}
	
	const FGsSchemaBattleArenaTierData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattleArenaTierData* Row = nullptr;
		return Table->FindRow<FGsSchemaBattleArenaTierData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattleArenaTierDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattleArenaTierDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattleArenaTierData final : public UGsTable
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
		TMap<FName, const FGsSchemaBattleArenaTierData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattleArenaTierData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaBattleArenaTierData*& OutRow) const
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
		const FGsSchemaBattleArenaTierData* Temp;
		return !FindRowById(FGsSchemaBattleArenaTierData().id, Temp);
	}
#endif
};
