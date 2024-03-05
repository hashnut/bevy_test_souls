/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "GsSchemaUIColorData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsUIColorData"))
struct DATACENTER_API FGsSchemaUIColorData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="colorType", Tooltip))
	EGsUIColorType colorType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="color", Tooltip))
	FLinearColor color;
	
public:
	FGsSchemaUIColorData();
	
protected:
	bool operator== (const FGsSchemaUIColorData& __Other) const
	{
		return FGsSchemaUIColorData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUIColorData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUIColorData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUIColorDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUIColorDataRow() : Super()
	{
	}
	
	const FGsSchemaUIColorData* GetRow() const
	{
		const FGsSchemaUIColorData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaUIColorData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUIColorDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUIColorDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUIColorData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsUIColorType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUIColorData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUIColorData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->colorType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsUIColorType& InId, OUT const FGsSchemaUIColorData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsUIColorType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaUIColorData* Temp;
		return !FindRowById(FGsSchemaUIColorData().colorType, Temp);
	}
#endif
};
