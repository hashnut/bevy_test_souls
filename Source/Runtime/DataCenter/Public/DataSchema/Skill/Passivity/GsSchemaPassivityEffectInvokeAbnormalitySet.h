/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInvokeAbnormalityParam.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectInvokeAbnormalitySet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/BSGsPassivityEffectInvokeAbnormalitySet"))
struct DATACENTER_API FGsSchemaPassivityEffectInvokeAbnormalitySet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param"))
	FGsSchemaPassivityEffectInvokeAbnormalityParam param;
public:
	FGsSchemaPassivityEffectInvokeAbnormalitySet();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInvokeAbnormalitySet& __Other) const
	{
		return FGsSchemaPassivityEffectInvokeAbnormalitySet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectInvokeAbnormalitySet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectInvokeAbnormalitySet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectInvokeAbnormalitySetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectInvokeAbnormalitySetRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectInvokeAbnormalitySet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectInvokeAbnormalitySet* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectInvokeAbnormalitySet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectInvokeAbnormalitySetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectInvokeAbnormalitySetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectInvokeAbnormalitySet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Passivity");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPassivityEffectInvokeAbnormalitySet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivityEffectInvokeAbnormalitySet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivityEffectInvokeAbnormalitySet*& OutRow) const
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
		const FGsSchemaPassivityEffectInvokeAbnormalitySet* Temp;
		return !FindRowById(FGsSchemaPassivityEffectInvokeAbnormalitySet().id, Temp);
	}
#endif
};
