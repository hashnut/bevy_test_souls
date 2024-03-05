/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "GameObject/Define/EGsDropItemCategory.h"
#include "GsSchemaDropObject.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/DropObject/BSGsDropObject"))
struct DATACENTER_API FGsSchemaDropObject
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	EGsDropItemCategory id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shapeId", Tooltip))
	FGsSchemaShapeDataRow shapeId;
	
public:
	FGsSchemaDropObject();
	
protected:
	bool operator== (const FGsSchemaDropObject& __Other) const
	{
		return FGsSchemaDropObject::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDropObject::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDropObject::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDropObjectRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDropObjectRow() : Super()
	{
	}
	
	const FGsSchemaDropObject* GetRow() const
	{
		const FGsSchemaDropObject* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaDropObject>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDropObjectRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDropObjectRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDropObject final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsDropItemCategory, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/DropObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaDropObject*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaDropObject>(RowsMap))
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
	bool FindRowById(const EGsDropItemCategory& InId, OUT const FGsSchemaDropObject*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsDropItemCategory& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaDropObject* Temp;
		return !FindRowById(FGsSchemaDropObject().id, Temp);
	}
#endif
};
