/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaShapeData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsShapeData"))
struct DATACENTER_API FGsSchemaShapeData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bPClass"))
	FSoftObjectPath bPClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(AllowedClasses="SkeletalMesh", DisplayName="skeletalMeshPath"))
	FSoftObjectPath skeletalMeshPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(AllowedClasses="StaticMesh", DisplayName="staticMeshPath"))
	FSoftObjectPath staticMeshPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="imageRegionMap"))
	FSoftObjectPath imageRegionMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hitAnimInstancePath"))
	FSoftObjectPath hitAnimInstancePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTransparency"))
	bool isTransparency;
public:
	FGsSchemaShapeData();
	
protected:
	bool operator== (const FGsSchemaShapeData& __Other) const
	{
		return FGsSchemaShapeData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaShapeData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaShapeData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaShapeDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaShapeDataRow() : Super()
	{
	}
	
	const FGsSchemaShapeData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaShapeData* Row = nullptr;
		return Table->FindRow<FGsSchemaShapeData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaShapeDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaShapeDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableShapeData final : public UGsTable
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
		TMap<FName, const FGsSchemaShapeData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaShapeData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaShapeData*& OutRow) const
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
		const FGsSchemaShapeData* Temp;
		return !FindRowById(FGsSchemaShapeData().id, Temp);
	}
#endif
};
