/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "Runtime/DataCenter/Public/Option/GsOptionData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionItemGainData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionItemGainData"))
struct DATACENTER_API FGsSchemaOptionItemGainData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gainType"))
	EGsOptionItem gainType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategoryInfoList"))
	TArray<FGsNoneEquipItemCategoryOptionInfo> itemCategoryInfoList;
public:
	FGsSchemaOptionItemGainData();
	
protected:
	bool operator== (const FGsSchemaOptionItemGainData& __Other) const
	{
		return FGsSchemaOptionItemGainData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionItemGainData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionItemGainData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionItemGainDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionItemGainDataRow() : Super()
	{
	}
	
	const FGsSchemaOptionItemGainData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionItemGainData* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionItemGainData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionItemGainDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionItemGainDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionItemGainData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsOptionItem, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaOptionItemGainData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaOptionItemGainData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->gainType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsOptionItem& InId, OUT const FGsSchemaOptionItemGainData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsOptionItem& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaOptionItemGainData* Temp;
		return !FindRowById(FGsSchemaOptionItemGainData().gainType, Temp);
	}
#endif
};
