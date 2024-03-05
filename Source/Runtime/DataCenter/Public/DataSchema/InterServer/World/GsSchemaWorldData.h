/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/World/GsSchemaWorldServerMapPreset.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/World/BSGsWorldData"))
struct DATACENTER_API FGsSchemaWorldData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldId"))
	int32 worldId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldName"))
	FText worldName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldPrefix"))
	FString worldPrefix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldPrefixText"))
	FText worldPrefixText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldServerMapPresetId"))
	FGsSchemaWorldServerMapPresetRow worldServerMapPresetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isInterServer"))
	bool isInterServer;
public:
	FGsSchemaWorldData();
	
protected:
	bool operator== (const FGsSchemaWorldData& __Other) const
	{
		return FGsSchemaWorldData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldDataRow() : Super()
	{
	}
	
	const FGsSchemaWorldData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldData* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldData final : public UGsTable
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
		TMap<FName, const FGsSchemaWorldData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->worldId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldData*& OutRow) const
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
		const FGsSchemaWorldData* Temp;
		return !FindRowById(FGsSchemaWorldData().worldId, Temp);
	}
#endif
};
