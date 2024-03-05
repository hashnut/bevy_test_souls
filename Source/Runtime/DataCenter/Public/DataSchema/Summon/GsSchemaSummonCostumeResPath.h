/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaSummonCostumeResPath.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Summon/BSGsSummonCostumeResPath"))
struct DATACENTER_API FGsSchemaSummonCostumeResPath
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gender", Tooltip))
	CreatureGenderType gender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="objectBPPath", Tooltip))
	FSoftObjectPath objectBPPath;
public:
	FGsSchemaSummonCostumeResPath();
	
protected:
	bool operator== (const FGsSchemaSummonCostumeResPath& __Other) const
	{
		return FGsSchemaSummonCostumeResPath::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSummonCostumeResPath::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSummonCostumeResPath::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSummonCostumeResPathRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSummonCostumeResPathRow() : Super()
	{
	}
	
	const FGsSchemaSummonCostumeResPath* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSummonCostumeResPath* Row = nullptr;
		return Table->FindRow<FGsSchemaSummonCostumeResPath>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSummonCostumeResPathRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSummonCostumeResPathRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSummonCostumeResPath final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureGenderType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Summon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSummonCostumeResPath*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSummonCostumeResPath>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->gender, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CreatureGenderType& InId, OUT const FGsSchemaSummonCostumeResPath*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CreatureGenderType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSummonCostumeResPath* Temp;
		return !FindRowById(FGsSchemaSummonCostumeResPath().gender, Temp);
	}
#endif
};
