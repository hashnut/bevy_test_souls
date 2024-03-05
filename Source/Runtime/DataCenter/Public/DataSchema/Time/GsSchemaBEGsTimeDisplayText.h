/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/Time/GsSchemaCultureTimeText.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBEGsTimeDisplayText.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Time/BEGsTimeDisplayText"))
struct DATACENTER_API FGsSchemaBEGsTimeDisplayText
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	EGsTimeDisplayType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cultureTimeText"))
	TArray<FGsSchemaCultureTimeText> cultureTimeText;
public:
	FGsSchemaBEGsTimeDisplayText();
	
protected:
	bool operator== (const FGsSchemaBEGsTimeDisplayText& __Other) const
	{
		return FGsSchemaBEGsTimeDisplayText::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBEGsTimeDisplayText::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBEGsTimeDisplayText::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBEGsTimeDisplayTextRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBEGsTimeDisplayTextRow() : Super()
	{
	}
	
	const FGsSchemaBEGsTimeDisplayText* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBEGsTimeDisplayText* Row = nullptr;
		return Table->FindRow<FGsSchemaBEGsTimeDisplayText>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBEGsTimeDisplayTextRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBEGsTimeDisplayTextRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBEGsTimeDisplayText final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsTimeDisplayType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Time");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaBEGsTimeDisplayText*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBEGsTimeDisplayText>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->type, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsTimeDisplayType& InId, OUT const FGsSchemaBEGsTimeDisplayText*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsTimeDisplayType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaBEGsTimeDisplayText* Temp;
		return !FindRowById(FGsSchemaBEGsTimeDisplayText().type, Temp);
	}
#endif
};
