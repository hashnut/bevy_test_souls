/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemCraftMaterial.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/Craft/BSGsItemCraftMaterial"))
struct DATACENTER_API FGsSchemaItemCraftMaterial
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemCraftMaterialId"))
	int32 itemCraftMaterialId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemIds"))
	TArray<int32> itemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemLevels"))
	TArray<int32> itemLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대체아이템으로 아이템그룹의 모든 아이템을 자동 추가할 지 여부", DisplayName="includeItemGroup"))
	bool includeItemGroup;
public:
	FGsSchemaItemCraftMaterial();
	
protected:
	bool operator== (const FGsSchemaItemCraftMaterial& __Other) const
	{
		return FGsSchemaItemCraftMaterial::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCraftMaterial::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCraftMaterial::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCraftMaterialRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCraftMaterialRow() : Super()
	{
	}
	
	const FGsSchemaItemCraftMaterial* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCraftMaterial* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCraftMaterial>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCraftMaterialRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCraftMaterialRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCraftMaterial final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item/Craft");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemCraftMaterial*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemCraftMaterial>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->itemCraftMaterialId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemCraftMaterial*& OutRow) const
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
		const FGsSchemaItemCraftMaterial* Temp;
		return !FindRowById(FGsSchemaItemCraftMaterial().itemCraftMaterialId, Temp);
	}
#endif
};
