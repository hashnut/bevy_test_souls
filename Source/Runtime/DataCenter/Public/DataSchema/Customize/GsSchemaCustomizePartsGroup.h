/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Customize/GsSchemaCustomizeSlotData.h"
#include "GsSchemaCustomizePartsGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizePartsGroup"))
struct DATACENTER_API FGsSchemaCustomizePartsGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc", Tooltip))
	FString desc;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotList", Tooltip))
	TArray<FGsSchemaCustomizeSlotData> slotList;
	
public:
	FGsSchemaCustomizePartsGroup();
	
protected:
	bool operator== (const FGsSchemaCustomizePartsGroup& __Other) const
	{
		return FGsSchemaCustomizePartsGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizePartsGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCustomizePartsGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizePartsGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizePartsGroupRow() : Super()
	{
	}
	
	const FGsSchemaCustomizePartsGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCustomizePartsGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaCustomizePartsGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizePartsGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizePartsGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizePartsGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Customize");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCustomizePartsGroup*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCustomizePartsGroup>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCustomizePartsGroup*& OutRow) const
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
		const FGsSchemaCustomizePartsGroup* Temp;
		return !FindRowById(FGsSchemaCustomizePartsGroup().id, Temp);
	}
#endif
};
