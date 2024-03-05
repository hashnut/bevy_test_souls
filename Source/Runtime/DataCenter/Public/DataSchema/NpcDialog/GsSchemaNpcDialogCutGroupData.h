/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/NpcDialog/GsSchemaNpcDialogCutData.h"
#include "GsSchemaNpcDialogCutGroupData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsNpcDialogCutGroupData"))
struct DATACENTER_API FGsSchemaNpcDialogCutGroupData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id", Tooltip))
	int32 Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cutDataId", Tooltip))
	TArray<FGsSchemaNpcDialogCutDataRow> cutDataId;
	
public:
	FGsSchemaNpcDialogCutGroupData();
	
protected:
	bool operator== (const FGsSchemaNpcDialogCutGroupData& __Other) const
	{
		return FGsSchemaNpcDialogCutGroupData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcDialogCutGroupData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcDialogCutGroupData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDialogCutGroupDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDialogCutGroupDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcDialogCutGroupData* GetRow() const
	{
		const FGsSchemaNpcDialogCutGroupData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaNpcDialogCutGroupData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDialogCutGroupDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDialogCutGroupDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcDialogCutGroupData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcDialog");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaNpcDialogCutGroupData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcDialogCutGroupData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcDialogCutGroupData*& OutRow) const
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
		const FGsSchemaNpcDialogCutGroupData* Temp;
		return !FindRowById(FGsSchemaNpcDialogCutGroupData().Id, Temp);
	}
#endif
};
