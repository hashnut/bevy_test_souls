/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/InterServer/World/GsSchemaMapId.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldServerMapPreset.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/World/BSGsWorldServerMapPreset"))
struct DATACENTER_API FGsSchemaWorldServerMapPreset
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="presetId"))
	int32 presetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapIdList"))
	TArray<FGsSchemaMapId> mapIdList;
public:
	FGsSchemaWorldServerMapPreset();
	
protected:
	bool operator== (const FGsSchemaWorldServerMapPreset& __Other) const
	{
		return FGsSchemaWorldServerMapPreset::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldServerMapPreset::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldServerMapPreset::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldServerMapPresetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldServerMapPresetRow() : Super()
	{
	}
	
	const FGsSchemaWorldServerMapPreset* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldServerMapPreset* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldServerMapPreset>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldServerMapPresetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldServerMapPresetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldServerMapPreset final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer/World");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWorldServerMapPreset*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldServerMapPreset>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->presetId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldServerMapPreset*& OutRow) const
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
		const FGsSchemaWorldServerMapPreset* Temp;
		return !FindRowById(FGsSchemaWorldServerMapPreset().presetId, Temp);
	}
#endif
};
