/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "GsSchemaSummonFairyGradeRes.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Summon/BSGsSummonFairyGradeRes"))
struct DATACENTER_API FGsSchemaSummonFairyGradeRes
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyGrade", Tooltip))
	FairyGrade fairyGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="summonFairyGradeEffect", Tooltip))
	FSoftObjectPath summonFairyGradeEffect;
public:
	FGsSchemaSummonFairyGradeRes();
	
protected:
	bool operator== (const FGsSchemaSummonFairyGradeRes& __Other) const
	{
		return FGsSchemaSummonFairyGradeRes::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSummonFairyGradeRes::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSummonFairyGradeRes::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSummonFairyGradeResRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSummonFairyGradeResRow() : Super()
	{
	}
	
	const FGsSchemaSummonFairyGradeRes* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSummonFairyGradeRes* Row = nullptr;
		return Table->FindRow<FGsSchemaSummonFairyGradeRes>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSummonFairyGradeResRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSummonFairyGradeResRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSummonFairyGradeRes final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<FairyGrade, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Summon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSummonFairyGradeRes*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSummonFairyGradeRes>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->fairyGrade, Row.Key);
		}
	}
	
public:
	bool FindRowById(const FairyGrade& InId, OUT const FGsSchemaSummonFairyGradeRes*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const FairyGrade& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSummonFairyGradeRes* Temp;
		return !FindRowById(FGsSchemaSummonFairyGradeRes().fairyGrade, Temp);
	}
#endif
};
