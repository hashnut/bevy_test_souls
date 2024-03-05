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
#include "GsSchemaCostumeResPath.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeResPath"))
struct DATACENTER_API FGsSchemaCostumeResPath
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	CreatureGenderType gender;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="objectBPPath", Tooltip))
	FSoftObjectPath objectBPPath;
	
public:
	FGsSchemaCostumeResPath();
	
protected:
	bool operator== (const FGsSchemaCostumeResPath& __Other) const
	{
		return FGsSchemaCostumeResPath::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCostumeResPath::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCostumeResPath::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCostumeResPathRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCostumeResPathRow() : Super()
	{
	}
	
	const FGsSchemaCostumeResPath* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCostumeResPath* Row = nullptr;
		return Table->FindRow<FGsSchemaCostumeResPath>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCostumeResPathRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCostumeResPathRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCostumeResPath final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureGenderType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Costume");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCostumeResPath*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCostumeResPath>(RowsMap))
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
	bool FindRowById(const CreatureGenderType& InId, OUT const FGsSchemaCostumeResPath*& OutRow) const
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
		const FGsSchemaCostumeResPath* Temp;
		return !FindRowById(FGsSchemaCostumeResPath().gender, Temp);
	}
#endif
};
