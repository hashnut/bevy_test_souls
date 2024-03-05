/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlate.h"
#include "GsSchemaItemEffectUnlockMonsterKnowledgeSlate.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectUnlockMonsterKnowledgeSlate"))
struct DATACENTER_API FGsSchemaItemEffectUnlockMonsterKnowledgeSlate
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SlateId"))
	FGsSchemaMonsterKnowledgeSlateRow SlateId;
public:
	FGsSchemaItemEffectUnlockMonsterKnowledgeSlate();
	
protected:
	bool operator== (const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate& __Other) const
	{
		return FGsSchemaItemEffectUnlockMonsterKnowledgeSlate::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectUnlockMonsterKnowledgeSlate::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectUnlockMonsterKnowledgeSlate::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectUnlockMonsterKnowledgeSlateRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectUnlockMonsterKnowledgeSlateRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectUnlockMonsterKnowledgeSlate>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectUnlockMonsterKnowledgeSlateRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectUnlockMonsterKnowledgeSlateRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectUnlockMonsterKnowledgeSlate final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate*& OutRow) const
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
		const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate* Temp;
		return !FindRowById(FGsSchemaItemEffectUnlockMonsterKnowledgeSlate().id, Temp);
	}
#endif
};
