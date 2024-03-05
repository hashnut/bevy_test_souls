/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "GsSchemaCommonIconData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsCommonIconData"))
struct DATACENTER_API FGsSchemaCommonIconData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconType", Tooltip))
	EGsIconType iconType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="note", Tooltip))
	FString note;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="widgetPath", Tooltip))
	FSoftObjectPath widgetPath;
	
public:
	FGsSchemaCommonIconData();
protected:
	bool operator== (const FGsSchemaCommonIconData& __Other) const
	{
		return FGsSchemaCommonIconData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommonIconData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const override
	{
		return FGsSchemaCommonIconData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommonIconDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommonIconDataRow() : Super()
	{
	}
	
	const UGsTable* GetTable() const
	{
		return FGsSchemaCommonIconData::GetStaticTable();
	}
	
	const FGsSchemaCommonIconData* GetRow() const
	{
		const FGsSchemaCommonIconData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaCommonIconData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommonIconDataRow>
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommonIconDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommonIconData : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsIconType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const override
	{
		return TEXT("Icon");
	}
	
	void OnCachedDataTableChanged() override
	{
		TMap<FName, const FGsSchemaCommonIconData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCommonIconData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->iconType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsIconType& InId, OUT const FGsSchemaCommonIconData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsIconType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const override
	{
		const FGsSchemaCommonIconData* Temp;
		return !FindRowById(FGsSchemaCommonIconData().iconType, Temp);
	}
#endif
};
