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
#include "Map/GsMapIconType.h"
#include "GsSchemaRegionMapIconData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsRegionMapIconData"))
struct DATACENTER_API FGsSchemaRegionMapIconData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconType", Tooltip))
	EGsRegionMapIconType _iconType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_depth", Tooltip))
	int32 _depth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_updateTime", Tooltip))
	float _updateTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconImagePath", Tooltip))
	FSoftObjectPath _iconImagePath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconShapeType", Tooltip))
	EGsIconType _iconShapeType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconColor", Tooltip))
	FLinearColor _iconColor;
	
public:
	FGsSchemaRegionMapIconData();
	
protected:
	bool operator== (const FGsSchemaRegionMapIconData& __Other) const
	{
		return FGsSchemaRegionMapIconData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRegionMapIconData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRegionMapIconData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRegionMapIconDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRegionMapIconDataRow() : Super()
	{
	}
	
	const FGsSchemaRegionMapIconData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRegionMapIconData* Row = nullptr;
		return Table->FindRow<FGsSchemaRegionMapIconData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRegionMapIconDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRegionMapIconDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRegionMapIconData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsRegionMapIconType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Icon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaRegionMapIconData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRegionMapIconData>(RowsMap))
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
	bool FindRowById(const EGsRegionMapIconType& InId, OUT const FGsSchemaRegionMapIconData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsRegionMapIconType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaRegionMapIconData* Temp;
		return !FindRowById(FGsSchemaRegionMapIconData()._iconType, Temp);
	}
#endif
};
