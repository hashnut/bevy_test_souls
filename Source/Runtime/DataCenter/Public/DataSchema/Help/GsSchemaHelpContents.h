/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaHelpContents.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Help/BSGsHelpContents"))
struct DATACENTER_API FGsSchemaHelpContents
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guideKey"))
	EGsHelpContents guideKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="customeViewId"))
	TMap<FString,int64> customeViewId;
public:
	FGsSchemaHelpContents();
	
protected:
	bool operator== (const FGsSchemaHelpContents& __Other) const
	{
		return FGsSchemaHelpContents::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaHelpContents::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaHelpContents::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaHelpContentsRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaHelpContentsRow() : Super()
	{
	}
	
	const FGsSchemaHelpContents* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaHelpContents* Row = nullptr;
		return Table->FindRow<FGsSchemaHelpContents>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaHelpContentsRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaHelpContentsRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableHelpContents final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsHelpContents, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Help");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaHelpContents*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaHelpContents>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->guideKey, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsHelpContents& InId, OUT const FGsSchemaHelpContents*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsHelpContents& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaHelpContents* Temp;
		return !FindRowById(FGsSchemaHelpContents().guideKey, Temp);
	}
#endif
};
