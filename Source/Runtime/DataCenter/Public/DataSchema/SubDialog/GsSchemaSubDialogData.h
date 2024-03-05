/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogCutData.h"
#include "GsSchemaSubDialogData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SubDialog/BSGsSubDialogData"))
struct DATACENTER_API FGsSchemaSubDialogData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id", Tooltip))
	int32 Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cutIdList", Tooltip))
	TArray<FGsSchemaSubDialogCutDataRow> cutIdList;
	
public:
	FGsSchemaSubDialogData();
	
protected:
	bool operator== (const FGsSchemaSubDialogData& __Other) const
	{
		return FGsSchemaSubDialogData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSubDialogData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSubDialogData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSubDialogDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSubDialogDataRow() : Super()
	{
	}
	
	const FGsSchemaSubDialogData* GetRow() const
	{
		const FGsSchemaSubDialogData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaSubDialogData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSubDialogDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSubDialogDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSubDialogData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SubDialog");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSubDialogData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSubDialogData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->Id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSubDialogData*& OutRow) const
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
		const FGsSchemaSubDialogData* Temp;
		return !FindRowById(FGsSchemaSubDialogData().Id, Temp);
	}
#endif
};
