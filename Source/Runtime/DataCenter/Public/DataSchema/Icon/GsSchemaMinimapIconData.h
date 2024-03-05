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
#include "GsSchemaMinimapIconData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsMinimapIconData"))
struct DATACENTER_API FGsSchemaMinimapIconData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconType", Tooltip))
	EGsMapIconType _iconType;
	
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
	FGsSchemaMinimapIconData();
	
protected:
	bool operator== (const FGsSchemaMinimapIconData& __Other) const
	{
		return FGsSchemaMinimapIconData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMinimapIconData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMinimapIconData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMinimapIconDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMinimapIconDataRow() : Super()
	{
	}
	
	const FGsSchemaMinimapIconData* GetRow() const
	{
		const FGsSchemaMinimapIconData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaMinimapIconData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMinimapIconDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMinimapIconDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMinimapIconData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsMapIconType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Icon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMinimapIconData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMinimapIconData>(RowsMap))
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
	bool FindRowById(const EGsMapIconType& InId, OUT const FGsSchemaMinimapIconData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsMapIconType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaMinimapIconData* Temp;
		return !FindRowById(FGsSchemaMinimapIconData()._iconType, Temp);
	}
#endif
};
