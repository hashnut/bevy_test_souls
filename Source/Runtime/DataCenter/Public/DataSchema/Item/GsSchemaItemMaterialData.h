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
#include "GsSchemaItemMaterialData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemMaterialData"))
struct DATACENTER_API FGsSchemaItemMaterialData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="id 변경 금지", Tooltip="ItemMaterial Enum과 동일값"))
	int32 id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="materialType", Tooltip))
	ItemMaterial materialType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name", Tooltip))
	FText name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath", Tooltip))
	FSoftObjectPath iconPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="toolTipText", Tooltip))
	FText toolTipText;
	
public:
	FGsSchemaItemMaterialData();
	
protected:
	bool operator== (const FGsSchemaItemMaterialData& __Other) const
	{
		return FGsSchemaItemMaterialData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemMaterialData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemMaterialData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemMaterialDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemMaterialDataRow() : Super()
	{
	}
	
	const FGsSchemaItemMaterialData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemMaterialData* Row = nullptr;
		return Table->FindRow<FGsSchemaItemMaterialData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemMaterialDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemMaterialDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemMaterialData final : public UGsTable
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
		TMap<FName, const FGsSchemaItemMaterialData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemMaterialData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemMaterialData*& OutRow) const
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
		const FGsSchemaItemMaterialData* Temp;
		return !FindRowById(FGsSchemaItemMaterialData().id, Temp);
	}
#endif
};
