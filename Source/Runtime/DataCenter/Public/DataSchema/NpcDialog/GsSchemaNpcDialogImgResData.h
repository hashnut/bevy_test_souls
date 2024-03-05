/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcDialogImgResData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsNpcDialogImgResData"))
struct DATACENTER_API FGsSchemaNpcDialogImgResData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resPath", Tooltip))
	FSoftObjectPath resPath;
public:
	FGsSchemaNpcDialogImgResData();
	
protected:
	bool operator== (const FGsSchemaNpcDialogImgResData& __Other) const
	{
		return FGsSchemaNpcDialogImgResData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcDialogImgResData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcDialogImgResData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDialogImgResDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDialogImgResDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcDialogImgResData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcDialogImgResData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcDialogImgResData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDialogImgResDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDialogImgResDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcDialogImgResData final : public UGsTable
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
		TMap<FName, const FGsSchemaNpcDialogImgResData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcDialogImgResData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcDialogImgResData*& OutRow) const
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
		const FGsSchemaNpcDialogImgResData* Temp;
		return !FindRowById(FGsSchemaNpcDialogImgResData().id, Temp);
	}
#endif
};
