/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "GsSchemaCollectionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Collection/BSGsCollectionData"))
struct DATACENTER_API FGsSchemaCollectionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyType", Tooltip))
	PassivityOwnerType keyType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="note", Tooltip))
	FString note;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText", Tooltip))
	FText nameText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="descText", Tooltip))
	FText descText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath", Tooltip))
	FSoftObjectPath iconPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="smallIconPath", Tooltip))
	FSoftObjectPath smallIconPath;
	
public:
	FGsSchemaCollectionData();
	
protected:
	bool operator== (const FGsSchemaCollectionData& __Other) const
	{
		return FGsSchemaCollectionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCollectionData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCollectionData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCollectionDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCollectionDataRow() : Super()
	{
	}
	
	const FGsSchemaCollectionData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCollectionData* Row = nullptr;
		return Table->FindRow<FGsSchemaCollectionData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCollectionDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCollectionDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCollectionData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<PassivityOwnerType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Collection");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCollectionData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCollectionData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->keyType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const PassivityOwnerType& InId, OUT const FGsSchemaCollectionData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const PassivityOwnerType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCollectionData* Temp;
		return !FindRowById(FGsSchemaCollectionData().keyType, Temp);
	}
#endif
};
