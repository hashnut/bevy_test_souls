/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "GsSchemaSummonCostumeGradeRes.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Summon/BSGsSummonCostumeGradeRes"))
struct DATACENTER_API FGsSchemaSummonCostumeGradeRes
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeGrade", Tooltip))
	CostumeGrade costumeGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="summonCostumeGradeEffect", Tooltip))
	FSoftObjectPath summonCostumeGradeEffect;
public:
	FGsSchemaSummonCostumeGradeRes();
	
protected:
	bool operator== (const FGsSchemaSummonCostumeGradeRes& __Other) const
	{
		return FGsSchemaSummonCostumeGradeRes::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSummonCostumeGradeRes::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSummonCostumeGradeRes::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSummonCostumeGradeResRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSummonCostumeGradeResRow() : Super()
	{
	}
	
	const FGsSchemaSummonCostumeGradeRes* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSummonCostumeGradeRes* Row = nullptr;
		return Table->FindRow<FGsSchemaSummonCostumeGradeRes>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSummonCostumeGradeResRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSummonCostumeGradeResRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSummonCostumeGradeRes final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CostumeGrade, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Summon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSummonCostumeGradeRes*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSummonCostumeGradeRes>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->costumeGrade, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CostumeGrade& InId, OUT const FGsSchemaSummonCostumeGradeRes*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CostumeGrade& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSummonCostumeGradeRes* Temp;
		return !FindRowById(FGsSchemaSummonCostumeGradeRes().costumeGrade, Temp);
	}
#endif
};
