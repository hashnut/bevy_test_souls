/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Map/GsMapIconType.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "GsSchemaContinentalMapIconData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsContinentalMapIconData"))
struct DATACENTER_API FGsSchemaContinentalMapIconData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconType", Tooltip))
	EGsContinentalMapIconType _iconType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconImagePath", Tooltip))
	FSoftObjectPath _iconImagePath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconShapeType", Tooltip))
	EGsIconType _iconShapeType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconColor", Tooltip))
	FLinearColor _iconColor;
	
public:
	FGsSchemaContinentalMapIconData();
	
protected:
	bool operator== (const FGsSchemaContinentalMapIconData& __Other) const
	{
		return FGsSchemaContinentalMapIconData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContinentalMapIconData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContinentalMapIconData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContinentalMapIconDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContinentalMapIconDataRow() : Super()
	{
	}
	
	const FGsSchemaContinentalMapIconData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContinentalMapIconData* Row = nullptr;
		return Table->FindRow<FGsSchemaContinentalMapIconData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContinentalMapIconDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContinentalMapIconDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContinentalMapIconData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsContinentalMapIconType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Icon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContinentalMapIconData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContinentalMapIconData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->_iconType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsContinentalMapIconType& InId, OUT const FGsSchemaContinentalMapIconData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsContinentalMapIconType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaContinentalMapIconData* Temp;
		return !FindRowById(FGsSchemaContinentalMapIconData()._iconType, Temp);
	}
#endif
};
