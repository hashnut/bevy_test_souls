/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/EffectText/GsSchemaEffectTextPassivityEffect.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEffectTextPassivity.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EffectText/BSGsEffectTextPassivity"))
struct DATACENTER_API FGsSchemaEffectTextPassivity
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityId"))
	int32 passivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectList"))
	TArray<FGsSchemaEffectTextPassivityEffect> effectList;
public:
	FGsSchemaEffectTextPassivity();
	
protected:
	bool operator== (const FGsSchemaEffectTextPassivity& __Other) const
	{
		return FGsSchemaEffectTextPassivity::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEffectTextPassivity::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEffectTextPassivity::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEffectTextPassivityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEffectTextPassivityRow() : Super()
	{
	}
	
	const FGsSchemaEffectTextPassivity* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEffectTextPassivity* Row = nullptr;
		return Table->FindRow<FGsSchemaEffectTextPassivity>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEffectTextPassivityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEffectTextPassivityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEffectTextPassivity final : public UGsTable
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
		TMap<FName, const FGsSchemaEffectTextPassivity*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEffectTextPassivity>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->passivityId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaEffectTextPassivity*& OutRow) const
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
		const FGsSchemaEffectTextPassivity* Temp;
		return !FindRowById(FGsSchemaEffectTextPassivity().passivityId, Temp);
	}
#endif
};
