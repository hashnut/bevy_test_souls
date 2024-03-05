/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormalityLevel.h"
#include "GsSchemaEffectTextAbnormality.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EffectText/BSGsEffectTextAbnormality"))
struct DATACENTER_API FGsSchemaEffectTextAbnormality
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityId"))
	int32 abnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dataList"))
	TArray<FGsSchemaEffectTextAbnormalityLevel> dataList;
public:
	FGsSchemaEffectTextAbnormality();
	
protected:
	bool operator== (const FGsSchemaEffectTextAbnormality& __Other) const
	{
		return FGsSchemaEffectTextAbnormality::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEffectTextAbnormality::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEffectTextAbnormality::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEffectTextAbnormalityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEffectTextAbnormalityRow() : Super()
	{
	}
	
	const FGsSchemaEffectTextAbnormality* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEffectTextAbnormality* Row = nullptr;
		return Table->FindRow<FGsSchemaEffectTextAbnormality>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEffectTextAbnormalityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEffectTextAbnormalityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEffectTextAbnormality final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("EffectText");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEffectTextAbnormality*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEffectTextAbnormality>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->abnormalityId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaEffectTextAbnormality*& OutRow) const
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
		const FGsSchemaEffectTextAbnormality* Temp;
		return !FindRowById(FGsSchemaEffectTextAbnormality().abnormalityId, Temp);
	}
#endif
};
