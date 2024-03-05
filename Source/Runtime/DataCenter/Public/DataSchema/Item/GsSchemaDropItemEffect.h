/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDropItemEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsDropItemEffect"))
struct DATACENTER_API FGsSchemaDropItemEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	DropItemEffect type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="path"))
	FSoftObjectPath path;
public:
	FGsSchemaDropItemEffect();
	
protected:
	bool operator== (const FGsSchemaDropItemEffect& __Other) const
	{
		return FGsSchemaDropItemEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDropItemEffect::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDropItemEffect::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDropItemEffectRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDropItemEffectRow() : Super()
	{
	}
	
	const FGsSchemaDropItemEffect* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDropItemEffect* Row = nullptr;
		return Table->FindRow<FGsSchemaDropItemEffect>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDropItemEffectRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDropItemEffectRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDropItemEffect final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<DropItemEffect, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaDropItemEffect*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaDropItemEffect>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->type, Row.Key);
		}
	}
	
public:
	bool FindRowById(const DropItemEffect& InId, OUT const FGsSchemaDropItemEffect*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const DropItemEffect& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaDropItemEffect* Temp;
		return !FindRowById(FGsSchemaDropItemEffect().type, Temp);
	}
#endif
};
