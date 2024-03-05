/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "GsSchemaQuestSlotData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UI/Quest/BSGsQuestSlotData"))
struct DATACENTER_API FGsSchemaQuestSlotData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	EGsQuestSlotType id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type", Tooltip))
	int32 type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="typeInfo", Tooltip))
	FGsSchemaRowPoly typeInfo;
	
public:
	FGsSchemaQuestSlotData();
	
protected:
	bool operator== (const FGsSchemaQuestSlotData& __Other) const
	{
		return FGsSchemaQuestSlotData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestSlotData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestSlotData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestSlotDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestSlotDataRow() : Super()
	{
	}
	
	const FGsSchemaQuestSlotData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestSlotData* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestSlotData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestSlotDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestSlotDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestSlotData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsQuestSlotType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("UI/Quest");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestSlotData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestSlotData>(RowsMap))
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
	bool FindRowById(const EGsQuestSlotType& InId, OUT const FGsSchemaQuestSlotData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsQuestSlotType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaQuestSlotData* Temp;
		return !FindRowById(FGsSchemaQuestSlotData().id, Temp);
	}
#endif
};
