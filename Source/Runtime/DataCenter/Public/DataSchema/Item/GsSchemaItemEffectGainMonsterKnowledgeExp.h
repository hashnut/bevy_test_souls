/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectGainMonsterKnowledgeExp.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectGainMonsterKnowledgeExp"))
struct DATACENTER_API FGsSchemaItemEffectGainMonsterKnowledgeExp
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gainAmount"))
	int32 gainAmount;
public:
	FGsSchemaItemEffectGainMonsterKnowledgeExp();
	
protected:
	bool operator== (const FGsSchemaItemEffectGainMonsterKnowledgeExp& __Other) const
	{
		return FGsSchemaItemEffectGainMonsterKnowledgeExp::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectGainMonsterKnowledgeExp::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectGainMonsterKnowledgeExp::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectGainMonsterKnowledgeExpRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectGainMonsterKnowledgeExpRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectGainMonsterKnowledgeExp* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectGainMonsterKnowledgeExp* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectGainMonsterKnowledgeExp>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectGainMonsterKnowledgeExpRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectGainMonsterKnowledgeExpRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectGainMonsterKnowledgeExp final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectGainMonsterKnowledgeExp*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectGainMonsterKnowledgeExp>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectGainMonsterKnowledgeExp*& OutRow) const
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
		const FGsSchemaItemEffectGainMonsterKnowledgeExp* Temp;
		return !FindRowById(FGsSchemaItemEffectGainMonsterKnowledgeExp().effectId, Temp);
	}
#endif
};
