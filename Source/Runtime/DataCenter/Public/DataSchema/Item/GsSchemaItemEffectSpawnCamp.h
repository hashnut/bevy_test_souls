/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectSpawnCamp.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectSpawnCamp"))
struct DATACENTER_API FGsSchemaItemEffectSpawnCamp
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="allowedMapIdList"))
	TArray<int32> allowedMapIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enemyCampDistance"))
	int32 enemyCampDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="otherCampDistance"))
	int32 otherCampDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="campId"))
	int32 campId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="campLevel"))
	int32 campLevel;
public:
	FGsSchemaItemEffectSpawnCamp();
	
protected:
	bool operator== (const FGsSchemaItemEffectSpawnCamp& __Other) const
	{
		return FGsSchemaItemEffectSpawnCamp::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectSpawnCamp::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectSpawnCamp::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectSpawnCampRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectSpawnCampRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectSpawnCamp* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectSpawnCamp* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectSpawnCamp>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectSpawnCampRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectSpawnCampRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectSpawnCamp final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectSpawnCamp*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectSpawnCamp>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectSpawnCamp*& OutRow) const
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
		const FGsSchemaItemEffectSpawnCamp* Temp;
		return !FindRowById(FGsSchemaItemEffectSpawnCamp().effectId, Temp);
	}
#endif
};
