/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaItemConsumeParticleRes.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemConsumeParticleRes"))
struct DATACENTER_API FGsSchemaItemConsumeParticleRes
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategorySub", Tooltip))
	ItemCategorySub itemCategorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="particleRes", Tooltip))
	FSoftObjectPath particleRes;
public:
	FGsSchemaItemConsumeParticleRes();
	
protected:
	bool operator== (const FGsSchemaItemConsumeParticleRes& __Other) const
	{
		return FGsSchemaItemConsumeParticleRes::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemConsumeParticleRes::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemConsumeParticleRes::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemConsumeParticleResRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemConsumeParticleResRow() : Super()
	{
	}
	
	const FGsSchemaItemConsumeParticleRes* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemConsumeParticleRes* Row = nullptr;
		return Table->FindRow<FGsSchemaItemConsumeParticleRes>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemConsumeParticleResRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemConsumeParticleResRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemConsumeParticleRes final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<ItemCategorySub, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemConsumeParticleRes*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemConsumeParticleRes>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->itemCategorySub, Row.Key);
		}
	}
	
public:
	bool FindRowById(const ItemCategorySub& InId, OUT const FGsSchemaItemConsumeParticleRes*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const ItemCategorySub& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaItemConsumeParticleRes* Temp;
		return !FindRowById(FGsSchemaItemConsumeParticleRes().itemCategorySub, Temp);
	}
#endif
};
