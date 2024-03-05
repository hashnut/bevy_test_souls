/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaTitleConditionDataCreature.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataCreature"))
struct DATACENTER_API FGsSchemaTitleConditionDataCreature
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCreatureId"))
	int32 mCreatureId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCreatureRankType"))
	CreatureRankType mCreatureRankType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCreatureRaceType"))
	CreatureRaceType mCreatureRaceType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mMapId"))
	int32 mMapId;
public:
	FGsSchemaTitleConditionDataCreature();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataCreature& __Other) const
	{
		return FGsSchemaTitleConditionDataCreature::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataCreature::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataCreature::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataCreatureRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataCreatureRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataCreature* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataCreature* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataCreature>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataCreatureRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataCreatureRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataCreature final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTitleConditionDataCreature*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTitleConditionDataCreature>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->mCreatureId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaTitleConditionDataCreature*& OutRow) const
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
		const FGsSchemaTitleConditionDataCreature* Temp;
		return !FindRowById(FGsSchemaTitleConditionDataCreature().mCreatureId, Temp);
	}
#endif
};
