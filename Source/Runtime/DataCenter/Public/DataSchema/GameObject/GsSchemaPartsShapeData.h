/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaPartsShapeInfo.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsPartsType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPartsShapeData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsPartsShapeData"))
struct DATACENTER_API FGsSchemaPartsShapeData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="partsType"))
	EGsPartsType partsType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shapeInfo"))
	TMap<CreatureGenderType,FGsSchemaPartsShapeInfo> shapeInfo;
public:
	FGsSchemaPartsShapeData();
	
protected:
	bool operator== (const FGsSchemaPartsShapeData& __Other) const
	{
		return FGsSchemaPartsShapeData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPartsShapeData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPartsShapeData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPartsShapeDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPartsShapeDataRow() : Super()
	{
	}
	
	const FGsSchemaPartsShapeData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPartsShapeData* Row = nullptr;
		return Table->FindRow<FGsSchemaPartsShapeData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPartsShapeDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPartsShapeDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePartsShapeData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPartsShapeData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPartsShapeData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPartsShapeData*& OutRow) const
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
		const FGsSchemaPartsShapeData* Temp;
		return !FindRowById(FGsSchemaPartsShapeData().id, Temp);
	}
#endif
};
